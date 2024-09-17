[private]
default:
    just --list

[private]
build NAME *CONFIG:
    denv cmake -B build/{{ NAME }} -S . {{ CONFIG }}
    denv cmake --build build/{{ NAME }}


# build without LTO
build-no-lto: (build "no-lto")

# build with LTO
build-lto: (build "lto" "-DENABLE_LTO=YES" "-DCMAKE_CXX_COMPILER=clang++" "-DCMAKE_C_COMPILER=clang")

[private]
run NAME ENTRY:
    denv build/{{ NAME }}/fave-things build/{{ NAME }}/libLibrary.so {{ ENTRY }}

# run without LTO
run-lto ENTRY: build-lto (run "lto" ENTRY)

# run with LTO
run-no-lto ENTRY: build-no-lto (run "no-lto" ENTRY)

