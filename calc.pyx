'''
Wrap of C code.
'''
import numpy as np
cimport numpy as np

cdef extern from "./pc2d.h" nogil:
    void pc2d(double *xc, double *p1, long np1, double *p2, long np2, double *blen,
          double *posmin, double *rlim, int nbins0, int nbins1, int ncells, int njk);


def pc2dpy(np.ndarray[np.double_t, ndim=2] p1, np1,
           np.ndarray[np.double_t, ndim=2] p2, np2,
           np.ndarray[np.double_t, ndim=1] blen,
           np.ndarray[np.double_t, ndim=1] posmin,
           np.ndarray[np.double_t, ndim=1] rlim,
           nbins0, nbins1, ncells, njk):
    
    cdef np.ndarray[np.double_t, ndim=2] xc = np.zeros((nbins0, nbins1),
                                                        dtype=np.float64)
    cdef np.ndarray[np.double_t, ndim=3] xc_jk = np.zeros((nbins0, nbins1, njk+1),
                                                            dtype=np.float64)

    if (njk == 0):
        pc2d(<double*> xc.data, <double*> p1.data, np1, <double*> p2.data, np2, <double*> blen.data,
             <double*> posmin.data, <double*> rlim.data, nbins0, nbins1, ncells, njk)

        return xc
    
    else:
        pc2d(<double*> xc_jk.data, <double*> p1.data, np1, <double*> p2.data, np2, <double*> blen.data,
             <double*> posmin.data, <double*> rlim.data, nbins0, nbins1, ncells, njk)

        return xc_jk
