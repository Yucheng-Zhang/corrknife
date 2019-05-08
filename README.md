# CorrKnife

Correlation function with jackknife - High Performance Computing class final project.

- `Yucheng Zhang`, `Kaizhe Wang`

## Description

For this final project, we will do the estimation of correlation function of galaxies, which is one of the most important applications of High Performance Computing in Cosmology. The most computation intensive part is to calculate the distance between two sets of points and fit them into proper bins. The things we plan to do include,

- Figure out the mesh algorithm. Naive pair count requires `O(N^2)` time complexity, where `N` is the size of the data. With the mesh algorithm, we hope to reduce the time complexity to `O(NlogN)`.
- Figure out the way to jackknife resampling in the code. The input data points are divided into `njk` parts, where `njk` stands for the number of jackknife regions. For more information about jackknife resampling, see https://en.wikipedia.org/wiki/Jackknife_resampling.
- Parallelize the code with `OpenMP` and `MPI` in `C`.
- Wrap the parallelized `C` with `Python`, which works better in loading data and plotting results.

## Notes

## Acknowledgement

- This project is based on Shadab Alam's Correlation Function code https://gitlab.com/shadaba/CorrelationFunction.
