
#include"head.h"

void draw(float*x01,float*x02,float*x03,float*x04,float*x05,float*x06,										float*x07,float*x08,int n)
{
	float hy = 1.0/n;
	float h = sqrt(3)/3;
	float hx = h/n;
	float x,y,x1,x2,x3,x4,y1,y2,y3,y4,x5,x6,x7,x8,y5,y6,y7,y8;
	int i,j,k;
	x1 = hx/2 + h;
	y1 = 1 - hy/2;
	x2 = hx/2 + h;
	y2 = hy/2 - 1;
	x3 = -hx/2 - h;
	y3 = 1 - hy/2;
	x4 = -hx/2 - h;
	y4 = hy/2 - 1;
	x5 = hx/2;
	y5 = 1 - hy/2;
	x6 = hx/2;
	y6 = hy/2 - 1;
	x7 = -hx/2;
	y7 = 1 - hy/2;
	x8 = -hx/2;
	y8 = hy/2 - 1;

//MPI_FILE fp;	
//MPI_File_open(MPI_COMM_WORLD,"data",MPI_MODE_RDONLY,MPI_INFO_NULL,&fh);/
FILE *fp;
fp = fopen("/home/hh/helloworld/homework1/homework/hh_draw1.csv","w");

//MPI_File_open(MPI_COMM_WORLD,"data",MPI_MODE_RDONLY,MPI_INFO_NULL,&fh);

	for(i=1;i<=n;i++)
	{	
		y = (i-1)*hy;
		k = i*(i-1)/2;
		for(j=1;j<=i;j++)
		{
			x = (j-1)*hx;
			k = k+1;
	//		MPI_File_write_at_all(fh,0,&n,3,MPI_float,&status);
			fprintf(fp,"%.6f %.6f %.6f\n",x1+x,y1-y,x01[k]);
			fprintf(fp,"%.6f %.6f %.6f\n",x2+x,y2+y,x02[k]);
			fprintf(fp,"%.6f %.6f %.6f\n",x3-x,y3-y,x03[k]);
			fprintf(fp,"%.6f %.6f %.6f\n",x4-x,y4+y,x04[k]);	
		
//			fwrite(fp,"%.6f %.6f %.6f\n",x1+x,y1-y,x01[k]);
//			fwrite(fp,"%.6f %.6f %.6f\n",x2+x,y2+y,x02[k]);
//			fwrite(fp,"%.6f %.6f %.6f\n",x3-x,y3-y,x03[k]);
//			fwrite(fp,"%.6f %.6f %.6f\n",x4-x,y4+y,x04[k]);	
		}
		k = i*n;
		for(j=1;j<=n;j++)
		{
			k = k+1;
			x = (j-1)*hx;
			fprintf(fp,"%.6f %.6f %.6f\n",x5+x,y5-y,x05[k]);
			fprintf(fp,"%.6f %.6f %.6f\n",x6+x,y6+y,x06[k]);
			fprintf(fp,"%.6f %.6f %.6f\n",x7-x,y7-y,x07[k]);
			fprintf(fp,"%.6f %.6f %.6f\n",x8-x,y8+y,x08[k]);



		}

	}

	for(j=1;j<=n;j++)
	{	
		y = -hy;
		x = (j-1)*hx;
		fprintf(fp,"%.6f %.6f %.6f\n",x5+x,y5-y,x05[j]);
		fprintf(fp,"%.6f %.6f %.6f\n",x6+x,y6+y,x06[j]);
		fprintf(fp,"%.6f %.6f %.6f\n",x7-x,y7-y,x07[j]);
		fprintf(fp,"%.6f %.6f %.6f\n",x8-x,y8+y,x08[j]);

	}

}

