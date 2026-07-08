from setuptools import setup
import os
from glob import glob

package_name = 'hyper_drive'

data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ]

def package_files(data_files, directory_list):

    paths_dict = {}

    for directory in directory_list:
        
        for (path, directories, filenames) in os.walk(directory):

            for filename in filenames:

                file_path = os.path.join(path, filename)
                install_path = os.path.join('share', package_name, path)
                
                if install_path in paths_dict.keys():
                    paths_dict[install_path].append(file_path)
                    
                else:
                    paths_dict[install_path] = [file_path]
                
    for key in paths_dict.keys():
        data_files.append((key, paths_dict[key]))

    return data_files

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    # data_files=[
    #     ('share/ament_index/resource_index/packages',
    #         ['resource/' + package_name]),
    #     ('share/' + package_name, ['package.xml']),
    #     (os.path.join('share', package_name, 'config'), glob('config/*.xml')),
    #     (os.path.join('share', package_name, 'launch'), glob('launch/*')),
    # ],
    data_files=package_files(data_files, ['config/', 'launch/']),
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Aidan Reichenberg',
    maintainer_email='reichenberg.a@northeastern.edu',
    description='The hyper_drive package',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'hyper_drive_pub = hyper_drive.cube_data:main',
            'combined_cube_data = hyper_drive.combined_cube_data:main',
            'hsi_hist = hyper_drive.hsi_hist:main',
            'cube_visualizer = hyper_drive.cube_visualizer:main',
            'synchronous_cubes = hyper_drive.synchronous_cubes:main',
            'synchronous_cube_visualizer = hyper_drive.synchronous_cube_visualizer:main',
        ],
    },
)
