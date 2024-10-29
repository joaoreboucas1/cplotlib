#include <Python.h>
#include <stdlib.h>
#include <string.h>

#define _SUCCESS_ 0
#define _ERROR_ 1

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

int declare_array(float* x, size_t x_len, const char* ident)
{
	// Constructs the command `ident = np.array([x[0], x[1], ...])`.
	// This Python line declares a Numpy array from the values in your C array.
	// Also appends the command into the program.
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
	// free(values);
	return _SUCCESS_;
}

int _cpl_plot(float* x, size_t len_x, float* y, size_t len_y)
{
	if (program_count == 0) append_cmd(preamble);
	if (len_x != len_y) {
		printf("ERROR: plotting arrays of different lengths.\n");
		return _ERROR_;
	}
	
	int error = declare_array(x, len_x, "x");
	if (error) {
		printf("ERROR: could not allocate array into command.\n");
		exit(1);
	}
	int error2 = declare_array(y, len_y, "y");
	if (error2) {
		printf("ERROR: could not allocate array into command.\n");
		exit(1);
	}
	const char* epilog = "plt.plot(x, y)\n";
	append_cmd(epilog);
	return _SUCCESS_;
}

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

void cpl_show()
{
	append_cmd("plt.show()\n");
	exec_program();
	reset_program();
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
