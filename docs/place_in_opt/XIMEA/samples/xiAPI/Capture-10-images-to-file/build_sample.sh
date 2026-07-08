#!/bin/bash

# exit when any command fails
set -e -o pipefail

mkdir -p tmp
pushd tmp
cmake .. -G "Unix Makefiles"
make
popd
