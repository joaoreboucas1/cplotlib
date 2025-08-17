#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define CPLOTLIB_IMPLEMENTATION
#include "cplotlib.h"

int main()
{
    const size_t N = 200;
    double x[N];
    double y[N];
    double z[N];
    const double x_min = 1.0f;
    const double x_max = 40.0f;
    const double dx = (x_max - x_min)/N;
    for (size_t i = 0; i < N; i++) {
        x[i] = x_min + i*dx;
        y[i] = x[i]*x[i];
        z[i] = expf(pow(sinf(x[i]), 2));
    }
    cpl_loglog(x, y, "color='tab:green', ls='--', label='Data 1'");
    cpl_loglog(x, z, "color='tab:blue', ls='--', label='Data 2'");
    cpl_xlabel("$x$");
    cpl_ylabel("$y$");
    double y_1[1] = {1.0};
    double y_2[1] = {10.0};
    cpl_fill_between(x, y_1, y_2, "color='gray', alpha=0.6");
    cpl_fill_between(x, y, z, "color='tab:red', alpha=0.4");
    cpl_title("Hello from C!");
    cpl_xlim(x[0], x[N-1]);
    cpl_ylim(0.1f, 1e5f);
    cpl_legend();
    cpl_grid();
    cpl_savefig("plot.pdf");
	cpl_exec_program();
    return 0;
}