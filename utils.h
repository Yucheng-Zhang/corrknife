#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void read_data(char fn[], long np, double *p, int ncol);

void write_pc(char fn[256], int nbins0, int nbins1, int njk, double *xc);

clock_t tic();

double toc(clock_t tt);
