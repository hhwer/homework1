
#include "head.h"

int main(int argc, char ** argv)
{
	int n;
	if(argc==1)
	{	
		n = 4;	
	}
	else	
		n = atoi(argv[1]); 
	
	MPI_Status status;
	float* myRows[8],*downRows[8],*leftRows[8],*rightRows[8];
//	float* x01,*x02,*x03,*x04,*x05,*x06,*x07,*x08;
//	float* x11,*x12,*x13,*x14,*x15,*x16,*x17,*x18;
//	float* y11,*y12,*y13,*y14,*y15,*y16,*y17,*y18;
//	float* y25,*y26,*y27,*y28;
	int Nrec = n*n;
	int Ntri = n*(n-1)/2;
	int i,j,k,total,myid;
	float epi = 0;
	int ite,rank ;
	int para1[] = {0,-5,-2,3,-1,-4,-1,-4};	
	int para2[] = {1,1,1,1,0,-5,-2,-3};	


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

//  x01 = (float *) malloc(sizeof(float)*(n*(n+1)/2+1));
//  x02 = (float *) malloc(sizeof(float)*(n*(n+1)/2+1));
//  x03 = (float *) malloc(sizeof(float)*(n*(n+1)/2+1));	
//  x04 = (float *) malloc(sizeof(float)*(n*(n+1)/2+1));
//  x05 = (float *) malloc(sizeof(float)*(n*(n+1)+1));
//  x06 = (float *) malloc(sizeof(float)*(n*(n+1)+1));
//  x07 = (float *) malloc(sizeof(float)*(n*(n+1)+1));	
//  x08 = (float *) malloc(sizeof(float)*(n*(n+1)+1));

// 初始化
	for(i=0;i<4;i++)
	{	
		if(myid == i)
		myRows[i] = x_init(n*(n+1)/2+1);
	}

	for(i=4;i<8;i++)
	{
		if(myid == i)
		{
			myRows[i] = x_init(n*(n+1));
			rightRows[i] = x_init(n+1);
		}
	}

	for(i=0;i<8;i++)
	{
		downRows[i] = x_init(n+1);
		leftRows[i] = x_init(n+1);
	}

//	x01 = x_init(n*(n+1)/2+1);
//	x02 = x_init(n*(n+1)/2+1);
//	x03 = x_init(n*(n+1)/2+1);
//	x04 = x_init(n*(n+1)/2+1);
//	x05 = x_init(n*(n+1)+1);
//	x06 = x_init(n*(n+1)+1);
//	x07 = x_init(n*(n+1)+1);
//	x08 = x_init(n*(n+1)+1);
//
//	x11 = x_init(n+1);
//	x12 = x_init(n+1);
//	x13 = x_init(n+1);	
//	x14 = x_init(n+1);
//	x15 = x_init(n+1);
//	x16 = x_init(n+1);
//	x17 = x_init(n+1);	
//	x18 = x_init(n+1);
//	y11 = x_init(n+1);
//	y12 = x_init(n+1);
//	y13 = x_init(n+1);	
//	y14 = x_init(n+1);
//	y15 = x_init(n+1);
//	y16 = x_init(n+1);
//	y17 = x_init(n+1);	
//	y18 = x_init(n+1);
//	y25 = x_init(n+1);
//	y26 = x_init(n+1);
//	y27 = x_init(n+1);	
//	y28 = x_init(n+1);
	


	
	for(ite=0;ite<50000;ite++)
	{

		//计算
		for(rank=0;rank<4;rank++)
		{
			if(myid == rank)
				iter_1(myRows[rank],downRows[rank],leftRows[rank],n,								para1[rank],para2[rank]);
		}

		for(rank=4;rank<8;rank++)
		{
			if(myid == rank)
				iter_2(myRows[rank],downRows[rank],leftRows[rank],								rightRows[rank],n,para1[rank],para2[rank]);
		}

		//传递数据
		
		for(i=1;i<n;i++)
		{
			j = Ntri+i;
			k = Nrec+i;
			for(rank=0;rank<8;rank=rank+2)
			{	
				if(myid==rank)
				{
					downRows[rank+1][i] = myRows[rank][j];
					MPI_Send(&downRows[rank+1][i], 8, MPI_FLOAT, myid+1, 0									, MPI_COMM_WORLD);
					MPI_Recv(&downRows[rank][i], 8, MPI_FLOAT, myid+1									, 0, MPI_COMM_WORLD, &status);

				}
			}
		
			for(rank=1;rank<8;rank=rank+2)
			{	
				if(myid==rank)
				{
					downRows[rank-1][i] = myRows[rank][j];

					MPI_Send(&downRows[rank-1][i], 8, MPI_FLOAT, myid-1, 0									, MPI_COMM_WORLD);
					MPI_Recv(&downRows[rank][i], 8, MPI_FLOAT, myid-1									, 0, MPI_COMM_WORLD, &status);

				}
			}
			
			j = i*n+n;
			k = i*(i-1)/2+1;

			for(rank=0;rank<4;rank++)
			{
				if(myid==rank)
				{
					rightRows[rank+4][i]=myRows[rank][j];

					MPI_Send(&rightRows[rank+4][i], 8, MPI_FLOAT, myid+4, 0									, MPI_COMM_WORLD);
					MPI_Recv(&leftRows[rank][i], 8, MPI_FLOAT, myid+4									, 0, MPI_COMM_WORLD, &status);


				}
				else if(myid==rank+4)
				{
					leftRows[rank-4][i]=myRows[rank][k];

					MPI_Send(&leftRows[rank-4][i], 8, MPI_FLOAT, myid-4, 0									, MPI_COMM_WORLD);
					MPI_Recv(&rightRows[rank-4][i], 8, MPI_FLOAT, myid-4									, 0, MPI_COMM_WORLD, &status);
		
				}
			}
			j = n*i+1;

			for(rank=5;rank<7;rank++)
			{
				if(myid==rank)
				{
					leftRows[rank+2][i]=myRows[rank][j];

					MPI_Send(&leftRows[rank+2][i], 8, MPI_FLOAT, myid+2, 0									, MPI_COMM_WORLD);
					MPI_Recv(&leftRows[rank+2][i], 8, MPI_FLOAT, myid+2									, 0, MPI_COMM_WORLD, &status);

				}
				else if(myid==rank+2)
				{
					leftRows[rank-2][i]=myRows[rank][j];

					MPI_Send(&leftRows[rank-2][i], 8, MPI_FLOAT, myid-2, 0									, MPI_COMM_WORLD);
					MPI_Recv(&leftRows[rank-2][i], 8, MPI_FLOAT, myid-2									, 0, MPI_COMM_WORLD, &status);

				}
			}
		}
		for(rank=0;rank<8;rank++)
		{
			if(myid==rank)
				epi = myRows[rank][0];
				MPI_Send(&epi, 8, MPI_FLOAT, 8, 0,										 MPI_COMM_WORLD);
			if(myid==8)
			{
				MPI_Recv(&epi, rank, MPI_FLOAT, myid-2									, 0, MPI_COMM_WORLD, &status);
				total = total + epi;
			}

		}
		if(myid==8)
		{
			total = total/n/n/6;	
			if(total<1e-14)	
				break;
		}

	}

	for(rank=0;rank<8;rank++)
	{
		if(myid!=8)
		{
			MPI_Send(&leftRows[myid], 8, MPI_FLOAT, 8, 0									, MPI_COMM_WORLD);
		}
		else
		{
			MPI_Recv(&leftRows[rank], 8, MPI_FLOAT, rank									, 0, MPI_COMM_WORLD, &status);
		}
	}
	if(myid==8)
		{
			draw(myRows[0],myRows[1],myRows[2],myRows[3],myRows[4],myRows[5]								,myRows[6],myRows[7],n);
			printf("hello\n");
		}
		
	MPI_Finalize();
	
//	while (epi>1e-16 && ite<100000)
//	{	
//		ite ++;
//		
//
//		iter_1(x01,x11,y11,n,0,1);
//		iter_1(x02,x12,y12,n,-5,1);
//		iter_1(x03,x13,y13,n,-2,1);
//		iter_1(x04,x14,y14,n,-3,1);
//		iter_2(x05,x15,y15,y25,n,-1,0);	
//		iter_2(x06,x16,y16,y26,n,-4,-5);	
//		iter_2(x07,x17,y17,y27,n,-1,-2);	
//		iter_2(x08,x18,y18,y28,n,-4,-3);	
//
//
//		for (i=1;i<=n;i++)
//		{
//			j = Ntri+i;
//			k = Nrec+i;
//			x11[i] = x02[j];
//			x12[i] = x01[j];
//			x13[i] = x04[j];
//			x14[i] = x03[j];
//			x15[i] = x06[k];
//			x16[i] = x05[k];
//			x17[i] = x08[k];
//			x18[i] = x07[k];
//			
//			j = i*n+n;
//			k = i*(i-1)/2+1;
//			y11[i] = x05[j];
//			y12[i] = x06[j];
//			y13[i] = x07[j];
//			y14[i] = x08[j];
//			y25[i] = x01[k];
//			y26[i] = x02[k];
//			y27[i] = x03[k];
//			y28[i] = x04[k];
//
//			j = n*i+1;
//			y15[i] = x07[j];
//			y17[i] = x05[j];
//			y16[i] = x08[j];
//			y18[i] = x06[j];
//		}
//		
//		epi = x01[0] + x02[0] + x03[0] + x04[0] + 									x05[0] + x06[0] + x07[0] + x08[0];
//		epi = epi/n/n/6;
//		printf("%.9lf  %d\n",epi,ite);	
//	}

	
	
	return 0;
}

