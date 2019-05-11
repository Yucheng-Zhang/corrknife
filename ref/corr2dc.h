#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void corr2d(double *xc_2d, double *p1, long fp1, long np1, double *p2, long fp2,
            long np2, double *rlim, int nbins0, int nbins1, int nhocells,
            double *blen, double *posmin, int samp, int njn, int pbc, int los,
            int interactive);
