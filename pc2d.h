#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_mesh(long *ll, long *hoc, double *p, long np, int nattr, int ncells,
               double *blen, double *posmin);

int find_bin(double *pp1, double *pp2, double *rlim, int *nbins, int *b1,
             int *b2);

void pc2d(double *res, double *p1, long np1, double *p2, long np2, double *blen,
          double *posmin, double *rlim, int nbins0, int nbins1, int ncells,
          int njk);
