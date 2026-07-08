#!/bin/bash

################################################################################
#
# build.sh
# Pleora Technologies Inc. Copyright (c) 2002-2019
# for ebUniversalProForEthernet-x86_64.ko 7.1.1 build 5002
#
################################################################################

# Variables
CHMOD_BIN=chmod
CP_BIN=cp
ECHO_BIN=echo
MAKE_BIN=make
MKDIR_BIN=mkdir
MV_BIN=mv
POP_BIN=popd
PUSHD_BIN=pushd

HOST_VERSION=`uname -r`
HOST_ARCH=`uname -m`
OUT_FOLDER=$PWD
SRC_FOLDER=$PWD/ebUniversalProForEthernet
KERNEL_FOLDER=
PREFIX=
PATH_TOOLS=

#Display the help for this script
DisplayHelp()
{
    $ECHO_BIN ""
    $ECHO_BIN "NAME"
    $ECHO_BIN "    build.sh - Build the eBUS Universal Pro For Ethernet driver module based"
    $ECHO_BIN "               based on existing transport layer library"
    $ECHO_BIN "               ( libebTransportLayer-x86_64 )"
    $ECHO_BIN ""
    $ECHO_BIN "SYNOPSIS"
    $ECHO_BIN "    bash build.sh [--output=OUTPUT] [--kernel=KERNEL [--cross_compile=PREFIX ] ] "
    $ECHO_BIN "                    [--help]" 
    $ECHO_BIN ""
    $ECHO_BIN "DESCRIPTION"
    $ECHO_BIN "    Build the kernel module by linking the localy builded code"
    $ECHO_BIN "    to an existing transport layer library"
    $ECHO_BIN "    --src=SRC_FOLDER   is the location where the source code is"
    $ECHO_BIN "                       locate"
    $ECHO_BIN "    --output=OUTPUT    is the location where the module folder"
    $ECHO_BIN "                       will be create by the script. This folder"
    $ECHO_BIN "                       will contains all theeverything needed for"
    $ECHO_BIN "                       the distribution of the module."
    $ECHO_BIN "    --kernel=KERNEL    point to the kernel header files. This"
    $ECHO_BIN "                       is only optional argument required when"
    $ECHO_BIN "                       compiling/cross-compiling for another kernel that"
    $ECHO_BIN "                       is not the one actually in use by your computer"
    $ECHO_BIN "                       i.e. /lib/modules/2.6.18-92.el5/build"
    $ECHO_BIN "    --cross_compile=PATH/PREFIX is a variable containing the full path to the prefix "
    $ECHO_BIN "                       compiler tool as the CROSS_COMPILE variable used by the"
    $ECHO_BIN "                       linux kernel Makefiles"    
    $ECHO_BIN "                       i.e. [some path on your hd]/bin/powerpc-e500v2-linux-gnuspe-"
    $ECHO_BIN "    --help             Display this help"
    $ECHO_BIN ""
    $ECHO_BIN "COPYRIGHT"
    $ECHO_BIN "    Pleora Technologies Inc. Copyright (c) 2002-2019"
    $ECHO_BIN ""
    $ECHO_BIN "VERSION"
    $ECHO_BIN "    7.1.1 build 5002"
    $ECHO_BIN ""
}

#Print out the error and exit 
# $1 Error message
# $2 Exit code
ErrorOut()
{
	$ECHO_BIN "Error: $1"
	exit $2
}

# Parse the input arguments
for i in $*
do
    case $i in
        --kernel=*)
            KERNEL_FOLDER=${i#*=} 
            ;;    	
        --src=*)
            SRC_FOLDER=${i#*=} 
            ;;  
        --output=*)
            OUT_FOLDER=${i#*=} 
            ;;    	
        --cross_compile=*)
            TOOLS=${i#*=} 
            ;;
        --help)
            DisplayHelp
            exit 0
        ;;
        *)
        # unknown option
        DisplayHelp
        exit 1
        ;;
    esac
done

# Display banner
$ECHO_BIN "*********************************************"
$ECHO_BIN "  Build ebUniversalProForEthernet-x86_64.ko"
$ECHO_BIN "*********************************************"
$ECHO_BIN ""

IFS=- read KVER PATCH ARCH <<< $HOST_VERSION
if [ "$KVER" = "4.4.0" -a "$PATCH" -ge 143 ] ; then
   export UBUNTU_PATCH=-DUBUNTU_PATCH
fi

# When cross-compiling, we need to ensure we have the proper extra configuration...
if [ -z "$TOOLS" ]; then
    # When no kernel source is provided, we assume the default location
    if [ -z "$KERNEL_FOLDER" ]; then
        KERNEL_FOLDER="/lib/modules/$HOST_VERSION/build"
    fi 
else
    if [ -z "$KERNEL_FOLDER" ]; then
        ErrorOut "*** The parameter --kernel is required when cross-compiling ***" 1
    fi 
fi

# Ensure the kernel header files are availables
if [ ! -d "$KERNEL_FOLDER" ]; then
    ErrorOut "*** Cannot find the files to build kernel module in this PC ***" 1
fi

# Convert from ppc to powerpc for the kernel build process
KERNEL_TARGET_ARCH=`echo "x86_64" | sed -e s/ppc.*/powerpc/ -e s/arm.*/arm/`
case "$TOOLS" in
    *"aarch64"*) KERNEL_TARGET_ARCH="arm64" ;;
    *) ;;
esac

# Printout some information...
$ECHO_BIN "Using output folder $OUT_FOLDER"
$ECHO_BIN "Using kernel from folder $KERNEL_FOLDER"
if [ ! -z "$PREFIX" ]; then
    $ECHO_BIN "Using tools with prefix $PREFIX"
fi
if [ ! -z "$PATH_TOOLS" ]; then
    $ECHO_BIN "Using bin folder $PATH_TOOLS"
fi

# Now build the module and link the ebTransportLayer
$ECHO_BIN "Invoking makefile to build the ebUniversalProForEthernet-x86_64.ko module"

if [ -z "$TOOLS" ]; then
    $MAKE_BIN -C"$SRC_FOLDER" "KERNEL_FOLDER=$KERNEL_FOLDER" "all"
else
    $MAKE_BIN -C"$SRC_FOLDER" "KERNEL_FOLDER=$KERNEL_FOLDER" "ARCH=$KERNEL_TARGET_ARCH" "CROSS_COMPILE=$TOOLS" "all"
fi

if [ ! -f "$SRC_FOLDER/ebUniversalProForEthernet-x86_64.ko" ]; then
	ErrorOut "*** Fail to create the module ebUniversalProForEthernet-x86_64.ko ***" 1
fi

if [ "$SRC_FOLDER"!="$OUT_FOLDER" ]; then
    # Create the output directory
    $ECHO_BIN "Create output directory $OUT_FOLDER"
    $MKDIR_BIN "-p" "$OUT_FOLDER"

    # Populate the output folder 
    $ECHO_BIN "Moving the output files to $OUT_FOLDER for distribution"
    $MV_BIN "$SRC_FOLDER/ebUniversalProForEthernet-x86_64.ko" "$OUT_FOLDER"
    $CHMOD_BIN a+x "$OUT_FOLDER/ebUniversalProForEthernet-x86_64.ko"
fi

# Cleanup the module builder folder to be ready for distribution
if [ -z "$TOOLS" ]; then
    $MAKE_BIN -C"$SRC_FOLDER" "KERNEL_FOLDER=$KERNEL_FOLDER" "clean"
else
    $MAKE_BIN -C"$SRC_FOLDER" "KERNEL_FOLDER=$KERNEL_FOLDER" "ARCH=$KERNEL_TARGET_ARCH" "CROSS_COMPILE=$TOOLS" "clean"
fi

$ECHO_BIN ""
$ECHO_BIN "The module ebUniversalProForEthernet-x86_64.ko is now available."
$ECHO_BIN "Do not forget to rebuild the module after each kernel update!"
$ECHO_BIN ""
