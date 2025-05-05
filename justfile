[private]
@default:
    just --list

# configure, build, and run a specific option
test COMPILER NAME *CONFIG:
    #!/bin/bash
    set -eu
    dest=build/{{ NAME }}/{{ COMPILER }}
    mkdir -p "${dest}"
    denv cmake -B "${dest}" -S src/ {{ if COMPILER == "clang" { "-DCMAKE_XX_COMPILER=clang" } else { "" } }} {{ CONFIG }} &> "${dest}/test.log"
    if ! denv cmake --build "${dest}" &>> "${dest}/test.log"; then
      echo "-2" > "${dest}/result"
      exit
    fi
    denv ${dest}/fave-things ${dest}/libLibrary.so Book &>> "${dest}/test.log"
    echo "$?" > "${dest}/result"

# test all of the configurations in jobs.txt with both compilers
multi-test:
    parallel eval just test ::: clang gcc :::: jobs.txt

# summarize results from a multi-test
summary:
    #!/bin/bash
    set -eu
    while read line; do
      words=($line);
      name="${words[0]}"
      printf "${line}\n"
      for compiler in gcc clang; do
          printf "    %5s: %d\n" "${compiler}" "$(cat build/${name}/${compiler}/result)"
      done
    done < jobs.txt
