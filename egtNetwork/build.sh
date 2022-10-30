#! /bin/sh

RED='\033[0;31m'
NC='\033[0m' # No Color

WARN_CFLAGS="-Wall -Wextra -pedantic -Wfloat-equal -Wsign-compare -Wpointer-arith -Wwrite-strings -Wmissing-declarations -Wpacked -Wstrict-aliasing=2 -Winit-self -Wno-attributes -Wno-long-long -Wno-missing-field-initializers -Wno-unused-parameter -Wno-psabi"

# Script to build source code to executable
if g++ -std=c++14 egtNetwork.cpp -o egtNetwork_x86 -O $WARN_CFLAGS `pkg-config libegt --cflags --libs` -lgpiod -pthread -lsqlite3; then
    echo "x86 build success"
else
    echo -e "x86 build ${RED}FAILURE${NC}, exiting"
    exit 1
fi

# Set environment using Yocto/Poky SDK
# YOURS IS PROBABLY DIFFERENT!
. /opt/poky-atmel/3.1.15/environment-setup-cortexa5t2hf-neon-vfpv4-poky-linux-gnueabi

# Build for ARM
if $CXX $INC_DIR egtNetwork.cpp -o egtNetwork_ARM -O $WARN_CFLAGS `pkg-config libegt --cflags --libs` -lgpiod -pthread -lsqlite3; then
    echo "ARM build success"
    # use -r to recursively copy files
    # Uncomment below & update to copy to board
    # scp egtNetwork_ARM root@192.168.178.10:/usr/share/
else
    echo -e "ARM build ${RED}FAILURE${NC}"
fi
