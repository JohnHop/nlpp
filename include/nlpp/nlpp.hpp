#if !defined(NLUTILS_HPP)
#define NLUTILS_HPP

/**
 * @file nlpp.hpp
 * Contains common definitions used across by entities.
 */


#include "StrongType.hpp"

#include <net/if.h>
#include <linux/nl80211.h>
#include <netlink/socket.h>
#include <linux/netlink.h>

#include <string_view>
#include <string>
#include <bitset>
#include <vector>
#include <variant>
#include <optional>


namespace nlpp {


/// @brief Bitset for all possible device flags defined in `linux/if.h`.
// FIXME: non va bene. Forse deve essere un tipo a se. Troppo complicato.
using if_flags_t = StrongType<std::bitset<18>, struct if_flags_tag>;

/// @brief Strong type for a device index.
using if_index_t = StrongType<uint32_t, struct if_index_tag>;

/// @brief Strong type for a wiphy index.
using wiphy_index_t = StrongType<uint32_t, struct wiphy_index_tag>;

/// @brief Strong type for frequency channel.
using freq_chan_t = StrongType<int, struct freq_chan_tag>;

/// @brief Strong type for frequency.
using frequency_t 
  = StrongType<unsigned int, struct frequency_tag, LessComparable>;

/// @brief A device can be identified by his name or numeric ID.
using if_identifier_t = std::variant<std::string_view,if_index_t>;


/// @brief Netlink protocols used in `nlsocket_t::connect()` call.
/// @note From `<linux/netlink.h>`.
enum class netlink_protocol_e
{
  route          = NETLINK_ROUTE,
  unused         = NETLINK_UNUSED,
  user_sock      = NETLINK_USERSOCK,
  firewall       = NETLINK_FIREWALL,
  sock_diag      = NETLINK_SOCK_DIAG,
  nflog          = NETLINK_NFLOG,
  xfrm           = NETLINK_XFRM,
  selinux        = NETLINK_SELINUX,
  iscsi          = NETLINK_ISCSI,
  audit          = NETLINK_AUDIT,
  fib_lookup     = NETLINK_FIB_LOOKUP,
  connecor       = NETLINK_CONNECTOR,
  netfilter      = NETLINK_NETFILTER,
  ip6_fw         = NETLINK_IP6_FW,
  dnrtmsg        = NETLINK_DNRTMSG,
  kobject_uevent = NETLINK_KOBJECT_UEVENT,
  generic        = NETLINK_GENERIC,
  scsi_transport = NETLINK_SCSITRANSPORT,
  ecryt_fs       = NETLINK_ECRYPTFS,
  rdma           = NETLINK_RDMA,
  crypto         = NETLINK_CRYPTO,
  smc            = NETLINK_SMC
};


/// @brief Interface Operational Status.
/// @note From `<linux/if.h>`.
enum class if_operstate_e 
{
  unknow         = 1<<0, // IF_OPER_UNKNOWN
  notpresent     = 1<<1, // IF_OPER_NOTPRESENT
  down           = 1<<2, // IF_OPER_DOWN
  lowerlayerdown = 1<<3, // IF_OPER_LOWERLAYERDOWN
  testing        = 1<<4, // IF_OPER_TESTING
  dormant        = 1<<5, // IF_OPER_DORMANT
  up             = 1<<6  // IF_OPER_UP
};


/// @brief IEEE80211 interface type.
/// @note From `<linux/nl80211.h>`
enum class if_type_e 
{
  unspecified   = NL80211_IFTYPE_UNSPECIFIED, 
  adhoc         = NL80211_IFTYPE_ADHOC,
  station       = NL80211_IFTYPE_STATION, 
  ap            = NL80211_IFTYPE_AP,
  ap_vlan       = NL80211_IFTYPE_AP_VLAN, 
  wds           = NL80211_IFTYPE_WDS,
  monitor       = NL80211_IFTYPE_MONITOR, 
  mesh_point    = NL80211_IFTYPE_MESH_POINT,
  p2p_client    = NL80211_IFTYPE_P2P_CLIENT,
  p2p_go        = NL80211_IFTYPE_P2P_GO,
  p2p_device    = NL80211_IFTYPE_P2P_DEVICE,
  ocb           = NL80211_IFTYPE_OCB,
  nan           = NL80211_IFTYPE_NAN
};


/// @brief Interface Flags.
/// @note From `<net/if.h>`
enum class if_flag_e 
{
  up           = IFF_UP,          // 1<<0
  broadcast    = IFF_BROADCAST,   // 1<<1
  debug        = IFF_DEBUG,       // 1<<2
  loopback     = IFF_LOOPBACK,    // 1<<3
  point_to_point = IFF_POINTOPOINT, // 1<<4
  no_trailers  = IFF_NOTRAILERS,  // 1<<5
  running      = IFF_RUNNING,     // 1<<6
  no_arp       = IFF_NOARP,       // 1<<7
  promisc      = IFF_PROMISC,     // 1<<8
  all_multi    = IFF_ALLMULTI,    // 1<<9
  master       = IFF_MASTER,      // 1<<10
  slave        = IFF_SLAVE,       // 1<<11
  multicast    = IFF_MULTICAST,   // 1<<12
  port_sel     = IFF_PORTSEL,     // 1<<13
  automedia    = IFF_AUTOMEDIA,   // 1<<14
  dynamic      = IFF_DYNAMIC,     // 1<<15
  lower_up     = 1<<16,           // IFF_LOWER_UP
  dormant      = 1<<17,           // IFF_DORMANT
  echo         = 1<<18            // IFF_ECHO
};


/// @brief Supported nl80211 commands.
/// @note From `<linux/nl80211.h>`, line 347.
enum class nl80211_command_e
{
  unspec                        = NL80211_CMD_UNSPEC,
  get_wiphy                     = NL80211_CMD_GET_WIPHY,
  set_wiphy                     = NL80211_CMD_SET_WIPHY,
  new_wiphy                     = NL80211_CMD_NEW_WIPHY,
  del_wiphy                     = NL80211_CMD_DEL_WIPHY,
  get_interface                 = NL80211_CMD_GET_INTERFACE,
  set_interface                 = NL80211_CMD_SET_INTERFACE,
  new_interface                 = NL80211_CMD_NEW_INTERFACE,
  del_interface                 = NL80211_CMD_DEL_INTERFACE,
  get_key                       = NL80211_CMD_GET_KEY,
  set_key                       = NL80211_CMD_SET_KEY,
  new_key                       = NL80211_CMD_NEW_KEY,
  del_key                       = NL80211_CMD_DEL_KEY,
  get_beacon                    = NL80211_CMD_GET_BEACON,
  set_beacon                    = NL80211_CMD_SET_BEACON,
  start_ap                      = NL80211_CMD_START_AP,
  new_beacon                    = NL80211_CMD_NEW_BEACON,
  stop_ap                       = NL80211_CMD_STOP_AP,
  del_beacon                    = NL80211_CMD_DEL_BEACON,
  get_station                   = NL80211_CMD_GET_STATION,
  set_station                   = NL80211_CMD_SET_STATION,
  new_station                   = NL80211_CMD_NEW_STATION,
  del_station                   = NL80211_CMD_DEL_STATION,
  get_mpath                     = NL80211_CMD_GET_MPATH,
  set_mpath                     = NL80211_CMD_SET_MPATH,
  new_mpath                     = NL80211_CMD_NEW_MPATH,
  del_mpath                     = NL80211_CMD_DEL_MPATH,
  set_bss                       = NL80211_CMD_SET_BSS,
  set_reg                       = NL80211_CMD_SET_REG,
  req_set_reg                   = NL80211_CMD_REQ_SET_REG,
  get_mesh_config               = NL80211_CMD_GET_MESH_CONFIG,
  set_mesh_config               = NL80211_CMD_SET_MESH_CONFIG,
  set_mgmt_extra_ie             = NL80211_CMD_SET_MGMT_EXTRA_IE,
  get_reg                       = NL80211_CMD_GET_REG,
  get_scan                      = NL80211_CMD_GET_SCAN,
  trigger_scan                  = NL80211_CMD_TRIGGER_SCAN,
  new_scan_results              = NL80211_CMD_NEW_SCAN_RESULTS,
  scan_aborted                  = NL80211_CMD_SCAN_ABORTED,
  reg_change                    = NL80211_CMD_REG_CHANGE,
  authenticate                  = NL80211_CMD_AUTHENTICATE,
  associate                     = NL80211_CMD_ASSOCIATE,
  deauthenticate                = NL80211_CMD_DEAUTHENTICATE,
  disassociate                  = NL80211_CMD_DISASSOCIATE,
  michael_mic_failure           = NL80211_CMD_MICHAEL_MIC_FAILURE,
  reg_beacon_hint               = NL80211_CMD_REG_BEACON_HINT,
  join_ibss                     = NL80211_CMD_JOIN_IBSS,
  leave_ibss                    = NL80211_CMD_LEAVE_IBSS,
  testmode                      = NL80211_CMD_TESTMODE,
  connect                       = NL80211_CMD_CONNECT,
  roam                          = NL80211_CMD_ROAM,
  disconnect                    = NL80211_CMD_DISCONNECT,
  set_wiphy_netns               = NL80211_CMD_SET_WIPHY_NETNS,
  get_survey                    = NL80211_CMD_GET_SURVEY,
  new_survey_results            = NL80211_CMD_NEW_SURVEY_RESULTS,
  set_pmksa                     = NL80211_CMD_SET_PMKSA,
  del_pmksa                     = NL80211_CMD_DEL_PMKSA,
  flush_pmksa                   = NL80211_CMD_FLUSH_PMKSA,
  remain_on_channel             = NL80211_CMD_REMAIN_ON_CHANNEL,
  cancel_remain_on_channel      = NL80211_CMD_CANCEL_REMAIN_ON_CHANNEL,
  set_tx_bitrate_mask           = NL80211_CMD_SET_TX_BITRATE_MASK,
  register_frame                = NL80211_CMD_REGISTER_FRAME,
  register_action               = NL80211_CMD_REGISTER_ACTION,
  frame                         = NL80211_CMD_FRAME,
  action                        = NL80211_CMD_ACTION,
  frame_tx_status               = NL80211_CMD_FRAME_TX_STATUS,
  action_tx_status              = NL80211_CMD_ACTION_TX_STATUS,
  set_power_save                = NL80211_CMD_SET_POWER_SAVE,
  get_power_save                = NL80211_CMD_GET_POWER_SAVE,
  set_cqm                       = NL80211_CMD_SET_CQM,
  notify_cqm                    = NL80211_CMD_NOTIFY_CQM,
  set_channel                   = NL80211_CMD_SET_CHANNEL,
  set_wds_peer                  = NL80211_CMD_SET_WDS_PEER,
  frame_wait_cancel             = NL80211_CMD_FRAME_WAIT_CANCEL,
  join_mesh                     = NL80211_CMD_JOIN_MESH,
  leave_mesh                    = NL80211_CMD_LEAVE_MESH,
  unprot_deauthenticate         = NL80211_CMD_UNPROT_DEAUTHENTICATE,
  unprot_disassociate           = NL80211_CMD_UNPROT_DISASSOCIATE,
  new_peer_candidate            = NL80211_CMD_NEW_PEER_CANDIDATE,
  get_wowlan                    = NL80211_CMD_GET_WOWLAN,
  set_wowlan                    = NL80211_CMD_SET_WOWLAN,
  start_sched_scan              = NL80211_CMD_START_SCHED_SCAN,
  stop_sched_scan               = NL80211_CMD_STOP_SCHED_SCAN,
  sched_scan_results            = NL80211_CMD_SCHED_SCAN_RESULTS,
  sched_scan_stopped            = NL80211_CMD_SCHED_SCAN_STOPPED,
  set_rekey_offload             = NL80211_CMD_SET_REKEY_OFFLOAD,
  pmksa_candidate               = NL80211_CMD_PMKSA_CANDIDATE,
  tdls_oper                     = NL80211_CMD_TDLS_OPER,
  tdls_mgmt                     = NL80211_CMD_TDLS_MGMT,
  unexpected_frame              = NL80211_CMD_UNEXPECTED_FRAME,
  probe_client                  = NL80211_CMD_PROBE_CLIENT,
  register_beacons              = NL80211_CMD_REGISTER_BEACONS,
  unexpected_4addr_frame        = NL80211_CMD_UNEXPECTED_4ADDR_FRAME,
  set_noack_map                 = NL80211_CMD_SET_NOACK_MAP,
  ch_switch_notify              = NL80211_CMD_CH_SWITCH_NOTIFY,
  start_p2p_device              = NL80211_CMD_START_P2P_DEVICE,
  stop_p2p_device               = NL80211_CMD_STOP_P2P_DEVICE,
  conn_failed                   = NL80211_CMD_CONN_FAILED,
  set_mcast_rate                = NL80211_CMD_SET_MCAST_RATE,
  set_mac_acl                   = NL80211_CMD_SET_MAC_ACL,
  radar_detect                  = NL80211_CMD_RADAR_DETECT,
  get_protocol_features         = NL80211_CMD_GET_PROTOCOL_FEATURES,
  update_ft_ies                 = NL80211_CMD_UPDATE_FT_IES,
  ft_event                      = NL80211_CMD_FT_EVENT,
  crit_protocol_start           = NL80211_CMD_CRIT_PROTOCOL_START,
  crit_protocol_stop            = NL80211_CMD_CRIT_PROTOCOL_STOP,
  get_coalesce                  = NL80211_CMD_GET_COALESCE,
  set_coalesce                  = NL80211_CMD_SET_COALESCE,
  channel_switch                = NL80211_CMD_CHANNEL_SWITCH,
  vendor                        = NL80211_CMD_VENDOR,
  set_qos_map                   = NL80211_CMD_SET_QOS_MAP,
  add_tx_ts                     = NL80211_CMD_ADD_TX_TS,
  del_tx_ts                     = NL80211_CMD_DEL_TX_TS,
  get_mpp                       = NL80211_CMD_GET_MPP,
  join_ocb                      = NL80211_CMD_JOIN_OCB,
  leave_ocb                     = NL80211_CMD_LEAVE_OCB,
  ch_switch_started_notify      = NL80211_CMD_CH_SWITCH_STARTED_NOTIFY,
  tdls_channel_switch           = NL80211_CMD_TDLS_CHANNEL_SWITCH,
  tdls_cancel_channel_switch    = NL80211_CMD_TDLS_CANCEL_CHANNEL_SWITCH,
  wiphy_reg_change              = NL80211_CMD_WIPHY_REG_CHANGE,
  abort_scan                    = NL80211_CMD_ABORT_SCAN,
  start_nan                     = NL80211_CMD_START_NAN,
  stop_nan                      = NL80211_CMD_STOP_NAN,
  add_nan_function              = NL80211_CMD_ADD_NAN_FUNCTION,
  del_nan_function              = NL80211_CMD_DEL_NAN_FUNCTION,
  change_nan_config             = NL80211_CMD_CHANGE_NAN_CONFIG,
  nan_match                     = NL80211_CMD_NAN_MATCH,
  set_multicast_to_unicast      = NL80211_CMD_SET_MULTICAST_TO_UNICAST,
  update_connect_params         = NL80211_CMD_UPDATE_CONNECT_PARAMS,
  set_pmk                       = NL80211_CMD_SET_PMK,
  del_pmk                       = NL80211_CMD_DEL_PMK,
  port_authorized               = NL80211_CMD_PORT_AUTHORIZED,
  reload_regdb                  = NL80211_CMD_RELOAD_REGDB,
  external_auth                 = NL80211_CMD_EXTERNAL_AUTH,
  sta_opmode_changed            = NL80211_CMD_STA_OPMODE_CHANGED,
  control_port_frame            = NL80211_CMD_CONTROL_PORT_FRAME,
  get_ftm_responder_stats       = NL80211_CMD_GET_FTM_RESPONDER_STATS,
  peer_measurement_start        = NL80211_CMD_PEER_MEASUREMENT_START,
  peer_measurement_result       = NL80211_CMD_PEER_MEASUREMENT_RESULT,
  peer_measurement_complete     = NL80211_CMD_PEER_MEASUREMENT_COMPLETE,
  notify_radar                  = NL80211_CMD_NOTIFY_RADAR,
  update_owe_info               = NL80211_CMD_UPDATE_OWE_INFO,
  probe_mesh_link               = NL80211_CMD_PROBE_MESH_LINK,
  set_tid_config                = NL80211_CMD_SET_TID_CONFIG,
  unprot_beacon                 = NL80211_CMD_UNPROT_BEACON,
  control_port_frame_tx_status  = NL80211_CMD_CONTROL_PORT_FRAME_TX_STATUS,
  set_sar_specs                 = NL80211_CMD_SET_SAR_SPECS,
  obss_color_collision          = NL80211_CMD_OBSS_COLOR_COLLISION,
  color_change_request          = NL80211_CMD_COLOR_CHANGE_REQUEST,
  color_change_started          = NL80211_CMD_COLOR_CHANGE_STARTED,
  color_change_aborted          = NL80211_CMD_COLOR_CHANGE_ABORTED,
  color_change_completed        = NL80211_CMD_COLOR_CHANGE_COMPLETED,
  set_fils_aad                  = NL80211_CMD_SET_FILS_AAD,
  assoc_comeback                = NL80211_CMD_ASSOC_COMEBACK,
  add_link                      = NL80211_CMD_ADD_LINK,
  remove_link                   = NL80211_CMD_REMOVE_LINK,
  add_link_sta                  = NL80211_CMD_ADD_LINK_STA,
  modify_link_sta               = NL80211_CMD_MODIFY_LINK_STA,
  remove_link_sta               = NL80211_CMD_REMOVE_LINK_STA,
  set_hw_timestamp              = NL80211_CMD_SET_HW_TIMESTAMP,
  links_removed                 = NL80211_CMD_LINKS_REMOVED,
  set_tid_to_link_mapping       = NL80211_CMD_SET_TID_TO_LINK_MAPPING,
};


/// @brief Helper struct containing some device info.
/// @note Obtained with a NetlinkGeneric::get_interface() call.
struct dev_info_t
{
  if_index_t                  if_index;      ///< NL80211_ATTR_IFINDEX
  std::string                 if_name;       ///< NL80211_ATTR_IFNAME
  unsigned long long          wdev;          ///< NL80211_ATTR_WDEV
  std::string                 mac_address;   ///< NL80211_ATTR_MAC
  std::string                 ssid;          ///< NL80211_ATTR_SSID
  if_type_e                   type;          ///< NL80211_ATTR_IFTYPE
  if_index_t                  wiphy_index;   ///< NL80211_ATTR_WIPHY
  std::optional<frequency_t>  wiphy_freq;    ///< NL80211_ATTR_WIPHY_FREQ
  int                         channel_width; ///< NL80211_ATTR_CHANNEL_WIDTH

  /// @brief Compares two `dev_info_t`.
  /// @param[in] lhs Left operand to compare.
  /// @param[in] rhs Right operand to compare.
  /// @returns True only if `lhs` and `rhs` are the same device.
  [[nodiscard]] friend 
  bool operator==(dev_info_t const& lhs, dev_info_t const& rhs) noexcept
  {
    return lhs.if_index == rhs.if_index || lhs.if_name == rhs.if_name;
  }
};


/// @brief Helper struct containing the device capabilities.
struct dev_capability_t
{
  wiphy_index_t wiphy_index;            ///< From `NL80211_ATTR_WIPHY`
  std::string wiphy_name;               ///< From `NL80211_ATTR_WIPHY_NAME`
  std::vector<if_type_e> iftypes;       ///< From `NL80211_ATTR_SUPPORTED_IFTYPES`
  std::vector<frequency_t> freqs;       ///< From `NL80211_FREQUENCY_ATTR_FREQ`
  std::vector<nl80211_command_e> cmds;  ///< From `NL80211_ATTR_SUPPORTED_COMMANDS`

  /// @brief Checks if a interface type is supported by this device.
  /// @param[in] mode Interface type mode to check.
  /// @returns true if `mode` is supported, otherwise false.
  [[nodiscard]] bool is_supported(if_type_e mode) const noexcept;
  
  /// @brief Checks if a specific frequency is supported by this device.
  /// @param[in] freq Frequency to check.
  /// @returns true if `freq` is supported, otherwise false.
  [[nodiscard]] bool is_supported(frequency_t freq) const noexcept;
  
  /// @brief Checks if a frequency channel is suppoted by this device.
  /// @param[in] chan Frequency channel to check.
  /// @returns true if `chan` is supported, otherwise false.
  [[nodiscard]] bool is_supported(freq_chan_t chan) const noexcept;
  
  /// @brief Check is a nl80211 command is supported by this device.
  /// @param[in] cmd A nl80211 command to check.
  /// @returns true if `cmd` is supported, otherwise false.
  [[nodiscard]] bool is_supported(nl80211_command_e cmd) const noexcept;

  /// @brief Compares two `dev_capability_t`.
  /// @param[in] lhs Left capability operand.
  /// @param[in] rhs Right capability operand.
  /// @returns true if the wiphy index or wiphy name are equals.
  [[nodiscard]] friend bool
  operator==(dev_capability_t const& lhs, dev_capability_t const& rhs) noexcept
  {
    return lhs.wiphy_index == rhs.wiphy_index 
      || lhs.wiphy_name == rhs.wiphy_name;
  }
};


// Helper functions -----------------------------------------------------------/

/// @brief Translation from operational status code to std::string.
/// @param[in] operstate An operational state.
/// @returns The operational status name.
[[nodiscard]] std::string to_string(if_operstate_e operstate);

/// @brief Translation from interface type to std::string.
/// @param[in] type The interface type.
/// @returns The interface type name.
[[nodiscard]] std::string_view to_string(if_type_e type);

/// @brief Translation from interface flag to std::string.
/// @param[in] flag An interface flag.
/// @returns The interface flag name.
[[nodiscard]] std::string_view to_string(if_flag_e flag);

/// @brief Translation from interface flags to comma separated std::string.
/// @param[in] flags An interface flag bitset.
/// @returns The comma separated interface flag names.
[[nodiscard]] std::string to_string(if_flags_t flags);

/// @brief Translation from `enum nl80211_commands` to string;
/// @param[in] cmd An interface command.
/// @returns The interface command name.
[[nodiscard]] std::string_view to_string(nl80211_command_e cmd);

/// @brief Translation from a nlpp device info type to string.
/// @param[in] info A device info.
/// @returns The description of the device info.
[[nodiscard]] std::string to_string(dev_info_t const& info);

/// @brief Translation from a nlpp device capability info to string.
/// @param[in] capability A device capability.
/// @returns The description of the device capability.
[[nodiscard]] std::string to_string(dev_capability_t const& capability);


/// @brief Returns the wiphy index from his name.
/// @param[in] phy_name Physical device name.
/// @returns The wiphy index associated to a name.
/// @throws `std::runtime_error` When device does not exists.
/// @note From `phy_lookup()` from `iw.c` source code.
[[nodiscard]] if_index_t phy_lookup(std::string_view phy_name);

// Utilities ------------------------------------------------------------------/

/// @brief Convert a frequency to channel index.
/// @param[in] freq A wireless frequency.
/// @returns The frequency associated with a channel.
/// @note From iw source code.
[[nodiscard]] freq_chan_t freq2chan(frequency_t freq) noexcept;

/// @brief Returns the frequency equivalents to a channel.
/// @param[in] chan A frequency channel.
/// @returns The wireless frequency associated with a frequency channel.
/// @note From `iw` source code.
[[nodiscard]] frequency_t chan2freq(freq_chan_t chan) noexcept;


};  // end namespace nlpp


#endif // NLUTILS_HPP
