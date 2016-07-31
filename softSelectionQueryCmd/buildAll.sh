#!/bin/bash

mkdir -p bin

declare -a maya_versions=(2011 2012 2013 2014 2015 2016 2017)

# output binary name
if [ "$(uname -s)" == "Darwin" ]; then
    output_name=softSelectionQuery.bundle
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    output_name=softSelectionQuery.so
fi

output_bin_file=./build/$output_name
maya_sdk_dir=/Volumes/Data/Libs/MayaSDK

for v in "${maya_versions[@]}"
do
    echo ""
    echo "Building plugin for Maya $v"
    rm -rf ./build
    mkdir build
    cmake -DMAYA_VERSION=$v -B./build -H. -DMAYA_SDK_BASE_DEFAULT=$maya_sdk_dir
    cmake --build ./build --clean-first

    dest_dir=./bin/$v/plug-ins/
    test -d $dest_dir || mkdir -p $dest_dir && cp $output_bin_file $dest_dir
done

echo ""
echo "All DONE!"