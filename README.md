# MiNI CAHD - Computer Assisted Hole Detection

Made by students of Warsaw University of Technology, Faculty of Mathematics and Information Science.

Filip Łojek and Patryk Pochmara

# Building

Requires OpenCASCADE installed.

## Windows

Requires OpenCASCADE 3rd party libs in addition to OpenCASCADE itself.

1. Configure CMake variable `OpenCASCADE_DIR` to point to your OpenCASCADE's installation cmake directory.
   - `OpenCASCADE_DIR` should contain `OpenCASCADEConfig.cmake` file among others.
   - Set it in QtCreator->Build->CMake->CurrentConfiguration or append `-D OpenCASCADE_DIR=<dir>` to `cmake` CLI arguments or use `cmake-gui`.
1. Modify all files in `OpenCASCADE_DIR` replacing all occurrences of `C:/work/occt/3rdparty-vc14-64` with `${THIRD_PARTY_DIR}`
   - `OpenCASCADE_DIR` is the path entered in previous step.
   - This is an ugly hack, I welcome suggestions on how to override this path in a better way.
1. Configure CMake variable `THIRD_PARTY_DIR` to point to your OpenCASCADE's installation 3rd party libs directory.
   - `THIRD_PARTY_DIR` should contain `jemalloc-...` subdir among others.
   - Set it in QtCreator->Build->CMake->CurrentConfiguration or append `-D THIRD_PARTY_DIR=<dir>` to `cmake` CLI arguments or use `cmake-gui`.
1. Open [CMakeLists.txt](./app/CMakeLists.txt) as project in QtCreator and build via UI.
   - You can also try `cmake [--fresh] -S app -B build <variables described above> && cmake --build build`, but it's untested.
1. Executable will be named `CAHD.exe` and reside somewhere within build directory.

## Linux

1. \[OPTIONAL\] Configure CMake variable `OpenCASCADE_DIR` to point to your OpenCASCADE's installation cmake directory.
   - This step should not be needed when installing OpenCASCADE with your distro's package manager.
1. Open [CMakeLists.txt](./app/CMakeLists.txt) as project in QtCreator and build via UI.
   - You can also use `cmake [--fresh] -S app -B build && cmake --build build`.
1. Executable will be named `CAHD` and reside somewhere within build directory.
