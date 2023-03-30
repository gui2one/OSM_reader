$build_dir = "./build"
$config = "Release"
$exe = "osm_reader.exe"

Remove-Item -Path $build_dir/$config/$exe


cmake --build $build_dir --config $config --target ALL_BUILD -j 14


Invoke-Expression $build_dir/$config/$exe