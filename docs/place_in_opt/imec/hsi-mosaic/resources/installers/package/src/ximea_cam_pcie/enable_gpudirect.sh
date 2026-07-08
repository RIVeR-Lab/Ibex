#!/bin/bash
function Check() {
	if [ $? == 0 ]
	then
		echo "OK"
	else
		echo "Error on previous command"
		exit 1
	fi
}

cd "$(dirname "$0")"
KERNEL="${KERNEL:-$(uname -r)}"
NV_VERSION="${NV_VERSION:-$(modinfo -k "$KERNEL" -F version "$(modprobe -R nvidia)")}"
if [ -z "$NV_VERSION" ]
then
	echo "NVIDIA drivers are not installed!!!"
	exit 1
fi
if [ ! -d "NVIDIA-Linux-x86_64-$NV_VERSION" ]
then
	echo "Downloading NVIDIA drivers package"
	curl -fLO "http://us.download.nvidia.com/XFree86/Linux-x86_64/${NV_VERSION}/NVIDIA-Linux-x86_64-${NV_VERSION}.run" || \
			curl -fLO "http://us.download.nvidia.com/tesla/${NV_VERSION}/NVIDIA-Linux-x86_64-${NV_VERSION}.run"
	if [ $? != 0 ]
	then
		echo "Could not find the requested version ${NV_VERSION}!"
		echo "Please check the list of available NVIDIA drivers packages here:"
		echo "http://download.nvidia.com/XFree86/Linux-x86_64/"
		echo "Choose a version closest to ${NV_VERSION} (first two numbers must match) and re-run this script like this:"
		echo "NV_VERSION=<VERSION> ./enable_gpudirect.sh"
		echo "where <VERSION> is the version you've chosen"
		exit 1
	fi
	chmod a+x "NVIDIA-Linux-x86_64-$NV_VERSION.run"
	./NVIDIA-Linux-x86_64-$NV_VERSION.run -x
	Check
fi
echo "Copying nv-p2p.h"
find "NVIDIA-Linux-x86_64-$NV_VERSION" -name nv-p2p.h -exec cp '{}' . \;
Check
make clean
echo "Generating Module.symvers"
pushd "NVIDIA-Linux-x86_64-$NV_VERSION"/kernel
make clean
make KERNEL="$KERNEL" module
Check
popd
echo "Rebuilding ximea_cam_pcie kernel module"
make KERNEL="$KERNEL" GPUDIRECT=1 KBUILD_EXTRA_SYMBOLS="$(pwd)"/"NVIDIA-Linux-x86_64-$NV_VERSION"/kernel/Module.symvers
Check
echo "Installing ximea_cam_pcie kernel module"
make KERNEL="$KERNEL" install
Check
depmod "$KERNEL"
Check
if [ "$KERNEL" = "$(uname -r)" ]
then
	echo "Reloading ximea_cam_pcie kernel module, will fail if it is in use"
	rmmod ximea_cam_pcie 2>/dev/null
	modprobe --first-time ximea_cam_pcie
	Check
fi

echo "Add the following lines to /etc/rc.local or analogous file and reboot the system:"
echo 'modprobe ximea_cam_pcie'
echo 'major="$(grep ximea_cam_pcie /proc/devices | cut -f1 -d\ )"'
echo 'for i in $(seq "$(lspci -nd deda:|wc -l)")'
echo 'do'
echo 'minor="$((i - 1))"'
echo 'devname="/dev/ximea$(printf "%02d" ${minor})"'
echo 'mknod -m 660 "${devname}" c "${major}" "${minor}"'
echo 'chgrp plugdev "${devname}"'
echo 'done'
