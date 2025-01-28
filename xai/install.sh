#1. **Ensure System Compatibility**
#    - Ensure your system has the necessary 32-bit libraries and development tools installed (e.g., `gcc-multilib` for Linux).
#    - Check that the toolchain you use supports both 32-bit and 64-bit builds.

sudo dpkg --add-architecture i386

sudo apt-get install libgl1:i386
#or
sudo apt-get install libgl1-mesa-glx:i386


sudo apt-get install libx11:i386
#or
sudo apt-get install libx11-dev:i386


#2. **Compile C/C++ Code for 32-bit Architecture**
#Use `-m32` with your C/C++ compiler to force a 32-bit build:

# g++ -m32 -o GameBoard.o -c GameBoard.cpp

#3. **Set Go Environment to Use CGO**
#Go’s `cgo` feature is used to integrate C/C++ into Go code. You need to set `CGO_ENABLED` and provide the correct `CFLAGS` and `LDFLAGS` for 32-bit compilation.
#Set environment variables for compiling in mixed modes:

export CGO_ENABLED=1
export CGO_CFLAGS="-m32"
export CGO_LDFLAGS="-m32"

