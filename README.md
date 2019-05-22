# CorrKnife

Parallel computing of galaxy correlation function with jackknife - High Performance Computing class final project.

- `Yucheng Zhang`, `Kaizhe Wang`

## Description

For this final project, we are doing the estimation of correlation function of galaxies, which is one of the most important applications of High Performance Computing in Cosmology. The most computation intensive part is to calculate the distance between two sets of points and fit them into proper bins.

## Data Format

 - The input data and random files should have 5 (or 4 if no jackknife) columns, `x y z weight jackknife`.

## Run

- The code is parallelized with `MPI` and `OpenMP`.
- Check the parameters in `run.sh` before running, where the default parameters are for example data files in `data/`.

## Acknowledgement

- This project is based on the mesh algorithm in Shadab Alam's Correlation Function code https://gitlab.com/shadaba/CorrelationFunction.
