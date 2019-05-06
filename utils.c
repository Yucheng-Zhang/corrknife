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
    for (int j = 0; j < njk; j++) {
      fprintf(fp, " %.8e", xc[i * njk + j]);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}
