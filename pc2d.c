/*
Pair count in 2D, (s,mu).
*/
#include "pc2d.h"

/* Main function. */
int pc2d(double *xc, double *p1, long np1, double *p2, long np2, double *blen,
         double *posmin, double *rlim, int nbins0, int nbins1, int ncells,
         int njk) {
  /* Parameters:
      xc: pair count to be returned;
      p1: first set of particles;
      np1: size of first dataset;
      p2: second set of particles;
      np2: size of second dataset;
      blen[3]: box length in x, y, z;
      posmin[3]: minimum corner of the box;
      rlim[4]: the range in both axis;
      nbins0,1: number of bins in both axis;
      ncells: number of cells in each dim;
      njk: number of jackknives, 0: no jackknife;
  */
  printf(">> Pair counting......\n");

  double maxrad = rlim[1];
  // get the number of cells in max radius
  int ncb[3];
  for (int i = 0; i < 3; i++) {
    ncb[i] = ceil((maxrad / blen[i]) * (double)(ncells)) + 1;
  }

  int nbins[2];
  nbins[0] = nbins0;
  nbins[1] = nbins1;

  int nattr = (njk > 0) ? 5 : 4; // number of attributes (columns)

  // mesh particles in p2 into cells
  // linked list and head used to denote cells
  long *ll = (long *)malloc(np2 * sizeof(long));
  long *hoc = (long *)malloc(ncells * ncells * ncells * sizeof(long));
  init_mesh(ll, hoc, p2, np2, nattr, ncells, blen, posmin);

  printf(">> Looping over p1...");
  clock_t tt = tic();
  // loop over particles in p1
  double pp0[3],
      bl[3]; // cache
  for (int i = 0; i < 3; i++) {
    pp0[i] = posmin[i];
    bl[i] = blen[i];
  }
  for (long i1 = 0; i1 < np1; i1++) {
    // particle one coordinates & weight
    double pp1[3];
    for (int i = 0; i < 3; i++)
      pp1[i] = p1[i1 * nattr + i];
    double wp1 = p1[i1 * nattr + 3];
    int jk1 = (njk > 0) ? p1[i1 * nattr + 4] : -1;

    // cell it belongs
    int ic[3];
    for (int i = 0; i < 3; i++) {
      ic[i] = floor((pp1[i] - pp0[i]) / bl[i] * ncells);
    }

    // cell range
    int cmm[6];
    for (int i = 0; i < 3; i++) {
      cmm[i] = ic[i] - ncb[i];
      cmm[i + 3] = ic[i] + ncb[i];
    }

    // taking care of the boundary
    for (int i = 0; i < 3; i++) {
      cmm[i] = (cmm[i] < 0) ? 0 : cmm[i];
      cmm[i + 3] = (cmm[i + 3] > ncells) ? ncells : cmm[i + 3];
    }

    // loop over cells
    for (long ix = cmm[0]; ix < cmm[3]; ix++) {
      for (long iy = cmm[1]; iy < cmm[4]; iy++) {
        for (long iz = cmm[2]; iz < cmm[5]; iz++) {
          long j = hoc[ix * ncells * ncells + iy * ncells + iz];
          if (j != -1) { // not empty cell
            while (1) {  // go through the linked list
              // particle two coordinates
              double pp2[3];
              for (int i = 0; i < 3; i++)
                pp2[i] = p2[j * nattr + i];
              double weight12 = wp1 * p2[j * nattr + 3];

              // find the bin
              int b1, b2;
              int check = find_bin(pp1, pp2, rlim, nbins, &b1, &b2);

              int idx;
              if (check != -1 && b1 < nbins[0] && b2 < nbins[1] && b1 >= 0 &&
                  b2 >= 0) {
                if (njk == 0) { // no jackknife
                  idx = b1 * nbins[1] + b2;
                  xc[idx] += weight12;
                } else { // jackknife
                  int idx = b1 * nbins[1] * (njk + 1) + b2 * (njk + 1);
                  xc[idx + njk] += weight12;
                  // jackknife region of particle 1
                  xc[idx + jk1] += weight12;
                  // jackknife region of particle 2
                  int jk2 = p2[j * nattr + 4];
                  if (jk1 != jk2)
                    xc[idx + jk2] += weight12;
                }
              }
              if (ll[j] != -1)
                j = ll[j];
              else // come to the end of cell
                break;
            }
          }
        }
      }
    }
  }
  double tt1 = toc(tt);
  printf(" %.2lf s\n", tt1);

  free(ll);
  free(hoc);

  return 0;
}

/* Mesh the particles into cells. */
void init_mesh(long *ll, long *hoc, double *p, long np, int nattr, int ncells,
               double *blen, double *posmin) {
  /* Parameters:
      ll: linked list of particles in the same cell, returned;
      hoc: head of cell (linked list ll), returned;
  */

  printf(">> Initing mesh...");
  clock_t tt = tic();
  // initialize hoc to be -1, which denotes the end of cell list
  long ncells_tot = ncells * ncells * ncells;
  memset(hoc, -1, ncells_tot * sizeof(long));

  int ic[3];
  double pp0[3], bl[3]; // cache
  for (int i = 0; i < 3; i++) {
    pp0[i] = posmin[i];
    bl[i] = blen[i];
  }

  for (long ii = 0; ii < np; ii++) {
    // the cell that the particle belongs
    for (int i = 0; i < 3; i++) {
      ic[i] = floor((p[ii * nattr + i] - pp0[i]) / bl[i] * ncells);
      // boundary
      ic[i] = (ic[i] == ncells) ? ic[i] - 1 : ic[i];
    }

    long idx = ic[0] * ncells * ncells + ic[1] * ncells + ic[2];
    ll[ii] = hoc[idx];
    hoc[idx] = ii;
  }

  double tt1 = toc(tt);
  printf(" %.2lf s\n", tt1);
}

/* Find the bin that the pair belongs. */
int find_bin(double *pp1, double *pp2, double *rlim, int *nbins, int *b1,
             int *b2) {
  /* Parameters:
      b1,2: bin index to be returned.
  */
  double dxyz[3], dmag = 0;
  double losxyz[3], losmag = 0;
  double ddlos = 0;
  for (int i = 0; i < 3; i++) {
    dxyz[i] = pp1[i] - pp2[i];
    losxyz[i] = 0.5 * (pp1[i] + pp2[i]);
  }
  for (int i = 0; i < 3; i++) { // vectorzied ?
    dmag += dxyz[i] * dxyz[i];
    losmag += losxyz[i] * losxyz[i];
    ddlos += dxyz[i] * losxyz[i];
  }
  dmag = sqrt(dmag); // r
  losmag = sqrt(losmag);
  double mu = fabs(ddlos / dmag / losmag); // |cos(theta)|

  // find bin
  if (dmag > rlim[0] && dmag < rlim[1] && mu > rlim[2] && mu < rlim[3]) {
    int rbin = floor((dmag - rlim[0]) * nbins[0] / (rlim[1] - rlim[0]));
    int mubin = floor((mu - rlim[2]) * nbins[1] / (rlim[3] - rlim[2]));
    *b1 = (rbin == nbins[0]) ? rbin - 1 : rbin;
    *b2 = (mubin == nbins[1]) ? mubin - 1 : mubin;
    return 0;
  } else {
    *b1 = -1;
    *b2 = -1;
    return -1;
  }
  //  return -1;
}
