# C++ Factory Testing Ground

```
just multi-test summary
```

Needs
- `just`
- GNU `parallel`
- `denv`
- container runner supported by `denv`

# :tada: Unique `static` Variables :tada:
Moving the _definition_ of the singleton factory instance to the implementation file
for the prototype class seems to have resolved the issues being observed within ldmx-sw.
This makes sense to me. Before, when the `static Factory` was in the `Factory.h` header,
it could be defined in multiple source files leading to a complicated set of duplicates
that the linker had to resolve. In some cases (e.g. GCC in the ldmx/dev image), the linker
was able to resolve them properly while it was unable to do so in other cases.
The solution to this duplication issue is to ensure that `static Factory` is only written down
once per factory, which we can accomplish by only writing `static Factory` in the implementation file
for the prototype class. I've wrapped the definition and declaration for a type-specific factory
in some macros to make there be a bit less boilerplate for users to write.

# :warning: `ENABLE_EXPORTS` was not the fix :warning:

Upon further investigation, using `ENABLE_EXPORTS` did not fix the issue within ldmx-sw.
Moreover, using the newer OS and its newer GCC and Clang compilers (ldmx/dev:5.0.0), I am unable to replicate
the double-construction observed with the v4 ldmx/dev images.

I've kept my notes from before below for now, but I'll probably delete them as I think of more.

---

## This was a doozy
The key annoynace is how `static` variables are (by default) library-local.
This means that when the Factory :tm: is built into a separate library
_and_ built into the executable, there end up being **two** Factory instances
one that registers/declares things within the library and the other trying
to create things within the executable.
The solution is to enforce "exporting" of these symbols.
This is done with the
[`-rdynamic`](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html#index-rdynamic)
linker flag or the `ENABLE_EXPORTS` property within CMake applied to the executable
that loads the libraries.
```cmake
set_property(TARGET <tgt> PROPERTY ENABLE_EXPORTS TRUE)
```
As pointed out [on StackOverflow](https://stackoverflow.com/a/60768340),
this is necessary in order to enable a function loaded from a library to
call a function defined within the executable (i.e. a callback) which is
almost exactly what we want in this situation.


### As Designed
I orginally designed the `Factory` where the `declare` and `make` calls
are compiled into the same target executable.
This leads to the desired behavior.

<details>

  <summary>Build and Running Output</summary>

```
tom@appa:~/code/ldmx/1465-lto$ just build 0-as-designed -DSEPARATE_LIB=OFF
denv cmake -B build/0-as-designed -S . -DSEPARATE_LIB=OFF
-- The CXX compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/tom/code/ldmx/1465-lto/build/0-as-designed
denv cmake --build build/0-as-designed
[ 20%] Building CXX object CMakeFiles/fave-things.dir/fave_things.cpp.o
[ 40%] Building CXX object CMakeFiles/fave-things.dir/Book.cpp.o
[ 60%] Building CXX object CMakeFiles/fave-things.dir/Album.cpp.o
[ 80%] Building CXX object CMakeFiles/fave-things.dir/Podcast.cpp.o
[100%] Linking CXX executable fave-things
[100%] Built target fave-things
tom@appa:~/code/ldmx/1465-lto$ just run 0-as-designed
denv cmake -B build/0-as-designed -S . 
-- Configuring done
-- Generating done
-- Build files have been written to: /home/tom/code/ldmx/1465-lto/build/0-as-designed
denv cmake --build build/0-as-designed
Consolidate compiler generated dependencies of target fave-things
[100%] Built target fave-things
denv build/0-as-designed/fave-things build/0-as-designed/libLibrary.so Book
Factory(0x559d6384e2c0)
Factory(0x559d6384e2c0): declare Book
Factory(0x559d6384e2c0): declare Album
Factory(0x559d6384e2c0): declare Podcast
From executable: library compiled into executable
Factory(0x559d6384e2c0): make Book
Poverty, By America
~Factory(0x559d6384e2c0)
```

</details>

### Separate `declare` from `make`
The key issue I observed within ldmx-sw is that the address of the Facotry used to declare
was different from the address of the Factory used to make.
I can do this directly by compiling the Library where declaration happens into a separate
object that is then manually loaded at runtime.
This does replicate the issue observed in ldmx-sw.

<details>

  <summary>Build and Running Output</summary>

```
tom@appa:~/code/ldmx/1465-lto$ just build 1-sep-make
denv cmake -B build/1-sep-make -S . 
-- The CXX compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/tom/code/ldmx/1465-lto/build/1-sep-make
denv cmake --build build/1-sep-make
[ 16%] Building CXX object CMakeFiles/Library.dir/Book.cpp.o
[ 33%] Building CXX object CMakeFiles/Library.dir/Album.cpp.o
[ 50%] Building CXX object CMakeFiles/Library.dir/Podcast.cpp.o
[ 66%] Linking CXX shared library libLibrary.so
[ 66%] Built target Library
[ 83%] Building CXX object CMakeFiles/fave-things.dir/fave_things.cpp.o
[100%] Linking CXX executable fave-things
[100%] Built target fave-things
tom@appa:~/code/ldmx/1465-lto$ just run 1-sep-make
denv cmake -B build/1-sep-make -S . 
-- Configuring done
-- Generating done
-- Build files have been written to: /home/tom/code/ldmx/1465-lto/build/1-sep-make
denv cmake --build build/1-sep-make
Consolidate compiler generated dependencies of target Library
[ 66%] Built target Library
Consolidate compiler generated dependencies of target fave-things
[100%] Built target fave-things
denv build/1-sep-make/fave-things build/1-sep-make/libLibrary.so Book
From executable: Factory(0x55833d9262c0)
manually loading library
Factory(0x7f4bd6d3a9e0)
Factory(0x7f4bd6d3a9e0): declare Book
Factory(0x7f4bd6d3a9e0): declare Album
Factory(0x7f4bd6d3a9e0): declare Podcast
done loading library
Factory(0x55833d9262c0): make Book
ERROR: An object named Book has not been declared.
~Factory(0x7f4bd6d3a9e0)
~Factory(0x55833d9262c0)
```

</details>


### Enable symbol exports
If we tell CMake to tell the compiler that we want the executable and library
to load global symbols from each other, then we return to the desired behavior.

<details>

<summary>Build and Run Output</summary>

```
tom@appa:~/code/ldmx/1465-lto$ just build 2-sep-make-with-export -DENABLE_SYMB_EXPORTS=ON
denv cmake -B build/2-sep-make-with-export -S . -DENABLE_SYMB_EXPORTS=ON
-- The CXX compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/tom/code/ldmx/1465-lto/build/2-sep-make-with-export
denv cmake --build build/2-sep-make-with-export
[ 16%] Building CXX object CMakeFiles/Library.dir/Book.cpp.o
[ 33%] Building CXX object CMakeFiles/Library.dir/Album.cpp.o
[ 50%] Building CXX object CMakeFiles/Library.dir/Podcast.cpp.o
[ 66%] Linking CXX shared library libLibrary.so
[ 66%] Built target Library
[ 83%] Building CXX object CMakeFiles/fave-things.dir/fave_things.cpp.o
[100%] Linking CXX executable fave-things
[100%] Built target fave-things
tom@appa:~/code/ldmx/1465-lto$ just run 2-sep-make-with-export
denv cmake -B build/2-sep-make-with-export -S . 
-- Configuring done
-- Generating done
-- Build files have been written to: /home/tom/code/ldmx/1465-lto/build/2-sep-make-with-export
denv cmake --build build/2-sep-make-with-export
Consolidate compiler generated dependencies of target Library
[ 66%] Built target Library
Consolidate compiler generated dependencies of target fave-things
[100%] Built target fave-things
denv build/2-sep-make-with-export/fave-things build/2-sep-make-with-export/libLibrary.so Book
From executable: Factory(0x55e13651d2c0)
manually loading library
Factory(0x55e13651d2c0): declare Book
Factory(0x55e13651d2c0): declare Album
Factory(0x55e13651d2c0): declare Podcast
done loading library
Factory(0x55e13651d2c0): make Book
Poverty, By America
~Factory(0x55e13651d2c0)
```

</details>

This solution also avoids _Link-Time_ Optimization (LTO) from separating
the variables that need to be not separated.

<details>
<summary>Build and Run Output</summary>

```
tom@appa:~/code/ldmx/1465-lto$ just build 3-sep-make-with-export-and-lto -DENABLE_SYMB_EXPORTS=ON -DENABLE_LTO=ON                                                                           
denv cmake -B build/3-sep-make-with-export-and-lto -S . -DENABLE_SYMB_EXPORTS=ON -DENABLE_LTO=ON
-- The CXX compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/tom/code/ldmx/1465-lto/build/3-sep-make-with-export-and-lto
denv cmake --build build/3-sep-make-with-export-and-lto
[ 16%] Building CXX object CMakeFiles/Library.dir/Book.cpp.o
[ 33%] Building CXX object CMakeFiles/Library.dir/Album.cpp.o
[ 50%] Building CXX object CMakeFiles/Library.dir/Podcast.cpp.o
[ 66%] Linking CXX shared library libLibrary.so
[ 66%] Built target Library
[ 83%] Building CXX object CMakeFiles/fave-things.dir/fave_things.cpp.o
[100%] Linking CXX executable fave-things
[100%] Built target fave-things
tom@appa:~/code/ldmx/1465-lto$ just run 3-sep-make-with-export-and-lto
denv cmake -B build/3-sep-make-with-export-and-lto -S . 
-- Configuring done
-- Generating done
-- Build files have been written to: /home/tom/code/ldmx/1465-lto/build/3-sep-make-with-export-and-lto
denv cmake --build build/3-sep-make-with-export-and-lto
Consolidate compiler generated dependencies of target Library
[ 66%] Built target Library
Consolidate compiler generated dependencies of target fave-things
[100%] Built target fave-things
denv build/3-sep-make-with-export-and-lto/fave-things build/3-sep-make-with-export-and-lto/libLibrary.so Book
From executable: Factory(0x5605ba4e8060)
manually loading library
Factory(0x5605ba4e8060): declare Podcast
Factory(0x5605ba4e8060): declare Album
Factory(0x5605ba4e8060): declare Book
done loading library
Factory(0x5605ba4e8060): make Book
Poverty, By America
~Factory(0x5605ba4e8060)
```

</details>
