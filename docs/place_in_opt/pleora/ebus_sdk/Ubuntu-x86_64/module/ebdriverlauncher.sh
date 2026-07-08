#! /bin/sh
### BEGIN INIT INFO
# Provides:          ebdriverlauncher
# Required-Start:    $remote_fs $syslog $local_fs
# Required-Stop:     $remote_fs $syslog $local_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Starts eBUS Universal Pro For Ethernet driver at boot time
# Description:       Starts eBUS Universal Pro For Ethernet at boot time
### END INIT INFO

#
# NAME         
#     ebdriverlauncher - eBUS Universal Pro For Ethernet driver startup script
#
# DESCRIPTION
#     Manages startup of the eBUS Universal Pro For Ethernet driver at boot time.
#     This driver is used to improve the performance of network streaming with the
#     eBUS SDK.
#              
# COPYRIGHT
#     Pleora Technologies Inc. Copyright (c) 2002-2019
#              
# VERSION
#     7.1.1 build 5002
#              

SCRIPTPATH=`readlink -f $0`
SCRIPTDIR=`dirname $SCRIPTPATH`
DRIVERDIR=""
if [ -z "$DRIVERDIR" ]; then
    DRIVERDIR="$SCRIPTDIR"
fi

DRIVERNAME="ebUniversalProForEthernet"
DRIVERFILE="ebUniversalProForEthernet-x86_64.ko"
DRIVERPATH="$DRIVERDIR/$DRIVERFILE"
DRIVERNODE=/dev/$DRIVERNAME
VERBOSE=0


#
# Log/display message
#
PtLog()
{
  if [ "$VERBOSE" -ne "0" ]; then
    echo $1
  fi
}


#
# Exit on error
#
PtError()
{
  echo $1
  exit $2
}


#
# Function that starts the daemon/service
#
PtStart()
{
  # Do not re-load if not needed
  PtStatus
  if [ "$?" -eq "0" ]; then
	  PtError "Cannot start $DRIVERNAME, it is already running" 1
  fi 

  # Is the driver available?
  if [ ! -f "$DRIVERPATH" ]; then
    PtError "Driver not found" 2
  fi

  # Check if this module can be loaded on this kernel
  if command -v modinfo > /dev/null 2>&1; then
    HOST_VERSION=`uname -r`
    VERMAGIC=`modinfo -F vermagic $DRIVERPATH`
    if [ "$HOST_VERSION" != "${VERMAGIC%% *}" ]; then 
      PtError "Driver mismatch vs kernel: rebuild the driver" 2
    fi
  fi    

  # Load the module
  PtLog "Loading $DRIVERNAME module"
  /sbin/insmod $DRIVERPATH $* || PtError "insmod failed on $DRIVERPATH" 2

  # Remove existing node if any
  PtLog "Removing $DRIVERNAME node (if it exists)"
  rm -f $DRIVERNODE

  # Recreate the nodes
  PtLog "Creating $DRIVERNAME node"
  MAJOR=$(awk "\$2==\"$DRIVERNAME\" {print \$1}" /proc/devices)
  mknod $DRIVERNODE c $MAJOR 0 || PtError "mknod failed on $DRIVERNODE" 2

  # Change permission
  PtLog "Changing $DRIVERNAME node permission"
  group="staff"
  grep -q '^staff:' /etc/group || group="wheel"
  chgrp $group $DRIVERNODE
  chmod 777 $DRIVERNODE

  # Modify the socket configuration
  PtLog "Increasing socket receive and transmit buffers"
  /sbin/sysctl -w net.core.rmem_max=10485760 > /dev/null
  /sbin/sysctl -w net.core.wmem_max=10485760 > /dev/null
}


#
# Function that stops the daemon/service
#
PtStop()
{
  # Ensure the module is in memory
  PtStatus
  if [ "$?" -ne "0" ]; then
    return 1
  fi 

  # Unload the module
  PtLog "Unloading $DRIVERNAME module"
  /sbin/rmmod $DRIVERFILE || PtError "rmnod failed on $DRIVERFILE" 2

  # Remove existing node if any
  PtLog "Removing $DRIVERNAME node"
  rm -f /dev/$DRIVERNAME
}


#
# Tests if the driver is running
#
PtStatus()
{
  DRIVERLOADED=`lsmod | grep -o $DRIVERNAME`
  if [ "$DRIVERLOADED" != "$DRIVERNAME" ]; then
    PtLog "$DRIVERNAME is not running"
    return 1
  fi

  PtLog "$DRIVERNAME is running"
  return 0
}


#
# Validates the script is running with admin privileges
#
PtCheckAdmin()
{
  if [ `id -u` != "0" ]; then
    PtError "You need to run this script as superuser (root account)." 1
  fi
}


# Parse the optional arguments
for i in $*
do
  case $i in
    --verbose|-v)
    VERBOSE=1
    ;;
  esac
done


# Interpret script command
case "$1" in
  start)
    # Start
    PtCheckAdmin
    PtLog "==> Starting $DRIVERNAME"
    PtStart
    ;;
  stop)
    # Stop
    PtCheckAdmin
    PtLog "==> Stopping $DRIVERNAME"
    PtStop
    if [ "$?" -eq 1 ]; then
      echo "Cannot stop $DRIVERNAME: it is not running"
    fi
    exit $?
    ;;
  status)
    # Status
    PtLog "==> Checking status of $DRIVERNAME"
    PtStatus
    if [ "$?" -eq 0 ]; then
      echo "$DRIVERNAME is running"
    else
      echo "$DRIVERNAME is not running"
    fi
    exit $?
    ;;
  restart|force-reload)
    # Restart
    PtCheckAdmin
    PtLog "==> Restarting $DRIVERNAME"
    PtStop
    PtStart
    ;;
  *)
    # Default: show help
    echo "Usage: ebdriverlauncher.sh {start|stop|restart|status|force-reload}" >&2
    exit 3
    ;;
esac

exit 0

