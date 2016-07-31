@echo off

md bin

set maya_versions=2011, 2012, 2013, 2014, 2015, 2016, 2017
set output_name=softSelectionQuery.mll
set output_bin_file=build\Release\%output_name%
set maya_sdk_dir=V:\Libs\MayaSDK

for %%v in (%maya_versions: = %) do (
    echo.
    echo Building plugin for Maya %%v
    rd /q /s build # IMPORTANT
    md build
    cmake -G "Visual Studio 11 Win64" -DMAYA_VERSION=%%v -B./build -H. -DMAYA_SDK_BASE_DEFAULT=%maya_sdk_dir%
    cmake --build ./build --clean-first --config Release
    xcopy %output_bin_file% bin\%%v\plug-ins\ /i/y
)

echo.
echo All DONE!