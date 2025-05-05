# C++ Factory Source

The main file we are testing is the templated Factory in [Factory.h](Factory.h).
The program compiled here returns different exit codes depending on the result for automated testing.

Code | Description
---|---
-1 | Bad command line arguments
0 | Successfully loaded the library and created an object by name
1 | Loaded the library but failed to create object by name
2 | Failed to load library
3 | Failed to load `run` function
