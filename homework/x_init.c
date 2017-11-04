
#include"head.h"

float* x_init(int n)
{
	int i;
	float *x0;
	x0 = (float *) malloc(sizeof(float)*n);
	for(i=0;i<n;i++)
	{
		x0[i] = 0.0;
	}


return x0;

}

