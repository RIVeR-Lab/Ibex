This document describes how to compile and run the example project for the HSI Mosaic API.

# Linux

* Make sure you have CMake installed.

  sudo apt update
  sudo apt install cmake

* Make sure you have the build tools installed (gcc, g++, make, ...).

  sudo apt update
  sudo apt install build-essential

* Make sure you have the Ximea and eBus drivers installed.

* In the build folder, run the create_build_files.sh script. This will create a Release and Debug build folder.
* In the Release/Debug folder, run make. This will create the executable in a bin folder.
* Call the run_example.sh script to run the example. The scripts adds the dependent library path to the LD_LIBRARY_PATH variable.

# Windows

* Download and install CMake.

  Website: https://cmake.org/
  
  Make sure to enable the checkbox for adding CMake to your path.
  
* Create the build files for your environment.

  The general steps are as follows:
  
  - Create a build folder
  - Move into the build folder
  - Run CMake with the correct generator and architecture
    
    cmake .. -G [generator] <-A [architecture]>
    
    Consult the CMake documentation for the correct parameters for your environment.
  
  For the following settings, pre-made Powershell script are available which will do all of the above steps.
  
  VS2019_x64.ps1 -> Visual Studio 2019 x64
  
*