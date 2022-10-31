# An EGT based GUI application that displays network stats

A simple [egt](http://github.com/linux4sam/egt) gui application to display network settings. C++ based.

***Note: EGT is primarily targeted at Microchip SAM ARM devices***

## To Build
You'll need egt installed on your machines(s).

To cross-compile:
    You'll need the Yocto/Poky SDK or another method on your host
    Call invoke the [Yocto/Poky SDK script](http://docs.yoctoproject.org/sdk-manual/working-projects.html#makefile-based-projects), something like:
        ./opt/poky-atmel/3.1.15/environment-setup-cortexa5t2hf-neon-vfpv4-poky-linux-gnueabi
    Run make:
        make
To build x86 binary
    Run make:
        make egtNetwork_x86

![Screenshot](/screenshot/image1.png)