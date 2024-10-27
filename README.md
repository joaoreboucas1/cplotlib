# Cplotlib

A plotting library for C, using Matplotlib as backend.

## Introduction

Matplotlib is the current standard of scientific data visualization today. What if we could use Matplotlib from C? Cplotlib is a header-only library that enables that. The library generates a Python code containing the C data, and then evokes a Python interpreter that runs the code.

## Requirements

This library only requires working versions of Python, Numpy and Matplotlib.

## Using Cplotlib

Cplotlib is a header-only library, meaning you only have to copy the file `cplotlib.h` into your C project and including it. Your C project must also be linked to `libpython` and must include `Python.h`.

# Example

We provide an example C code using Cplotlib to plot a sinewave. We also provide a `Makefile` that compiles the example.