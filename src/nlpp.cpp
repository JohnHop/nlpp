#include "nlpp.hpp"


#include <netlink/route/link.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <array>
#include <format>


using namespace nlpp;


std::string_view nlpp::to_string(if_flag_e const flag)
{
  using namespace std::string_view_literals;

	std::string_view result;

	switch(flag)
	{
		case if_flag_e::up: 						result = "up"sv; 							break;
		case if_flag_e::broadcast: 			result = "broadcast"sv; 			break;
		case if_flag_e::debug: 					result = "debug"sv; 					break;
		case if_flag_e::loopback: 			result = "loopback"sv; 				break;
		case if_flag_e::point_to_point: result = "point_to_point"sv;	break;
		case if_flag_e::no_trailers: 		result = "no_trailers"sv; 		break;
		case if_flag_e::running: 				result = "running"sv; 				break;
		case if_flag_e::no_arp: 				result = "no_arp"sv; 					break;
		case if_flag_e::promisc: 				result = "promisc"sv; 				break;
		case if_flag_e::all_multi: 			result = "all_multi"sv; 			break;
		case if_flag_e::master: 				result = "master"sv; 					break;
		case if_flag_e::slave: 					result = "slave"sv; 					break;
		case if_flag_e::multicast: 			result = "multicast"sv; 			break;
		case if_flag_e::port_sel: 			result = "port_sel"sv; 				break;
		case if_flag_e::automedia: 			result = "automedia"sv; 			break;
		case if_flag_e::dynamic:	 			result = "dynamic"sv; 				break;
		case if_flag_e::lower_up: 			result = "lower_up"sv; 				break;
		case if_flag_e::dormant: 				result = "dormant"sv;	 				break;
		case if_flag_e::echo: 					result = "echo"sv; 						break;
	};

  return result;
}


std::string nlpp::to_string(if_operstate_e const operstate)
{
  // the longest operational status code string is 14 bytes
	std::array<char,15> buff{};	// fixme: usa direttamente std::string ?

	rtnl_link_operstate2str(
    std::to_underlying(operstate), 
		buff.data(), 
		buff.size() );

	return std::string(buff.data());
}


std::string_view nlpp::to_string(if_type_e const type)
{
  using namespace std::string_view_literals;

	static constexpr std::array strings{
		"unspecified"sv,
		"adhoc"sv,
		"managed"sv,
		"ap"sv,
		"ap_vlan"sv,
		"wds"sv,
		"monitor"sv,
		"mesh_point"sv,
		"p2p_client"sv,
		"p2p_go"sv,
		"p2p_device"sv,
		"ocb"sv,
		"nan"sv,
	};

  return strings[std::to_underlying(type)];
}


bool dev_capability_t::is_supported(if_type_e const mode) const 
{
  return 
    std::ranges::find(this->iftypes, mode) != std::ranges::end(this->iftypes);
}


bool dev_capability_t::is_supported(frequency_t const freq) const 
{
  return 
    std::ranges::find(this->freqs, freq) != std::ranges::end(this->freqs);
}


bool dev_capability_t::is_supported(channel_freq_t const chan) const 
{
  return
    std::ranges::find(this->freqs, chan, freq2chan) 
      != std::ranges::end(this->freqs);
}


bool dev_capability_t::is_supported(nl80211_command_e const cmd) const 
{
  return std::ranges::find(this->cmds, cmd) != std::ranges::end(this->cmds);
}


std::string nlpp::to_string(if_flags_t const flags)
{
	std::string result;
  
  auto const bitset = flags.get();
	for(int i = 0; i < bitset.size()-1; ++i)
	{
		auto const flag = (bitset[i] ? 1 : 0) << i;
		if(flag) {
			result += flag ? ::to_string(static_cast<if_flag_e>(flag)) : "";
			result += flag ? "," : "";
		}
	}

	result.pop_back();	// remove last comma

	return result;
}


std::string_view nlpp::to_string(nl80211_command_e const cmd)
{
	using namespace std::literals;

	static constexpr std::array strings{
		"unspec"sv,
		"get_wiphy"sv,
		"set_wiphy"sv,
		"new_wiphy"sv,
		"del_wiphy"sv,
		"get_interface"sv,
		"set_interface"sv,
		"new_interface"sv,
		"del_interface"sv,
		"get_key"sv,
		"set_key"sv,
		"new_key"sv,
		"del_key"sv,
		"get_beacon"sv,
		"set_beacon"sv,
		"start_ap"sv,
		"new_beacon"sv,
		"stop_ap"sv,
		"del_beacon"sv,
		"get_station"sv,
		"set_station"sv,
		"new_station"sv,
		"del_station"sv,
		"get_mpath"sv,
		"set_mpath"sv,
		"new_mpath"sv,
		"del_mpath"sv,
		"set_bss"sv,
		"set_reg"sv,
		"req_set_reg"sv,
		"get_mesh_config"sv,
		"set_mesh_config"sv,
		"set_mgmt_extra_ie"sv,
		"get_reg"sv,
		"get_scan"sv,
		"trigger_scan"sv,
		"new_scan_results"sv,
		"scan_aborted"sv,
		"reg_change"sv,
		"authenticate"sv,
		"associate"sv,
		"deauthenticate"sv,
		"disassociate"sv,
		"michael_mic_failure"sv,
		"reg_beacon_hint"sv,
		"join_ibss"sv,
		"leave_ibss"sv,
		"testmode"sv,
		"connect"sv,
		"roam"sv,
		"disconnect"sv,
		"set_wiphy_netns"sv,
		"get_survey"sv,
		"new_survey_results"sv,
		"set_pmksa"sv,
		"del_pmksa"sv,
		"flush_pmksa"sv,
		"remain_on_channel"sv,
		"cancel_remain_on_channel"sv,
		"set_tx_bitrate_mask"sv,
		"register_frame"sv,
		"register_action"sv,
		"frame"sv,
		"action"sv,
		"frame_tx_status"sv,
		"action_tx_status"sv,
		"set_power_save"sv,
		"get_power_save"sv,
		"set_cqm"sv,
		"notify_cqm"sv,
		"set_channel"sv,
		"set_wds_peer"sv,
		"frame_wait_cancel"sv,
		"join_mesh"sv,
		"leave_mesh"sv,
		"unprot_deauthenticate"sv,
		"unprot_disassociate"sv,
		"new_peer_candidate"sv,
		"get_wowlan"sv,
		"set_wowlan"sv,
		"start_sched_scan"sv,
		"stop_sched_scan"sv,
		"sched_scan_results"sv,
		"sched_scan_stopped"sv,
		"set_rekey_offload"sv,
		"pmksa_candidate"sv,
		"tdls_oper"sv,
		"tdls_mgmt"sv,
		"unexpected_frame"sv,
		"probe_client"sv,
		"register_beacons"sv,
		"unexpected_4addr_frame"sv,
		"set_noack_map"sv,
		"ch_switch_notify"sv,
		"start_p2p_device"sv,
		"stop_p2p_device"sv,
		"conn_failed"sv,
		"set_mcast_rate"sv,
		"set_mac_acl"sv,
		"radar_detect"sv,
		"get_protocol_features"sv,
		"update_ft_ies"sv,
		"ft_event"sv,
		"crit_protocol_start"sv,
		"crit_protocol_stop"sv,
		"get_coalesce"sv,
		"set_coalesce"sv,
		"channel_switch"sv,
		"vendor"sv,
		"set_qos_map"sv,
		"add_tx_ts"sv,
		"del_tx_ts"sv,
		"get_mpp"sv,
		"join_ocb"sv,
		"leave_ocb"sv,
		"ch_switch_started_notify"sv,
		"tdls_channel_switch"sv,
		"tdls_cancel_channel_switch"sv,
		"wiphy_reg_change"sv,
		"abort_scan"sv,
		"start_nan"sv,
		"stop_nan"sv,
		"add_nan_function"sv,
		"del_nan_function"sv,
		"change_nan_config"sv,
		"nan_match"sv,
		"set_multicast_to_unicast"sv,
		"update_connect_params"sv,
		"set_pmk"sv,
		"del_pmk"sv,
		"port_authorized"sv,
		"reload_regdb"sv,
		"external_auth"sv,
		"sta_opmode_changed"sv,
		"control_port_frame"sv,
		"get_ftm_responder_stats"sv,
		"peer_measurement_start"sv,
		"peer_measurement_result"sv,
		"peer_measurement_complete"sv,
		"notify_radar"sv,
		"update_owe_info"sv,
		"probe_mesh_link"sv,
		"set_tid_config"sv,
		"unprot_beacon"sv,
		"control_port_frame_tx_status"sv,
		"set_sar_specs"sv,
		"obss_color_collision"sv,
		"color_change_request"sv,
		"color_change_started"sv,
		"color_change_aborted"sv,
		"color_change_completed"sv,
		"set_fils_aad"sv,
		"assoc_comeback"sv,
		"add_link"sv,
		"remove_link"sv,
		"add_link_sta"sv,
		"modify_link_sta"sv,
		"remove_link_sta"sv,
		"set_hw_timestamp"sv,
		"links_removed"sv,
		"set_tid_to_link_mapping"sv,
	};

	return strings[std::to_underlying(cmd)];
}


std::string nlpp::to_string(dev_info_t const& dev_info)
{
	auto result = std::format("ifname: {}, ifindex: {}, type: {}",
		dev_info.if_name,
		dev_info.if_index.get(),
		to_string(dev_info.type));

	if(dev_info.type == if_type_e::monitor && dev_info.wiphy_freq.has_value()) {
		result += 
      std::format(", channel: {}", freq2chan(dev_info.wiphy_freq.value()).get());
	}

	return result;
}


std::string nlpp::to_string(dev_capability_t const& dev)
{
  using std::to_string;

  // get the interface types
  std::string iftypes;
  for(auto type: dev.iftypes) {
    iftypes += to_string(type);
		iftypes += ",";
  }
  iftypes.pop_back();

  // get the frequencies
  std::string freqs;
  for(auto freq: dev.freqs) {
    freqs += to_string(freq.get()) + ',';
  }
  freqs.pop_back();

  // get supported commands
  std::string cmds;
  for(auto cmd: dev.cmds) {
    cmds += to_string(cmd);
		cmds += ",";
  }
  cmds.pop_back();

  auto result = std::format("wiphy: {}, wiphy index: {}, iftypes: [{}], "
    "freqs: [{}], cmds: [{}]",
    dev.wiphy_name, 
    dev.wiphy_index.get(),
    iftypes,
    freqs,
    cmds);

  return result;
}


if_index_t phy_lookup(std::string_view phy_name)
{
  auto const path 
    = std::filesystem::path{"/sys/class/ieee80211/"} / phy_name / "index";
  if(!std::filesystem::exists(path)) {
    throw std::runtime_error{std::format("device{} does not exists", phy_name)};
  }

  std::ifstream file{path};
  if(!file.is_open()) {
    throw std::runtime_error{std::format("Unable to open {}", path.string())};
  }

  std::array<char,200> buf{};
  file.getline(buf.data(), 200, '\n');

  return if_index_t{static_cast<uint32_t>(std::atoi(buf.data()))};
}


channel_freq_t nlpp::freq2chan(frequency_t const freq) noexcept
{
	auto f = freq.get();
	int res = 0;

	if(f == 2484) {
		res = 14;
  } else if(f < 2484) {
	  res = (f - 2407) / 5;
  } else if(f < 45000) {  /* FIXME: dot11ChannelStartingFactor (802.11-2007 17.3.8.3.2) */
		res = f/5 - 1000;
  } else if(f >= 58320 && f <= 64800) {
		res = (f - 56160) / 2160;
  }
  //? else throw std::invalid_argument

	return channel_freq_t{res};
}


frequency_t nlpp::chan2freq(channel_freq_t const chan) noexcept
{
	auto c = chan.get();
	frequency_t::value_type res = (c + 1000) * 5;

	if(c < 14) {
		res = 2407 + c * 5;
  } else if(c == 14) {
		res = 2484;
  }
  //? else throw std::invalid_argument

	/* FIXME: dot11ChannelStartingFactor (802.11-2007 17.3.8.3.2) */
	return frequency_t{res};
}