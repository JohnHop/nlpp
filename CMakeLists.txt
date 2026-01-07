cmake_minimum_required(VERSION 3.28)
project(nlpp LANGUAGES CXX)

# external dependencies
message(CHECK_START "Looking for PkgConfig")
find_package(PkgConfig REQUIRED) 
message(CHECK_PASS "found")

message(CHECK_START "Looking for libnl")
pkg_check_modules(LIBNL3 REQUIRED 
  libnl-3.0 
  libnl-route-3.0 
  libnl-genl-3.0
)
message(CHECK_PASS "found")


add_library(nlpp
  src/NetlinkGeneric.cpp
  src/nlcache_t.cpp
  src/nlmsg_t.cpp
  src/nlpp.cpp
  src/NetlinkRoute.cpp
  src/nlcb_t.cpp
  src/nlsocket_t.cpp
  src/rtnl_link_t.cpp
)
target_include_directories(nlpp
  PRIVATE include/nlpp/
  PUBLIC  include/
          /usr/include/libnl3/
)
target_link_libraries(nlpp
  PRIVATE nl-3 
          nl-route-3
          nl-genl-3
)
target_compile_features(nlpp PUBLIC cxx_std_23)


add_subdirectory(examples/) # compile examples
add_subdirectory(tests/)  # compile tests
