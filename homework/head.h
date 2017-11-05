
#ifndef __PROTECT_HEAD_H
#define __PROTECT_HEAD_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int iter_1(float* x0 ,float* x1 ,float* y1 ,int n ,int f, int K);
int iter_2(float* x0 ,float* x1 ,float* y1 ,float* y2, int n ,int f, int g);
void draw(float*x01,float*x02,float*x03,float*x04,float*x05,float*x06,										float*x07,float*x08,int n);
float* x_init(int n);
#endif

