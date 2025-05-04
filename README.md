# MiNI CAHD - Computer Assisted Hole Detection

Made by students of Warsaw University of Technology, Faculty of Mathematics and Information Science.

Filip Łojek and Patryk Pochmara

# Building

## Windows

1. Create file `./app/build.env` based on [./app/build.env.template] with paths to OpenCASCADE package directories. Use forward slashes!
1. Modify all files in `<OCCT-install-dir>/cmake` replacing all occurrences of `C:/work/occt/3rdparty-vc14-64` with `${THIRD_PARTY_DIR}`
   - `<OCCT-install-dir>` is the path entered in previous step.
   - This is an ugly hack, I welcome suggestions on how to override this path in a better way.
1. Open [./app/CMakeLists.txt] as project in QtCreator and build via UI.

## Linux

Not ready yet. A similar approach as above might be possible.
