# 27th Oct 22
# Makefile for egtNetwork

# If you wish to cross compile using Yocto SKD invoke source env first, eg:
# . /opt/poky-atmel/3.1.15/environment-setup-cortexa5t2hf-neon-vfpv4-poky-linux-gnueabi

# Using Yocto/Poky SDK
CXX_ARM=arm-poky-linux-gnueabi-g++ -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a5 -fstack-protector-strong -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=/opt/poky-atmel/3.1.15/sysroots/cortexa5t2hf-neon-vfpv4-poky-linux-gnueabi

# Host
CXX_x86=g++ -fstack-protector-strong -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security

# Compiler flags
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS= -O `pkg-config libegt --cflags --libs` -pthread

# Default is to produce ARM binary
TARGET = egtNetwork
TARGET_x86 = egtNetwork_x86

$(TARGET): egtNetwork.cpp
	@echo Making ARM binary
	@echo "If you get an error check if you've invoked your Yocto/Poky SDK script"
	${CXX_ARM} egtNetwork.cpp -o $(TARGET) $(CFLAGS)

$(TARGET_x86): egtNetwork.cpp
	@echo Making x86 binary
	${CXX_x86} egtNetwork.cpp -o $(TARGET_x86) $(CFLAGS)

.PHONY: clean

clean:
	rm -rf *.o
	rm $(TARGET_x86) $(TARGET)
	@echo Cleaning done
