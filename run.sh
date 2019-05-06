#!/bin/bash

fdata=data/data.txt
np1=38589
frand=data/randoms.txt
np2=384977
ncol=5

nbins0=32
nbins1=100

njk=64
ncells=200

xmin=-1597
xmax=1597
ymin=-1558
ymax=1538
zmin=-544
zmax=1587

rlim0=0
rlim1=160

./main $fdata $np1 $frand $np2 $ncol $nbins0 $nbins1 $njk $ncells \
       $xmin $xmax $ymin $ymax $zmin $zmax $rlim0 $rlim1