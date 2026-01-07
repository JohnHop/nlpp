#include "NetlinkGeneric.hpp"


#include "nlattr_t.hpp"
#include "nlpp.hpp"

#include <netlink/attr.h>
#include <netlink/msg.h>
#include <linux/nl80211.h>

#include <stdexcept>
#include <system_error>
#include <cstdarg>


using namespace nlpp;


NetlinkGeneric::NetlinkGeneric()
{
  socket_.connect(netlink_protocol_e::generic);

  this->nl80211_id = genl_ctrl_resolve(this->socket_.get_pointer(), "nl80211");
  if(nl80211_id < 0) {
    throw 
      std::system_error{nl80211_id, std::system_category(), "nl80211 not found"};
  }
}


// TODO: fai in modo che il costruttore di `if_index_t` possa fallire.
dev_info_t NetlinkGeneric::get_interface(if_index_t ifindex)
{
  std::map<uint32_t,dev_info_t> interface_info; // key is device index

  // check pre-condition
  if(ifindex.get() == 0) {
    throw std::invalid_argument{"interface index cannot be zero"};
  }

  nlmsg_t msg{this->nl80211_id, nl80211_commands::NL80211_CMD_GET_INTERFACE};
  msg.put_attr({nl80211_attrs::NL80211_ATTR_IFINDEX, ifindex.get()});

  this->send_msg(msg, NetlinkGeneric::get_interface_handler, &interface_info);
  
  return interface_info.at(ifindex.get());
}


std::map<uint32_t,dev_info_t> NetlinkGeneric::get_list_interfaces()
{
  std::map<uint32_t,dev_info_t> result; // key is device index

  nlmsg_t msg{
    this->nl80211_id, nl80211_commands::NL80211_CMD_GET_INTERFACE, 768};

  this->send_msg(msg, NetlinkGeneric::get_interface_handler, &result);

  return result;
}


dev_capability_t NetlinkGeneric::get_phy(wiphy_index_t phy_index)
{
  std::map<uint32_t,dev_capability_t> result;

  // first, send `NL80211_CMD_GET_PROTOCOL_FEATURES`
  bool nl80211_has_split_wiphy{};

  nlmsg_t msg{this->nl80211_id,
    nl80211_commands::NL80211_CMD_GET_PROTOCOL_FEATURES};

  this->send_msg(msg, &NetlinkGeneric::get_feature_handler, 
    &nl80211_has_split_wiphy);

  // then, send `NL80211_CMD_GET_WIPHY` with appropriate flags
  msg = nlmsg_t{this->nl80211_id, nl80211_commands::NL80211_CMD_GET_WIPHY};

  msg.put_attr({NL80211_ATTR_WIPHY, static_cast<uint32_t>(phy_index.get())} );

  if(nl80211_has_split_wiphy) {
    msg.put_flag(NL80211_ATTR_SPLIT_WIPHY_DUMP);
    msg.nlmsg_hdr()->nlmsg_flags |= NLM_F_DUMP;
  }

  this->send_msg(msg, &NetlinkGeneric::get_phy_handler, &result);

  return result.at(phy_index.get());
}


std::map<uint32_t,dev_capability_t> NetlinkGeneric::get_list_phys()
{
  std::map<uint32_t,dev_capability_t> result;

  // first, send `NL80211_CMD_GET_PROTOCOL_FEATURES`
  bool nl80211_has_split_wiphy{};

  nlmsg_t msg{this->nl80211_id,
    nl80211_commands::NL80211_CMD_GET_PROTOCOL_FEATURES};

  this->send_msg(msg,
    &NetlinkGeneric::get_feature_handler, &nl80211_has_split_wiphy);

  // then, send `NL80211_CMD_GET_WIPHY` with appropriate flags
  msg = nlmsg_t{this->nl80211_id, nl80211_commands::NL80211_CMD_GET_WIPHY};

  if(nl80211_has_split_wiphy) {
    msg.put_flag(NL80211_ATTR_SPLIT_WIPHY_DUMP);
    msg.nlmsg_hdr()->nlmsg_flags |= NLM_F_DUMP;
  }

  this->send_msg(std::move(msg),
    &NetlinkGeneric::get_phy_handler, &result);

  return result;
}


void NetlinkGeneric::set_if_type(std::string const& ifname, if_type_e type)
{
  uint32_t const ifindex = if_nametoindex(ifname.data());

  nlmsg_t msg{this->nl80211_id, nl80211_commands::NL80211_CMD_SET_INTERFACE};
  msg.put_attr(
    nlattr_t{nl80211_attrs::NL80211_ATTR_IFINDEX, ifindex},
    nlattr_t{nl80211_attrs::NL80211_ATTR_IFTYPE, static_cast<uint32_t>(type)} );

  this->send_msg(msg);
}


void NetlinkGeneric::set_if_channel(std::string const& ifname, freq_chan_t chan)
{
  uint32_t const ifindex = if_nametoindex(ifname.data());

  nlmsg_t msg{this->nl80211_id, nl80211_commands::NL80211_CMD_SET_WIPHY};
  msg.put_attr(
    nlattr_t{nl80211_attrs::NL80211_ATTR_IFINDEX, ifindex},
    nlattr_t{nl80211_attrs::NL80211_ATTR_IFTYPE, 
      static_cast<uint32_t>(if_type_e::monitor)},
    nlattr_t{nl80211_attrs::NL80211_ATTR_WIPHY_FREQ, chan2freq(chan).get()},
    nlattr_t{nl80211_attrs::NL80211_ATTR_WIPHY_CHANNEL_TYPE, 
      static_cast<uint32_t>(nl80211_channel_type::NL80211_CHAN_NO_HT)}
  );

  this->send_msg(std::move(msg));
}


void NetlinkGeneric::send_msg(nlmsg_t const& msg, nl_recvmsg_msg_cb_t fun, void* arg)
{
  nlcb_t cb{NL_CB_DEFAULT};
  nlcb_t s_cb{NL_CB_DEFAULT}; // send cb

  // put handler
  if(fun) {
    cb.set(NL_CB_VALID, NL_CB_CUSTOM, fun, arg);
  }

  socket_.set_cb(std::move(s_cb));
  socket_.send_auto(msg);

  socket_.recvmsgs(cb);
}


/**
 * Parse interface info collecting these attributes inside a `dev_info_t` 
 *  + NL80211_ATTR_IFNAME
 *  + NL80211_ATTR_IFINDEX
 *  + NL80211_ATTR_WDEV
 *  - NL80211_ATTR_MAC
 *  - NL80211_ATTR_SSID
 *  + NL80211_ATTR_IFTYPE: managed, monitor, ...
 *  + NL80211_ATTR_WIPHY: dispositivo fisico
 *  + NL80211_ATTR_WIPHY_FREQ
 *  + NL80211_ATTR_CHANNEL_WIDTH
 *  - NL80211_ATTR_CENTER_FREQ1
 * 
 * See `iw` source code, file `interface.c`, line 285.
 *
 * This function is invoked a single time for each existing interface, also for 
 * "Unnamed/non-netdev interface".
 */
int NetlinkGeneric::get_interface_handler(struct nl_msg* msg, void* arg) noexcept
{
  dev_info_t dev_info;

  auto* resultPtr = reinterpret_cast<std::map<uint32_t,dev_info_t>*>(arg);

  auto gnlh = reinterpret_cast<struct genlmsghdr*>(nlmsg_data(nlmsg_hdr(msg)));
  struct nlattr* tb_msg[NL80211_ATTR_MAX + 1];
  uint32_t ifindex = 0;

  nla_parse(tb_msg, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), 
    genlmsg_attrlen(gnlh, 0), nullptr);

  if(tb_msg[NL80211_ATTR_IFNAME]) {
    dev_info.if_name = std::string(
      nla_get_string(tb_msg[NL80211_ATTR_IFNAME]));
  }
  if(tb_msg[NL80211_ATTR_IFINDEX]) {
    dev_info.if_index.get() 
      = ifindex = nla_get_u32(tb_msg[NL80211_ATTR_IFINDEX]);
  }
  if(tb_msg[NL80211_ATTR_WDEV]) {
    dev_info.wdev = nla_get_u64(tb_msg[NL80211_ATTR_WDEV]);
  }
  // if(tb_msg[NL80211_ATTR_MAC]) { }
  // if(tb_msg[NL80211_ATTR_SSID]) { }
  if(tb_msg[NL80211_ATTR_IFTYPE]) {
    dev_info.type = 
      static_cast<if_type_e>(nla_get_u32(tb_msg[NL80211_ATTR_IFTYPE]));
  }
  if(tb_msg[NL80211_ATTR_WIPHY]) {
    dev_info.wiphy_index.get() = nla_get_u32(tb_msg[NL80211_ATTR_WIPHY]);
  }
  if(tb_msg[NL80211_ATTR_WIPHY_FREQ]) 
  {
    dev_info.wiphy_freq.get() = nla_get_u32(tb_msg[NL80211_ATTR_WIPHY_FREQ]);

    if(tb_msg[NL80211_ATTR_CHANNEL_WIDTH]) {
      dev_info.channel_width = 
        nla_get_u32(tb_msg[NL80211_ATTR_CHANNEL_WIDTH]);
    }
  }

  // for now, we insert the device info inside `resultPtr` map only if it is not
  // a "Unnamed/non-netdev interface"
  if(ifindex) {
    resultPtr->insert({ifindex,std::move(dev_info)});
  }

  return NL_SKIP;
}


int NetlinkGeneric::get_feature_handler(struct nl_msg *msg, void *arg) noexcept
{
  bool* nl80211_has_split_wiphy = reinterpret_cast<bool*>(arg);

  struct nlattr* tb_msg[NL80211_ATTR_MAX + 1];
	auto gnlh = reinterpret_cast<struct genlmsghdr*>(nlmsg_data(nlmsg_hdr(msg)));

  nla_parse(tb_msg, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
		  genlmsg_attrlen(gnlh, 0), nullptr);

  if(tb_msg[NL80211_ATTR_PROTOCOL_FEATURES]) 
  {
		uint32_t feat = nla_get_u32(tb_msg[NL80211_ATTR_PROTOCOL_FEATURES]);

		if(feat & NL80211_PROTOCOL_FEATURE_SPLIT_WIPHY_DUMP) {
			*nl80211_has_split_wiphy = true;
		}
	}

  return NL_SKIP;
}


int NetlinkGeneric::get_phy_handler(struct nl_msg* msg, void* arg) noexcept
{
  struct nlattr* tb_msg[NL80211_ATTR_MAX + 1];
	auto gnlh = reinterpret_cast<struct genlmsghdr*>(nlmsg_data(nlmsg_hdr(msg)));
  struct nlattr* tb_band[NL80211_BAND_ATTR_MAX + 1];
	struct nlattr* tb_freq[NL80211_FREQUENCY_ATTR_MAX + 1];

  static struct nla_policy freq_policy[NL80211_FREQUENCY_ATTR_MAX + 1]{};
  freq_policy[NL80211_FREQUENCY_ATTR_FREQ] = { .type = NLA_U32 };
  freq_policy[NL80211_FREQUENCY_ATTR_DISABLED] = { .type = NLA_FLAG };
  freq_policy[NL80211_FREQUENCY_ATTR_PASSIVE_SCAN] = { .type = NLA_FLAG };
  freq_policy[NL80211_FREQUENCY_ATTR_NO_IBSS] = { .type = NLA_FLAG };
  freq_policy[NL80211_FREQUENCY_ATTR_RADAR] = { .type = NLA_FLAG };
  freq_policy[NL80211_FREQUENCY_ATTR_MAX_TX_POWER] = { .type = NLA_U32 };

  static int last_band = -1;
  static uint32_t phy_id = -1;
  static bool band_had_freq = false;

  // Get the return value pointer
  auto resultPtr 
    = reinterpret_cast<std::map<uint32_t,dev_capability_t>*>(arg);

  nla_parse(tb_msg, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
    genlmsg_attrlen(gnlh, 0), nullptr);

  if(tb_msg[NL80211_ATTR_WIPHY])
  {
    if(nla_get_u32(tb_msg[NL80211_ATTR_WIPHY]) != phy_id) {
      last_band = -1;
    }
    
    phy_id = nla_get_u32(tb_msg[NL80211_ATTR_WIPHY]);
    if(not resultPtr->contains(phy_id)) {
      resultPtr->insert({phy_id, dev_capability_t{wiphy_index_t{phy_id}}});
    } 
  }

  if(tb_msg[NL80211_ATTR_WIPHY_NAME]) {
    resultPtr->at(phy_id).wiphy_name = nla_get_string(tb_msg[NL80211_ATTR_WIPHY_NAME]);
  }

  if(tb_msg[NL80211_ATTR_SUPPORTED_IFTYPES]) 
  {
    struct nlattr* nl_mode;
    int rem_mode;
    nla_for_each_nested(nl_mode, tb_msg[NL80211_ATTR_SUPPORTED_IFTYPES], rem_mode) {
      resultPtr->at(phy_id).iftypes.emplace_back(static_cast<if_type_e>(nla_type(nl_mode)));
    }
  }

  if(tb_msg[NL80211_ATTR_WIPHY_BANDS])
  {
    struct nlattr* nl_band;
    int rem_band;

    nla_for_each_nested(nl_band, tb_msg[NL80211_ATTR_WIPHY_BANDS], rem_band) 
    {
      if(last_band != nl_band->nla_type) {
        band_had_freq = false;
      }
      last_band = nl_band->nla_type;

      nla_parse(tb_band, NL80211_BAND_ATTR_MAX, 
        reinterpret_cast<struct nlattr*>(nla_data(nl_band)), nla_len(nl_band),
        nullptr);

      if(tb_band[NL80211_BAND_ATTR_FREQS]) 
      {
        if(!band_had_freq) {
          band_had_freq = true;
        }

        struct nlattr *nl_freq;
        int rem_freq;
        nla_for_each_nested(nl_freq, tb_band[NL80211_BAND_ATTR_FREQS], rem_freq)
        {
          nla_parse(tb_freq, NL80211_FREQUENCY_ATTR_MAX, 
            reinterpret_cast<struct nlattr*>(nla_data(nl_freq)),
            nla_len(nl_freq), freq_policy);
            
					if(!tb_freq[NL80211_FREQUENCY_ATTR_FREQ]) {
            continue;
          }

          resultPtr->at(phy_id).freqs.emplace_back(
            nla_get_u32(tb_freq[NL80211_FREQUENCY_ATTR_FREQ]) );
        }
      }
    }
  }

  if(tb_msg[NL80211_ATTR_SUPPORTED_COMMANDS]) 
  {
    struct nlattr *nl_cmd;
    int rem_cmd;
    nla_for_each_nested(nl_cmd, tb_msg[NL80211_ATTR_SUPPORTED_COMMANDS], rem_cmd)
    {
      resultPtr->at(phy_id).cmds.push_back(
        static_cast<nl80211_command_e>(nla_get_u32(nl_cmd)));
    }
  }

  return NL_SKIP;
}