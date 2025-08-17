#ifndef CPLOTLIB_H_
#define CPLOTLIB_H_

#include <Python.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define CPLOTLIB_API

#define CPL_ARRAY_LEN(x) sizeof((x))/sizeof((x)[0])
CPLOTLIB_API static inline void cpl_print_program();
CPLOTLIB_API void cpl_exec_program();
CPLOTLIB_API static inline void cpl_reset_program();
CPLOTLIB_API bool declare_array(double* x, size_t x_len);
int _cpl_plot(double* x, size_t len_x, double* y, size_t len_y, const char* kwargs);
int _cpl_scatter(double* x, size_t len_x, double* y, size_t len_y, const char* kwargs);
int _cpl_loglog(double* x, size_t len_x, double* y, size_t len_y, const char* kwargs);
void _cpl_fill_between(double* x, size_t len_x, double* y1, size_t len_y1, double* y2, size_t len_y2, const char* kwargs);
#define cpl_plot(x, y, kwargs) _cpl_plot(x, CPL_ARRAY_LEN((x)), y, CPL_ARRAY_LEN((y)), kwargs);
#define cpl_scatter(x, y, kwargs) _cpl_scatter(x, CPL_ARRAY_LEN((x)), y, CPL_ARRAY_LEN((y)), kwargs);
#define cpl_loglog(x, y, kwargs) _cpl_loglog(x, CPL_ARRAY_LEN((x)), y, CPL_ARRAY_LEN((y)), kwargs);
#define cpl_fill_between(x, y1, y2, kwargs) _cpl_fill_between((x), CPL_ARRAY_LEN((x)), (y1), CPL_ARRAY_LEN((y1)), (y2), CPL_ARRAY_LEN((y2)), kwargs)
CPLOTLIB_API void cpl_xlabel(const char* xlabel);
CPLOTLIB_API void cpl_ylabel(const char* ylabel);
CPLOTLIB_API void cpl_title(const char* title);
CPLOTLIB_API void cpl_xlim(double x1, double x2);
CPLOTLIB_API void cpl_ylim(double y1, double y2);
CPLOTLIB_API void cpl_show();
CPLOTLIB_API void cpl_savefig(const char *filename);
CPLOTLIB_API void cpl_grid();
CPLOTLIB_API void cpl_legend();

typedef struct {
	double* p;
	size_t id;
} cpl_array;

typedef struct {
	cpl_array *items;
	size_t count;
	size_t capacity;
} cpl_arrays;

String_Builder program = {0};
cpl_arrays arrays = {0};

const char* preamble = "\
import numpy as np\n\
import matplotlib.pyplot as plt\n\
";

#ifdef CPLOTLIB_IMPLEMENTATION

static inline void cpl_print_program() 
{
	printf("%*s\n", (int)program.count, program.items);
}

void cpl_exec_program()
{
	// Executes a string as a Python program
	Py_Initialize();
	PyRun_SimpleString(program.items);
	Py_Finalize();
}

static inline void cpl_reset_program()
{
	// Resets the program state to only the preamble
	program.count = 0;
	arrays.count = 0;
	sb_append_cstr(&program, preamble);
}

size_t get_array_id(double *x) {
	for (size_t i = 0; i < arrays.count; i++) {
		const cpl_array array = arrays.items[i];
		if (array.p == x) return array.id;
	}
	return -1;
}

bool declare_array(double* x, size_t x_len)
{
	// `arrays` is a global dynamic array storing the pointers known to the program
	// If x is in `arrays`, then this function does nothing
	// If x is not in `arrays`:
	//     Constructs the command `x%i = np.array([x[0], x[1], ...])`. %i is the number of arrays in `arrays`
	//     Appends the command into the program.
	//     Appends the pointer into `arrays`.
	for (size_t i = 0; i < arrays.count; i++) {
		if (x == arrays.items[i].p) return true;
	}
	sb_appendf(&program, "x%zu = np.array([", arrays.count);
	for (size_t i = 0; i < x_len; i++) {
		sb_appendf(&program, "%.8g", x[i]);
		if (i < x_len - 1) {
			sb_append_cstr(&program, ", ");
		}
	}
	sb_append_cstr(&program, "])\n");	
	cpl_array a = (cpl_array) {.p = x, .id = arrays.count};
	da_append(&arrays, a);
	return true;
}

int _cpl_plot(double* x, size_t len_x, double* y, size_t len_y, const char* kwargs)
{
	// Compiles the command `plt.plot(x, y, kwargs)`
	if (program.count == 0) sb_append_cstr(&program, preamble);
	if (len_x != len_y) {
		printf("ERROR: plotting arrays of different lengths.\n");
		return false;
	}
	
	if (!declare_array(x, len_x)) {
		printf("ERROR: could not allocate array into program.\n");
		exit(1);
	}
	
	if (!declare_array(y, len_y)) {
		printf("ERROR: could not allocate array into program.\n");
		exit(1);
	}
	
	sb_appendf(&program, "plt.plot(x%zu, x%zu, %s)\n", get_array_id(x), get_array_id(y), kwargs);
	return true;
}

int _cpl_scatter(double* x, size_t len_x, double* y, size_t len_y, const char* kwargs)
{
	// Compiles the command `plt.plot(x, y, kwargs)`
	if (program.count == 0) sb_append_cstr(&program, preamble);
	if (len_x != len_y) {
		printf("ERROR: plotting arrays of different lengths.\n");
		return false;
	}
	
	if (!declare_array(x, len_x)) {
		printf("ERROR: could not allocate array into program.\n");
		exit(1);
	}
	
	if (!declare_array(y, len_y)) {
		printf("ERROR: could not allocate array into program.\n");
		exit(1);
	}
	
	sb_appendf(&program, "plt.scatter(x%zu, x%zu, %s)\n", get_array_id(x), get_array_id(y), kwargs);
	return true;
}

int _cpl_loglog(double* x, size_t len_x, double* y, size_t len_y, const char* kwargs)
{
	// Compiles the command `plt.loglog(x, y)`
	if (program.count == 0) sb_append_cstr(&program, preamble);
	if (len_x != len_y) {
		printf("ERROR: plotting arrays of different lengths.\n");
		return false;
	}
	
	if (!declare_array(x, len_x)) {
		printf("ERROR: could not allocate array into program.\n");
		exit(1);
	}
	if (!declare_array(y, len_y)) {
		printf("ERROR: could not allocate array into program.\n");
		exit(1);
	}

	sb_appendf(&program, "plt.loglog(x%zu, x%zu, %s)\n", get_array_id(x), get_array_id(y), kwargs);
	return true;
}

void _cpl_fill_between(double* x, size_t len_x, double* y1, size_t len_y1, double* y2, size_t len_y2, const char* kwargs)
{
	// Compiles the command `plt.fill_between(x, y1, y2, **kwargs)`
	
	if (len_y1 != len_y2) {
		printf("ERROR: in fill_between, arrays must have the same length, but y1 has length %zu and y2 has length %zu\n", len_y1, len_y2);
		exit(1);
	}

	if (len_y1 == 1) {
		sb_appendf(&program, "plt.fill_between(x%zu, %f, %f, %s)\n", get_array_id(x), *y1, *y2, kwargs);
	} else {
		if (len_y1 != len_x) {
			printf("ERROR: in fill_between, if y1 and y2 are arrays, then all arrays must have the same length, but x has length %zu, y1 has length %zu and y2 has length %zu\n", len_x, len_y1, len_y2);
			exit(1);
		}
		if (!declare_array(y1, len_y1)) {
			printf("ERROR: could not allocate array into program.\n");
			exit(1);
		}
		if (!declare_array(y2, len_y2)) {
			printf("ERROR: could not allocate array into program.\n");
			exit(1);
		}
		
		sb_appendf(&program, "plt.fill_between(x%zu, x%zu, x%zu, %s)\n", get_array_id(x), get_array_id(y1), get_array_id(y2), kwargs);
	}
}

void cpl_xlabel(const char* xlabel)
{
	sb_appendf(&program, "plt.xlabel(\"%s\")\n", xlabel);
}

void cpl_ylabel(const char* ylabel)
{
	sb_appendf(&program, "plt.ylabel(\"%s\")\n", ylabel);
}

void cpl_title(const char* title)
{
	sb_appendf(&program, "plt.title(\"%s\")\n", title);
}

void cpl_xlim(double x1, double x2)
{
	sb_appendf(&program, "plt.xlim([%f, %f])\n", x1, x2);
}

void cpl_ylim(double y1, double y2)
{
	sb_appendf(&program, "plt.ylim([%f, %f])\n", y1, y2);
}

void cpl_show()
{
	sb_append_cstr(&program, "plt.show()\n");
	cpl_exec_program();
	cpl_reset_program();
}

void cpl_legend()
{
	sb_append_cstr(&program, "plt.legend()\n");
}

void cpl_grid()
{
	sb_append_cstr(&program, "plt.grid()\n");
}

void cpl_savefig(const char* filename)
{
	sb_appendf(&program, "plt.savefig(\"%s\")\n", filename);
	cpl_exec_program();
}

#endif // CPLOTLIB_IMPLEMENTATION
#endif // CPLOTLIB_H