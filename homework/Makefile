
main:test
	mpirun -np 8 ./test  
test:main.c iter_1.c iter_2.c head.h draw.c x_init.c
		mpicc -o test main.c iter_1.c iter_2.c head.h draw.c x_init.c -g -Wall -lm

