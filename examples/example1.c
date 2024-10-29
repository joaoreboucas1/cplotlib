#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cplotlib.h"

int main()
{
    const size_t N = 100;
    float x[N];
    float y[N];
    const float x_min = -10.0f;
    const float x_max = 10.0f;
    const float dx = (x_max - x_min)/N;
    for (size_t i = 0; i < 100; i++) {
		x[i] = x_min + i*dx;
		y[i] = expf(-x[i]*x[i]);
    }
    _cpl_plot(x, N, y, N);
    cpl_xlabel("$x$");
    cpl_ylabel("$y$");
    cpl_savefig("plot.pdf");
	
    return 0;
}