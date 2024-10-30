# Cplotlib

A plotting library for C, using Matplotlib as backend.

## Introduction

Matplotlib is the current standard of scientific data visualization. What if we could use Matplotlib from C? Cplotlib is a header-only library that enables that. The library generates Python code using the C data, and then evokes a Python interpreter to runs the code.

## Requirements

This library only requires working versions of Python, Numpy and Matplotlib.

## Using Cplotlib

Cplotlib is a header-only library, meaning you only have to copy the file `cplotlib.h` into your C project and including it. Your C project must also be linked to `libpython` and must include `Python.h`.

Most of the `plt` functions have Cplotlib counterparts, such as `cpl_plot` which calls `plt.plot` under the hood. Some functions have `kwargs` which enable configuring the plot style (color, linestyle, line width, alpha...).

To control the Python program, the library uses the functions `append_cmd`, `declare_array` and `exec_program`. If you want more control over the Python program, you can use these commands directly.

## Example

We provide an example C code using Cplotlib to plot a sinewave. We also provide a `Makefile` that compiles the example.