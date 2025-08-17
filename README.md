# Cplotlib

A plotting library for C, using Python and Matplotlib as backend.

## Introduction

Matplotlib is the current standard of scientific data visualization. What if we could use Matplotlib from C? Cplotlib is a header-only library that enables that. The library generates Python code using the C data, and then evokes a Python interpreter to run the code.

## Requirements

This library only requires working versions of Python, Numpy and Matplotlib. The user must provide the paths to `Python.h` and `libbpythonX.Y.so` (where `X.Y` is your Python version). For the global Python environment, the headers are located in `/usr/include/python3.12/`, while the libraries are automatically found. Conda users may find these files within the Conda installation path.

## Using Cplotlib

Cplotlib is a header-only library, meaning you only have to copy the files `cplotlib.h` and `nob.h` into your C project and `#include` `cplotlib.h` in your C source code. Your C project must also be linked to `libpython` and must include `Python.h`, so point the compiler to those when compiling your C project.

The aim of this project is to be able to compile as many Matplotlib functions as possible from C code. As such, functions as `cpl_plot` call `plt.plot` under the hood, being designed to have a similar API. Most functions are able to get `kwargs` to customize the `plt` function calls.

The Python program is a `String_Builder` from `nob.h` and can be modified with `sb_append`. C array elements are compiled to strings, which are then baked into `numpy` arrays in the Python program using the `declare_array` function. To keep track of the data being passed to `cplotlib`, pointers are identified with an `id`, such that the corresponding Python object is named `x{id}`. The function `cpl_exec_program` runs the Python program. If you want more control over the Python program, you can use these commands directly.

## Example

We provide an example C code using Cplotlib to plot some arbitrary data, with a `Makefile` that compiles it. Adjust the include and library paths if necessary.

## Thanks

Thanks [Tsoding](https://github.com/tsoding) for creating amazing projects and providing `nob.h`.