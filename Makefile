main: main.c utils.c utils.h pc2d.c pc2d.h ref/corr2dc.c ref/corr2dc.h
	gcc -lm -o main main.c utils.c pc2d.c ref/corr2dc.c

clean:
	rm -f main out_xc.dat out_xc_ref.dat
