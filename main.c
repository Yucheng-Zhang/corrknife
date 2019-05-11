#include "pc2d.h"
#include "ref/corr2dc.h"
#include "utils.h"
#include <mpi.h>

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

  /* init MPI */
  int mpirank, mpisize;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpisize);

  /* get start and end index of p1 */
  long spi = mpirank * (np1 / mpisize);
  long epi = (mpirank + 1) * (np1 / mpisize); // not included
  if (mpirank == mpisize - 1)
    epi = np1;

  /* run */
  long size_xc = nbins0 * nbins1 * (njk + 1);
  double *xc = (double *)calloc(size_xc, sizeof(double));
  pc2d(xc, p1, np1, p2, np2, blen, posmin, rlim, nbins0, nbins1, ncells, njk,
       spi, epi);
  MPI_Barrier(MPI_COMM_WORLD);

  /* gather results */
  double *xc_g;
  if (mpirank == 0)
    xc_g = (double *)calloc(size_xc, sizeof(double));
  MPI_Allreduce(&xc, &xc_g, size_xc, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  /* write pair count to file */
  if (mpirank == 0)
    write_pc("out_xc.dat", nbins0, nbins1, njk, xc_g);

  /* reference */
  if (mpirank == 0) {
    printf(">> Computing reference results...\n");
    double *xc_r =
        (double *)calloc(nbins0 * nbins1 * (njk + 1), sizeof(double));
    clock_t tt = tic();
    corr2d(xc_r, p1, 0, np1, p2, 0, np2, rlim, nbins0, nbins1, ncells, blen,
           posmin, 0, njk, 0, 0, 0);
    double tt1 = toc(tt);
    printf(":: Time: %.6lf s\n", tt1);
    write_pc("out_xc_ref.dat", nbins0, nbins1, njk, xc_r);
  }

  /* free memory */
  free(xc);
  free(p1);
  free(p2);
  return 0;
}
