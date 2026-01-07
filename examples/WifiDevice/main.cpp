#include "WifiDevice.hpp"

#include <cstdlib>
#include <print>


int main(int argc, char* argv[])
{
  std::println("Searching and setting up a device with name wlx7cf17e67ae25");
  
  WifiDevice device{"wlx7cf17e67ae25"};

  std::println("{}", device.to_string()); // print some info

  std::println("Put the device down");
    device.put_down();
  
  std::println("Put the device up");
  device.put_up();

  std::println("{}", device.to_string()); // print some info

  return EXIT_SUCCESS;
}