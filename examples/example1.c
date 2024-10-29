#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cplotlib.h"

typedef struct {
  float alpha;
  const char* ls;
  const char* color;
} Style;

int main()
{
    const size_t N = 150;
    float x[N];
    float y[N];
    const float x_min = -5.0f;
    const float x_max = 5.0f;
    const float dx = (x_max - x_min)/N;
    for (size_t i = 0; i < N; i++) {
		x[i] = x_min + i*dx;
		y[i] = expf(-x[i]*x[i]);
    }
    _cpl_plot(x, N, y, N, "color='tab:green', ls='--', label='Data'");
    cpl_xlabel("$x$");
    cpl_ylabel("$y$");
    cpl_legend();
    cpl_grid();
    cpl_savefig("plot.pdf");
	
    return 0;
}