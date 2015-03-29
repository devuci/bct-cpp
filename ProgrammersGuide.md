# Creating a New Function #
  1. **Create function\_name.cpp**: Write the function in its own .cpp file.  Use the original BCT function name as both the filename and the function name.  Make sure to define the function in the `bct` namespace.
  1. **Update bct.h**: Add the function prototype for `function_name` to the `bct` namespace.
  1. **Update Makefile**: Add `function_name.o` to the list of `objects`.
  1. **Create test/function\_name\_cpp.cpp**: Write an Octave wrapper function in its own .cpp file.  The two macros defined in test/bct\_test.h should make this easy.  If the function calculates a scalar quantity, use the `MATRIX_TO_SCALAR_FUNCTION` macro.  If it calculates a vector or matrix (Octave doesn't care which), use the `MATRIX_TO_MATRIX_FUNCTION` macro.  These macros convert an Octave matrix to a GSL matrix, pass it to the C++ function in question, and convert the result back into Octave format.  Some error checking is done to ensure we don't crash Octave if something goes wrong.  Just write `MATRIX_TO_X_FUNCTION(function_name)`, and the macros take care of the rest.
  1. **Update test/bct\_test\_section.m**: Now that the C++ function is callable from Octave, add a test to compare the outputs of the original BCT function and the C++ version.  In general, this involves looping through the six cortical connectivity data sets and comparing the results of calls to `function_name` (original version) and `function_name_cpp` (C++ version).  Details, however, may vary from function to function.
  1. **Update test/Makefile**: Add `function_name_cpp` to the list of `filenames`.


# Dealing with Multiple Returns #
Use the following strategy to implement a BCT function that returns multiple values:
  * If computation is conserved by multiple returns:
    * If returns are all scalars, return a vector of those scalars.
    * Otherwise:
      * Return the "main" vector/matrix and allow pointer arguments.  Note that these arguments will be "double pointers" for vector or matrix types (`gsl_vector**` or `gsl_matrix**`).
      * For each pointer parameter:
        * If the pointer is non-`NULL`, return valid data.
        * Otherwise, allocate and free local memory.
  * Otherwise, decompose into multiple functions with single returns.


# Running the Regression Test Suite #
  1. **Install the bct-cpp library**: Run `make` followed by `make install` to compile and install libbct.a.
  1. **Install the regression test suite**: In the test directory, run `make` followed by `make install` to compile oct-files and install them (along with the supporting .m files).  `mkoctfile` (the Octave executable) must be in your shell's path for this to work.
  1. **Run the suite**: Start Octave and execute `bct_test_all`.  The regression test suite (located at /usr/local/share/bct by default) and the original BCT .m files must be in your Octave path for this to work.  See the Octave documentation for the `path`, `addpath`, and `savepath` commands.