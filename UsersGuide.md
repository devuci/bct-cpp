# Who Can Use bct-cpp? #

bct-cpp is a C++ port of the [Brain Connectivity Toolbox](http://www.brain-connectivity-toolbox.net/) (BCT), a set of MATLAB functions and neuroanatomical data sets useful in the analysis of structural or functional brain networks.

  * Naturally, if you know C++, you can use bct-cpp.
  * If you know (or prefer) Python, you can use bct-cpp to generate Python bindings.  In general, your code will still run faster than the original MATLAB implementation, since the Python bindings use bct-cpp's compiled C++ code under the hood.
  * If you know MATLAB (but not C++ or Python), you can't use bct-cpp.  Download the [MATLAB implementation of BCT](http://www.brain-connectivity-toolbox.net/) instead.


# Installation #


## bct-cpp for C++ ##


### Precision ###

bct-cpp may be compiled to use `float`, `double`, or `long double` precision for floating-point calculations.  The default is `double` precision (see below for instructions on how to change this).  Multiple precisions may be installed side by side and even called from the same piece of client code.  However, Python bindings only work when bct-cpp is compiled in `double` precision.

| **To use this precision ...** | **Use this header ...** | **Use this library ...** | **Use GSL data structures like ...** |
|:------------------------------|:------------------------|:-------------------------|:-------------------------------------|
| `float` | `<bct/bct_float.h>` | bct\_float | `gsl_matrix_float` |
| `double` | `<bct/bct.h>` | bct | `gsl_matrix` |
| `long double` | `<bct/bct_long_double.h>` | bct\_long\_double | `gsl_matrix_long_double` |


### Requirements ###

  * [Subversion](http://subversion.apache.org/)
  * [GNU Make](http://www.gnu.org/software/make/) or equivalent
  * [GCC](http://gcc.gnu.org/) or equivalent
  * [GSL](http://www.gnu.org/software/gsl/)


### How to Install ###

  1. Use Subversion to download the bct-cpp source code.  Instructions may be found on the [Source Checkout](http://code.google.com/p/bct-cpp/source/checkout) page.
  1. Copy Makefile.vars.template to Makefile.vars.  Edit Makefile.vars as needed.
    * `CXXFLAGS`: Additional options to be sent to the C++ compiler.  Some options may already be specified in Makefile.
      * To compile with `float` precision, replace `-DGSL_DOUBLE` with `-DGSL_FLOAT`.  To compile with `long double` precision, replace `-DGSL_DOUBLE` with `-DGSL_LONG_DOUBLE`.
      * To build for a particular architecture, add `-arch <arch>` (e.g., `-arch i386`).  For some versions of GCC, multiple `-arch` options may be specified to build a universal library.
      * To enable parallel processing with OpenMP, add `-fopenmp`.
    * `install_dir`: The directory under which bct-cpp's header and library files will be installed.
    * The remaining variables are only needed if you are building Python bindings with `make swig-manual` (see below).
  1. Run `make`.
  1. Run `make install`.

bct-cpp should now be ready to use.  The code in the example subdirectory demonstrates some simple usage.


### Memory Management ###

  * Some bct-cpp functions return pointers to `gsl_vector` or `gsl_matrix`.  It is your responsibility to free the memory associated with these pointers.
  * Some bct-cpp functions take optional "double pointer" arguments.  These arguments are used to pass additional returns back to the caller.  While the memory for these arguments is allocated by the bct-cpp function, it is your responsibility to free this memory.  If the additional returns are not needed, you may pass `NULL` to the function.  A default value of `NULL` is typically provided in the bct-cpp function prototype, so you can simply ignore any "double pointer" arguments you don't need.

Pointers to both `gsl_vector` and `gsl_matrix` may be freed by calling `bct::gsl_free` rather than GSL's type-specific `gsl_matrix_free`, `gsl_vector_free`, etc.

Avoid usage such as the following:
```
using namespace bct;
gsl_matrix* m;
// ...
double s = sum(sum(triu(m));
```
The call to `triu` returns a pointer to a newly allocated `gsl_matrix`.  The inner call to `sum` returns a pointer to a newly allocated `gsl_vector`.  The memory associated with both of these returns must be freed to avoid a memory leak:
```
using namespace bct;
gsl_matrix* m;
// ...
gsl_matrix* triu_m = triu(m);
gsl_vector* sum_triu_m = sum(triu_m);
double s = sum(sum_triu_m);
gsl_free(triu_m);
gsl_free(sum_triu_m);
```


## bct-cpp for Python ##


### Requirements ###

  * [Python](http://python.org/)
  * [SWIG](http://www.swig.org/)


### How to Install with Distutils ###

  1. Follow the instructions above to install bct-cpp for C++.
  1. Run `make swig`.  Distutils will try to build bct-cpp's Python bindings the same way your Python interpreter was built.  If there are significant differences between the way your Python interpreter was built and the way bct-cpp was built, this step may fail.  There are various ways around this, but details vary from system to system.  You'll need to either install a different Python or change the way you're building bct-cpp.
  1. Run `make swig-install`.

Python bindings for bct-cpp should now be ready to use.  The code in the example subdirectory demonstrates some simple usage.


### How to Install Manually ###

Installing with Distutils may fail if you can't build bct-cpp's Python bindings the same way your Python interpreter was built.  If you can't install a different Python or change the way you're building bct-cpp, you may be able to "manually" install Python bindings.

  1. Follow the instructions above to install bct-cpp for C++.
  1. Edit the additional variables in Makefile.vars.  You may be able to use one of the predefined values (e.g., `python_include_dir = $(python_include_dir_apple)`).
    * `python_include_dir`: The directory containing Python's C++ header files.
    * `python_package_dir`: Python's site-packages directory.
    * `swig_cxx_flags`: Options used when building SWIG's C++ wrapper files.
    * `swig_lib_flags`: Options used when generating a shared library from SWIG wrappers.
  1. Run `make swig-manual`.  You'll have to manually move bct\_py.py (or bct\_gsl.py) and `_`bct\_py.so (or `_`bct\_gsl.so) somewhere accessible to the Python interpreter.
  1. Run `make swig-manual-install`.


### Memory Management ###

bct-cpp's Python bindings come in two flavors: bct\_gsl and bct\_py.  bct\_py acts like a native Python module: you pass Python vectors/matrices (i.e., lists/lists of lists) to functions and receive Python vectors/matrices in return.  All memory management is handled by the module itself.  bct\_gsl gives you more flexibility but is more difficult to use.  Memory management is your responsibility: any memory associated with GSL data structures must be explicitly freed with `gsl_free`, and Python data structures must be explicitly converted to/from GSL data structures with functions like `from_gsl`, `to_gslv`, and `to_gslm`.  If you're unsure which flavor to use, you probably want bct\_py.


# Status Checking #

By default, bct-cpp functions check the status of any matrix passed to them.  E.g., the algorithm used in `bct::clustering_coef_bu` requires that its argument is a square, binary, undirected matrix, and so `bct::clustering_coef_bu` carries out a status check to verify these properties.  If this status check fails, a message is printed to `stderr`, but the function still attempts to complete the calculation.  To disable status checking and avoid the minor computational overhead, call `bct::set_safe_mode(false)`.


# Error Handling #

By default, GSL aborts program execution when an error is encountered.  bct-cpp can override this behavior by setting a custom GSL error handler.  This handler converts a GSL error to a `bct::bct_exception` which may be caught by client code.  `bct::init` must be called to enable this functionality.