#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void read_data(char fn[], long np, double *p, int ncol);

void write_pc(char fn[256], int nbins0, int nbins1, int njk, double *xc);

clock_t tic() {
  clock_t tt = clock();
  return tt;
}

double toc(clock_t tt) {
  clock_t tt1;
  return (tt1 - tt) / CLOCKS_PER_SEC;
}
