#!/bin/sh

#******************************************************************************
# @attention
#
#<h2><center>&copy; COPYRIGHT(c) 2021 Photonfocus AG</center></h2>
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
# 3. Neither the name of Photonfocus nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#*****************************************************************************

#Check root access
is_root=`id -u`
if [ $is_root -ne 0 ]; then
	echo "Kernel module installation requires root access."
	echo "Please run the script as the root user or use sudo"
	exit 1
fi

if ! [ -x "$(command -v dkms)" ]; then
    echo "dkms could not be found. Please install dkms first."
    echo "e.g. 'sudo apt -y install dkms'" 
    exit
fi

SELF_DIR=`dirname $0`
DKMS_ROOT_DIR=/usr/src
DKMS_FILE=${SELF_DIR}/dkms.conf
SRC_DIR=${SELF_DIR}/src
MODULE_NAME="s2igevfilter"

PACKAGE_NAME=`grep PACKAGE_NAME ${DKMS_FILE} | cut -d "=" -f 2 | tr -d '"'`
VERSION_VERSION=`grep PACKAGE_VERSION ${DKMS_FILE} | cut -d "=" -f 2 | tr -d '"'`
PACKAGE_DEST_DIR=${DKMS_ROOT_DIR}/${PACKAGE_NAME}-${VERSION_VERSION}


if [ -d ${PACKAGE_DEST_DIR} ] ; then
    echo "Cleaning previous install ${PACKAGE_DEST_DIR}"
	
	if dkms status | grep -q "$MODULE_NAME"
		then
			echo "Module is installed, removing..."
			for ver in $(dkms status | grep "$MODULE_NAME" | awk '{print $2}' | cut -d',' -f1); do
				dkms remove -m $MODULE_NAME -v $ver --all
			done
	fi
    sudo rm -r ${PACKAGE_DEST_DIR}
    sudo rm -r /var/lib/dkms/${PACKAGE_NAME}
fi


mkdir -p ${PACKAGE_DEST_DIR}

sudo cp -f ${DKMS_FILE} ${PACKAGE_DEST_DIR}
sudo cp -Rf ${SRC_DIR} ${PACKAGE_DEST_DIR}

dkms add -m ${PACKAGE_NAME} -v ${VERSION_VERSION}
dkms build -m ${PACKAGE_NAME} -v ${VERSION_VERSION}
dkms install -m ${PACKAGE_NAME} -v ${VERSION_VERSION}

#Load driver
modprobe $MODULE_NAME

#Additional check for proper use: s2igevfilter module exception must be registered in file etc/modules     
if [ -e /etc/modules ]; then
   if grep --quiet -Fx "$MODULE_NAME" /etc/modules; then
      echo "Boot exception module already created" 
   else
     echo "Boot exception module does not exist!"
     make -C ${SRC_DIR} install
     echo "Boot exception module created" 
   fi
else
  echo "Boot Exception modules file does not exist !"
  make -C ${SRC_DIR} install
fi









