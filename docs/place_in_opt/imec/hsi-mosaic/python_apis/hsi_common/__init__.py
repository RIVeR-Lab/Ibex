import os
import platform
import sys

# Get the path of this script
crnt_folder = os.path.abspath(os.path.dirname(__file__))

# Check if we are in the development environment
if os.path.isdir(crnt_folder + r'/../../../hsi_api/hsi_api_python'):
    # If so, set the required paths
    sys.path.append(crnt_folder + r'/../../../hsi_api/hsi_api_python')

    # According to https://stackoverflow.com/questions/856116/changing-ld-library-path-at-runtime-for-ctypes
    # setting the LD_LIBRARY_PATH from within a script has no effect on the availability of libraries, as 
    # the dynamic loader has already read the variable, and won't notice any changes thereafter.

    if (platform.system() == 'Windows'):
        os.environ['PATH'] = crnt_folder + r'/../../../../bin/Release' + os.pathsep + os.environ['PATH']

# Check if we are in the deployment environment
if os.path.isdir(crnt_folder + r'/../../bin'):
    # If so, set the required paths
    if (platform.system() == 'Windows'):
        os.environ['PATH'] = crnt_folder + r'/../../bin' + os.pathsep + os.environ['PATH']

from .hsi_common_api import *
