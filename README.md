# An EGT based GUI application that displays network stats

A simple [egt](http://github.com/linux4sam/egt) gui application that is purely C++ based.

***Note: EGT is primarily targeted at Microchip SAM ARM devices***

## To Build
You'll need egt installed on your machines(s).
To cross-compile you'll need the Yocto/Poky SDK or another method.
To build both x86 & ARM binaries run the included build.sh script.

***Note: The script invokes the [Yocto/Poky SDK env](http://docs.yoctoproject.org/sdk-manual/working-projects.html#makefile-based-projects), you'll need to modify the script to support yours***

![Screenshot](/screenshot/image1.png)