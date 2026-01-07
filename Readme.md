# nlpp library
This library is a modern C++ wrapper of the well-known [Netlink Protocol Library Suite (libnl)](https://www.infradead.org/~tgr/libnl/) written in C. Made to learn. Made for fun.

The library aims to simplify the access to the Linux kernel interfaces available via the Netlink protocol using a more modern approach.

## Library Overview

![alt text](doc/classes.png "Classes Diagram")

Low Level Classes wrap some fundamental libnl data types, like sockets, messages, attributes and callbacks.

High Level Classes provide more generic functionality, like obtain device info/capabilities, put link up/down, set device mode, and more.

### Netlink Generic

`NetlinkGeneric` methods are mapped to some `iw` commands:

- `NetlinkGeneric::get_interface()` → `iw dev <devname> info`: get information about an interface
- `NetlinkGeneric::get_list_interfaces()` → `iw dev`: get information about all interfaces
- `NetlinkGeneric::get_phy()` → `iw phy <phyname> info`: get capabilities about a device
- `NetlinkGeneric::set_if_type()` → `iw dev <devname> set type <type>`: set device type
- `NetlinkGeneric::set_if_channel()` → `iw dev <devname> set channel <channel>`: set device frequency

### Netlink Route

`NetlinkRoute` methods are mapped to some `ip link` commands:

- 