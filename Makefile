main: main.c utils.c utils.h pc2d.c pc2d.h
	gcc -lm -o main main.c utils.c pc2d.c

clean:
	rm -f main out_xc.dat
