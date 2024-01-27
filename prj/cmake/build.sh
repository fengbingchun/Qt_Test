#! /bin/bash

if [ $# != 1 ]; then
    echo "Error: requires one parameter: Release or Debug"
    echo "For example: $0 Debug"
    exit -1
fi

if [ $1 != "Debug" ] && [ $1 != "Release" ]; then
    echo "Error: supported parameter values are only Debug or Release: $1"
    exit -1
fi

result_judgment()
{
    rc=$?
    if [[ ${rc} != 0 ]]; then
        echo -e "\033[0;31mError: there are some errors in the above operation, please check: ${rc}\033[0m"
        exit ${rc}
    fi
}

build_dir="build"
if [ ! -d ${build_dir} ]; then
    mkdir -p ${build_dir}
fi

cd build

cmake \
    -G"Visual Studio 17 2022" -A x64 \
    -DCMAKE_BUILD_TYPE=$1 \
    -DCMAKE_CONFIGURATION_TYPES=$1 \
    ..
result_judgment

cmake --build . --config $1
result_judgment

echo -e "\033[0;32mbuild finish\033[0m"
