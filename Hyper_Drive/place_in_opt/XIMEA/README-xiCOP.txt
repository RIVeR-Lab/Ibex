xiCOP for Linux requires the following tools and libraries to be installed on the system:

* sed
* awk
* lsof
* lspci
* lshw
* libcurl

If not run with root privileges xiCOP will try to gain them using `sudo` utility.


Examples of usage:

1). Print overview of detected cameras and issues:
$ /opt/XIMEA/bin/xiCOP

2). Update firmware on the first camera:
$ /opt/XIMEA/bin/xiCOP -fwupdate

3). Update firmware on the specific camera:
$ /opt/XIMEA/bin/xiCOP -fwupdate <serial number>

4). Save diagnostics report to file `xicop_report.xml` in the current directory:
$ /opt/XIMEA/bin/xiCOP -save_diag
