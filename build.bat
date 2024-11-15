cd C:/Users/MAIN
cmake -G "Visual Studio 16 2019" -S source\repos\HeydoubleU\Hyuu -B source\builds\Hyuu -DCMAKE_INSTALL_PREFIX=source\bifrost_libs
cmake --build source\builds\Hyuu --target install --config Release