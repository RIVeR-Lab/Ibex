#!/bin/bash

# ***************************************************************************************
#
#     Copyright (c) 2017, Pleora Technologies Inc., All rights reserved.
#
# ***************************************************************************************

DISTRIBUTION="unknown"

if [ -f /etc/os-release ]; then
    DISTRIBUTION=`grep -w NAME /etc/os-release | sed 's/[^"]*"\([^"]*\)"/\1/'`
fi

if [[ "$DISTRIBUTION" != "Ubuntu"* ]] && 
   [[ "$DISTRIBUTION" != "Debian"* ]] &&
   [[ "$DISTRIBUTION" != "CentOS"* ]] &&
   [[ "$DISTRIBUTION" != "Red Hat"* ]]; then
    echo "Unsupported distribution. Exit now."
    exit 1
fi

if [ $(id -u) -ne 0 ]; then
    echo "User has insufficient privileges. Please try sudo command."
    exit 2
fi

echo "Setting usbfs memory size to 4000"
sh -c 'echo 4000 > /sys/module/usbcore/parameters/usbfs_memory_mb'

