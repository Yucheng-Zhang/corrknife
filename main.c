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

  int c_ref = 0;

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
  sscanf(argv[20], "%d", &c_ref);

  /* init MPI */
  int mpirank, mpisize;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpisize);

  double tt; // for timing

  /* load data */
  double *p1 = (double *)malloc(np1 * ncol * sizeof(double));
  double *p2 = (double *)malloc(np2 * ncol * sizeof(double));
  if (mpirank == 0)
    printf(">> Loading data files...\n");
  tt = MPI_Wtime();
  read_data(fdata, np1, p1, ncol);
  read_data(frand, np2, p2, ncol);
  if (mpirank == 0) {
    printf("++ Data file: %s\n", fdata);
    printf("++ Rand file: %s\n", frand);
    printf(":: Time elapsed: %f s \n", MPI_Wtime() - tt);
  }

  /* prepare parameters */
  double blen[3], posmin[3];
  if (mpirank == 0)
    printf(">> Box corner with length:\n");
  for (int i = 0; i < 3; i++) {
    blen[i] = brange[i + 3] - brange[i];
    posmin[i] = brange[i];
    if (mpirank == 0)
      printf(": %lf, %lf\n", posmin[i], blen[i]);
  }

  /* get start and end index of p1 */
  long spi = mpirank * (np1 / mpisize);
  long epi = (mpirank + 1) * (np1 / mpisize); // not included
  if (mpirank == mpisize - 1)
    epi = np1;

  /* run */
  long size_xc = nbins0 * nbins1 * (njk + 1);
  double *xc = (double *)calloc(size_xc, sizeof(double));
  MPI_Barrier(MPI_COMM_WORLD);
  if (mpirank == 0)
    printf(">> Pair couting...\n");
  tt = MPI_Wtime();
  pc2d(xc, p1, np1, p2, np2, blen, posmin, rlim, nbins0, nbins1, ncells, njk,
       spi, epi);
  MPI_Barrier(MPI_COMM_WORLD);

  /* gather results */
  double *xc_g = NULL;
  if (mpirank == 0)
    xc_g = (double *)calloc(size_xc, sizeof(double));
  MPI_Reduce(xc, xc_g, size_xc, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  /* write pair count to file */
  if (mpirank == 0) {
    printf(":: Time elapsed: %f s\n", MPI_Wtime() - tt);
    write_pc("out_xc.dat", nbins0, nbins1, njk, xc_g);
    printf("== Output file: out_xc.dat\n");
  }

  /* reference */
  if (mpirank == 0 && c_ref) {
    printf(">> Computing reference results...\n");
    double *xc_r =
        (double *)calloc(nbins0 * nbins1 * (njk + 1), sizeof(double));
    tt = MPI_Wtime();
    corr2d(xc_r, p1, 0, np1, p2, 0, np2, rlim, nbins0, nbins1, ncells, blen,
           posmin, 0, njk, 0, 0, 0);
    tt = MPI_Wtime() - tt;
    printf(":: Time elapsed: %f s\n", tt);
    write_pc("out_xc_ref.dat", nbins0, nbins1, njk, xc_r);
    printf("== Output file: out_xc_ref.dat\n");
  }

  /* free memory */
  free(xc);
  free(p1);
  free(p2);

  MPI_Finalize();
  return 0;
}
