#include <Python.h>
#include <stdlib.h>
#include <string.h>

#define _SUCCESS_ 0
#define _ERROR_ 1

#define ARRAY_LEN(x) sizeof((x))/sizeof((x)[0])

// Program is a string that accumulates Python code
#define PROGRAM_CAPACITY 10000
char program[PROGRAM_CAPACITY];
size_t program_count = 0;

static inline void print_program() 
{
	// Prints the program for debug purposes
	printf("%s\n", program);
}

void exec_program()
{
	// Executes a string as a Python program
	Py_Initialize();
	PyRun_SimpleString(program);
	Py_Finalize();
}

static inline int append_cmd(const char* command)
{
	// This function accumulates strings into `program`
	const size_t cmd_len = strlen(command);
	if (program_count + cmd_len > PROGRAM_CAPACITY) return _ERROR_;
	strcat(program, command);
	program_count += strlen(command);
	return _SUCCESS_;
}

const char* preamble = "\
import numpy as np\n\
import matplotlib.pyplot as plt\n\
";

static inline void reset_program()
{
	// Resets the program state to only the preamble
	program[0] = '\0';
	program_count = 0;
	append_cmd(preamble);
}

typedef struct {
	float* p;
	const char* ident;
} cpl_array;
#define ARRAY_CAPACITY 20
cpl_array arrays[ARRAY_CAPACITY];
size_t array_count;

static inline size_t get_array_index(float* p)
{
	for (size_t i = 0; i < array_count; i++) {
		if (arrays[i].p == p) return i;
	}
	return 0;
}

int declare_array(float* x, size_t x_len)
{
	// Constructs the command `ident = np.array([x[0], x[1], ...])`.
	// This Python line declares a Numpy array from the values in your C array.
	// Also appends the command into the program.
	for (size_t i = 0; i < array_count; i++) {
		float* p = arrays[i].p;
		if (x == p) return _SUCCESS_;
	}
	char ident[10];
	sprintf(ident, "x%zu", array_count);
	const char* prefix = " = np.array([";
	const char* suffix = "])\n";
	// Format .4g => 2.3412e-12\w, => 12 characters
	const char char_buf_size = 12;
	char value[char_buf_size];
	const size_t buf_size = char_buf_size*x_len;
	if (program_count + buf_size > PROGRAM_CAPACITY) {
		return _ERROR_;
	}
	char* values = (char*) malloc(buf_size*sizeof(char));
	for (size_t i = 0; i < x_len; i++) {
		if (i == x_len - 1) {
			sprintf(value, "%.4g", x[i]);
		} else{
			sprintf(value, "%.4g, ", x[i]);
		}
		strcat(values, value);
	}
	append_cmd(ident);
	append_cmd(prefix);
	append_cmd(values);
	append_cmd(suffix);
	arrays[array_count] = (cpl_array) { .p = x, .ident = ident };
	array_count += 1;
	// free(values);
	return _SUCCESS_;
}

int _cpl_plot(float* x, size_t len_x, float* y, size_t len_y, const char* kwargs)
{
	if (program_count == 0) append_cmd(preamble);
	if (len_x != len_y) {
		printf("ERROR: plotting arrays of different lengths.\n");
		return _ERROR_;
	}
	
	int error = declare_array(x, len_x);
	if (error) {
		printf("ERROR: could not allocate array into command.\n");
		exit(1);
	}
	int error2 = declare_array(y, len_y);
	if (error2) {
		printf("ERROR: could not allocate array into command.\n");
		exit(1);
	}
	size_t index_x = get_array_index(x);
	size_t index_y = get_array_index(y);
	char ident_x[12];
	char ident_y[12];
	sprintf(ident_x, "x%zu", index_x);
	sprintf(ident_y, "x%zu", index_y);

	const size_t kwargs_size = strlen(kwargs);
	char epilog[kwargs_size + 17];
	sprintf(epilog, "plt.plot(%s, %s, %s)\n", ident_x, ident_y, kwargs);
	append_cmd(epilog);
	return _SUCCESS_;
}

int _cpl_loglog(float* x, size_t len_x, float* y, size_t len_y, const char* kwargs)
{
	if (program_count == 0) append_cmd(preamble);
	if (len_x != len_y) {
		printf("ERROR: plotting arrays of different lengths.\n");
		return _ERROR_;
	}
	
	int error = declare_array(x, len_x);
	if (error) {
		printf("ERROR: could not allocate array into command.\n");
		exit(1);
	}
	int error2 = declare_array(y, len_y);
	if (error2) {
		printf("ERROR: could not allocate array into command.\n");
		exit(1);
	}
	size_t index_x = get_array_index(x);
	size_t index_y = get_array_index(y);
	char ident_x[12];
	char ident_y[12];
	sprintf(ident_x, "x%zu", index_x);
	sprintf(ident_y, "x%zu", index_y);

	const size_t kwargs_size = strlen(kwargs);
	char epilog[kwargs_size + 17];
	sprintf(epilog, "plt.loglog(%s, %s, %s)\n", ident_x, ident_y, kwargs);
	append_cmd(epilog);
	return _SUCCESS_;
}
#define cpl_plot(x, y, kwargs) _cpl_plot(x, ARRAY_LEN((x)), y, ARRAY_LEN((y)), kwargs)
#define cpl_loglog(x, y, kwargs) _cpl_loglog(x, ARRAY_LEN((x)), y, ARRAY_LEN((y)), kwargs)

void _cpl_fill_between(float* x, size_t len_x, float* y1, size_t len_y1, float* y2, size_t len_y2, const char* kwargs)
{
	(void) &x;
	const size_t kwargs_size = strlen(kwargs);
	char epilog[kwargs_size + 50];

	size_t index_x = get_array_index(x);
	char ident_x[12];
	sprintf(ident_x, "x%zu", index_x);
	
	if (len_y1 != len_y2) {
		printf("ERROR: in fill_between, arrays must have the same length, but y1 has length %zu and y2 has length %zu\n", len_y1, len_y2);
		exit(1);
	}

	if (len_y1 == 1) {
		sprintf(epilog, "plt.fill_between(%s, %f, %f, %s)\n", ident_x, *y1, *y2, kwargs);
	} else {
		if (len_y1 != len_x) {
			printf("ERROR: in fill_between, if y1 and y2 are arrays, then all arrays must have the same length, but x has length %zu, y1 has length %zu and y2 has length %zu\n", len_x, len_y1, len_y2);
			exit(1);
		}
		int error = declare_array(y1, len_y1);
		if (error) {
			printf("ERROR: could not allocate array into command.\n");
			exit(1);
		}
		error = declare_array(y2, len_y2);
		if (error) {
			printf("ERROR: could not allocate array into command.\n");
			exit(1);
		}
		size_t index_y1 = get_array_index(y1);
		size_t index_y2 = get_array_index(y2);
		char ident_y1[12];
		char ident_y2[12];
		sprintf(ident_y1, "x%zu", index_y1);
		sprintf(ident_y2, "x%zu", index_y2);
		sprintf(epilog, "plt.fill_between(%s, %s, %s, %s)\n", ident_x, ident_y1, ident_y2, kwargs);
	}
	append_cmd(epilog);
}
#define cpl_fill_between(x, y1, y2, kwargs) _cpl_fill_between((x), ARRAY_LEN((x)), (y1), ARRAY_LEN((y1)), (y2), ARRAY_LEN((y2)), kwargs)

void cpl_xlabel(const char* xlabel)
{
	const size_t xlabel_size = strlen(xlabel);
	char xlabel_cmd[xlabel_size + 13];
	sprintf(xlabel_cmd, "plt.xlabel('%s')\n", xlabel);
	append_cmd(xlabel_cmd);
}

void cpl_ylabel(const char* ylabel)
{
	const size_t ylabel_size = strlen(ylabel);
	char ylabel_cmd[ylabel_size + 13];
	sprintf(ylabel_cmd, "plt.ylabel('%s')\n", ylabel);
	append_cmd(ylabel_cmd);
}

void cpl_title(const char* title)
{
	const size_t title_size = strlen(title);
	char title_cmd[title_size + 13];
	sprintf(title_cmd, "plt.title('%s')\n", title);
	append_cmd(title_cmd);
}

void cpl_xlim(float x1, float x2)
{
	char xlim_cmd[35];
	sprintf(xlim_cmd, "plt.xlim([%f, %f])\n", x1, x2);
	append_cmd(xlim_cmd);
}

void cpl_ylim(float y1, float y2)
{
	char ylim_cmd[35];
	sprintf(ylim_cmd, "plt.ylim([%f, %f])\n", y1, y2);
	append_cmd(ylim_cmd);
}

void cpl_show()
{
	append_cmd("plt.show()\n");
	exec_program();
	reset_program();
}

void cpl_legend()
{
	append_cmd("plt.legend()\n");
}

void cpl_grid()
{
	append_cmd("plt.grid()\n");
}

void cpl_savefig(const char* filename)
{
	const size_t filename_size = strlen(filename);
	char save_cmd[filename_size + 15];
	sprintf(save_cmd, "plt.savefig('%s')\n", filename);
	append_cmd(save_cmd);
	exec_program();
	reset_program();
}
