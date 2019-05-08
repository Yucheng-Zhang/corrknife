#include "pc2d.h"
#include "utils.h"

/* Main function. */
int main(int argc, char *argv[]) {

  /* parse cmd arguments */
  char fdata[256], frand[256];
  long np1, np2;      // size of data and rand sample
  int ncol;           // number of columns
  int nbins0, nbins1; // number of bins
  int njk;            // number of jackknives
  int ncells;         // number of cells
  double brange[6];   // range of x,y,z
  double rlim[4];

  sscanf(argv[1], "%s", fdata);
  sscanf(argv[2], "%ld", &np1);
  sscanf(argv[3], "%s", frand);
  sscanf(argv[4], "%ld", &np2);
  sscanf(argv[5], "%d", &ncol);
  sscanf(argv[6], "%d", &nbins0);
  sscanf(argv[7], "%d", &nbins1);
  sscanf(argv[8], "%d", &njk);
  sscanf(argv[9], "%d", &ncells);
  sscanf(argv[10], "%lf", &brange[0]);
  sscanf(argv[11], "%lf", &brange[3]);
  sscanf(argv[12], "%lf", &brange[1]);
  sscanf(argv[13], "%lf", &brange[4]);
  sscanf(argv[14], "%lf", &brange[2]);
  sscanf(argv[15], "%lf", &brange[5]);
  sscanf(argv[16], "%lf", &rlim[0]);
  sscanf(argv[17], "%lf", &rlim[1]);
  sscanf(argv[18], "%lf", &rlim[2]);
  sscanf(argv[19], "%lf", &rlim[3]);

  /* load data */
  double *p1 = (double *)malloc(np1 * ncol * sizeof(double));
  double *p2 = (double *)malloc(np2 * ncol * sizeof(double));
  printf(">> Loading file: %s\n", fdata);
  read_data(fdata, np1, p1, ncol);
  printf(">> Loading file: %s\n", frand);
  read_data(frand, np2, p2, ncol);

  /* prepare parameters */
  double blen[3], posmin[3];
  printf(">> Box corner with length:\n");
  for (int i = 0; i < 3; i++) {
    blen[i] = brange[i + 3] - brange[i];
    posmin[i] = brange[i];
    printf(":: %lf, %lf\n", posmin[i], blen[i]);
  }

  /* run */
  double *xc = (double *)calloc(nbins0 * nbins1 * (njk + 1), sizeof(double));
  pc2d(xc, p1, np1, p2, np2, blen, posmin, rlim, nbins0, nbins1, ncells, njk);

  /* write pair count to file */
  write_pc("out_xc.dat", nbins0, nbins1, njk, xc);

  return 0;
}
