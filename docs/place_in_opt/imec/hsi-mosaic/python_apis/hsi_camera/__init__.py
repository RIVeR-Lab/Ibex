import os
import sys

# Get the path of this script
crnt_folder = os.path.abspath(os.path.dirname(__file__))

# Check if we are in the development environment
if os.path.isdir(crnt_folder + r'/../../../hsi_api/hsi_api_python'):
    # If so, set the required paths
    sys.path.append(crnt_folder + r'/../../../hsi_api/hsi_api_python')
    os.environ['PATH'] = crnt_folder + r'/../../../../bin/Release' + os.pathsep + os.environ['PATH']

# Check if we are in the deployment environment
if os.path.isdir(crnt_folder + r'/../bin'):
    # If so, set the required paths
    os.environ['PATH'] = crnt_folder + r'/../bin' + os.pathsep + os.environ['PATH']

from .hsi_camera_api import *
