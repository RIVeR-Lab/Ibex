from setuptools import find_packages, setup
import os
from glob import glob

package_name = 'ibex_state'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['tests', 'tests.*']),
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.py')),
        (os.path.join('share', package_name, 'config'), glob('config/*')),
    ],
    install_requires=['setuptools', 'utm'],
    extras_require={
        'test': ['pytest', 'mock']  # Testing dependencies
    },
    zip_safe=True,
    maintainer='Ben Cometto',
    maintainer_email='benjamin@cometto.org',
    description='State estimation for IBEX',
    license='Apache-2.0',
    entry_points={
        'console_scripts': [
            'graph_frontender = ibex_state.graph_frontender:main',
        ],
    },
)
