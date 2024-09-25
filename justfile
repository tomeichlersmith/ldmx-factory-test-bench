[private]
default:
    just --list

# current build options
@list:
    [ -d build ] && ls build/

# configure and build a specific option
build NAME *CONFIG:
    denv cmake -B build/{{ NAME }} -S . {{ CONFIG }}
    denv cmake --build build/{{ NAME }}

# run a specific option
run NAME ENTRY="Book": (build NAME)
    denv build/{{ NAME }}/fave-things build/{{ NAME }}/libLibrary.so {{ ENTRY }}

