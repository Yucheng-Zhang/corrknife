#include "utils.h"

/* Read data from file. */
void read_data(char fn[256], long np, double *p, int ncol) {
  // read in file
  FILE *fp = fopen(fn, "r");
  for (long i = 0; i < np; i++) {
    for (int j = 0; j < ncol; j++) {
      fscanf(fp, "%lf", &p[i * ncol + j]);
    }
  }
  fclose(fp);
}

/* Write pair count to file. */
void write_pc(char fn[256], int nbins0, int nbins1, int njk, double *xc) {
  FILE *fp = fopen(fn, "w");
  for (int i = 0; i < nbins0 * nbins1; i++) {
    double xc_tot = xc[i * (njk + 1) + njk];
    fprintf(fp, "%.8e", xc_tot);
    for (int j = 0; j < njk; j++) {
      fprintf(fp, " %.8e", xc_tot - xc[i * (njk + 1) + j]);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}

/* Timer */
clock_t tic() {
  clock_t tt = clock();
  return tt;
}

double toc(clock_t tt) {
  clock_t tt1;
  return (tt1 - tt) / CLOCKS_PER_SEC;
}
