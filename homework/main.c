
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
	
//	MPI_Status status;
	float* myRows[8],*downRows[8],*leftRows[8],*rightRows[8];
//	float* x01,*x02,*x03,*x04,*x05,*x06,*x07,*x08;
//	float* x11,*x12,*x13,*x14,*x15,*x16,*x17,*x18;
//	float* y11,*y12,*y13,*y14,*y15,*y16,*y17,*y18;
//	float* y25,*y26,*y27,*y28;
	int Nrec = n*n;
	int Ntri = n*(n-1)/2;
	int i,j,k;
//	int total,myid;
	float epi = 1;
	int ite = 0;
//	int rank ;
	int para1[] = {0,-5,-2,3,-1,-4,-1,-4};	
	int para2[] = {1,1,1,1,0,-5,-2,-3};	


//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

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
//		if(myid == i)
		myRows[i] = x_init(n*(n+1)/2+1);
	}

	for(i=4;i<8;i++)
	{
//		if(myid == i)
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
	


	
//	for(ite=0;ite<50000;ite++)
//	{
//
//		//计算
//		for(rank=0;rank<4;rank++)
//		{
//			if(myid == rank)
//				iter_1(myRows[rank],downRows[rank],leftRows[rank],n,								para1[rank],para2[rank]);
//		}
//
//		for(rank=4;rank<8;rank++)
//		{
//			if(myid == rank)
//				iter_2(myRows[rank],downRows[rank],leftRows[rank],								rightRows[rank],n,para1[rank],para2[rank]);
//		}
//
//		//传递数据
//		
//		for(i=1;i<n;i++)
//		{
//			j = Ntri+i;
//			k = Nrec+i;
//			for(rank=0;rank<8;rank=rank+2)
//			{	
//				if(myid==rank)
//				{
//					downRows[rank+1][i] = myRows[rank][j];
//					MPI_Send(&downRows[rank+1][i], 8, MPI_FLOAT, myid+1, 0									, MPI_COMM_WORLD);
//					MPI_Recv(&downRows[rank][i], 8, MPI_FLOAT, myid+1									, 0, MPI_COMM_WORLD, &status);
//
//				}
//			}
//		
//			for(rank=1;rank<8;rank=rank+2)
//			{	
//				if(myid==rank)
//				{
//					downRows[rank-1][i] = myRows[rank][j];
//
//					MPI_Send(&downRows[rank-1][i], 8, MPI_FLOAT, myid-1, 0									, MPI_COMM_WORLD);
//					MPI_Recv(&downRows[rank][i], 8, MPI_FLOAT, myid-1									, 0, MPI_COMM_WORLD, &status);
//
//				}
//			}
//			
//			j = i*n+n;
//			k = i*(i-1)/2+1;
//
//			for(rank=0;rank<4;rank++)
//			{
//				if(myid==rank)
//				{
//					rightRows[rank+4][i]=myRows[rank][j];
//
//					MPI_Send(&rightRows[rank+4][i], 8, MPI_FLOAT, myid+4, 0									, MPI_COMM_WORLD);
//					MPI_Recv(&leftRows[rank][i], 8, MPI_FLOAT, myid+4									, 0, MPI_COMM_WORLD, &status);
//
//
//				}
//				else if(myid==rank+4)
//				{
//					leftRows[rank-4][i]=myRows[rank][k];
//
//					MPI_Send(&leftRows[rank-4][i], 8, MPI_FLOAT, myid-4, 0									, MPI_COMM_WORLD);
//					MPI_Recv(&rightRows[rank-4][i], 8, MPI_FLOAT, myid-4									, 0, MPI_COMM_WORLD, &status);
//		
//				}
//			}
//			j = n*i+1;
//
//			for(rank=5;rank<7;rank++)
//			{
//				if(myid==rank)
//				{
//					leftRows[rank+2][i]=myRows[rank][j];
//
//					MPI_Send(&leftRows[rank+2][i], 8, MPI_FLOAT, myid+2, 0									, MPI_COMM_WORLD);
//					MPI_Recv(&leftRows[rank+2][i], 8, MPI_FLOAT, myid+2									, 0, MPI_COMM_WORLD, &status);
//
//				}
//				else if(myid==rank+2)
//				{
//					leftRows[rank-2][i]=myRows[rank][j];
//
//					MPI_Send(&leftRows[rank-2][i], 8, MPI_FLOAT, myid-2, 0									, MPI_COMM_WORLD);
//					MPI_Recv(&leftRows[rank-2][i], 8, MPI_FLOAT, myid-2									, 0, MPI_COMM_WORLD, &status);
//
//				}
//			}
//		}
//		for(rank=0;rank<8;rank++)
//		{
//			if(myid==rank)
//				epi = myRows[rank][0];
//				MPI_Send(&epi, 8, MPI_FLOAT, 8, 0,										 MPI_COMM_WORLD);
//			if(myid==8)
//			{
//				MPI_Recv(&epi, rank, MPI_FLOAT, myid-2									, 0, MPI_COMM_WORLD, &status);
//				total = total + epi;
//			}
//
//		}
//		if(myid==8)
//		{
//			total = total/n/n/6;	
//			if(total<1e-14)	
//				break;
//		}
//
//	}
//
//	for(rank=0;rank<8;rank++)
//	{
//		if(myid!=8)
//		{
//			MPI_Send(&leftRows[myid], 8, MPI_FLOAT, 8, 0									, MPI_COMM_WORLD);
//		}
//		else
//		{
//			MPI_Recv(&leftRows[rank], 8, MPI_FLOAT, rank									, 0, MPI_COMM_WORLD, &status);
//		}
//	}
//	if(myid==8)
//		{
//			draw(myRows[0],myRows[1],myRows[2],myRows[3],myRows[4],myRows[5]								,myRows[6],myRows[7],n);
//			printf("hello\n");
//		}
//		
//	MPI_Finalize();
	
  	while (epi>1e-16 && ite<100000)
  	{	
  		ite ++;
  		
  
  		iter_1(myRows[0],downRows[0],leftRows[0],n,para1[0],1);
  		iter_1(myRows[1],downRows[1],leftRows[1],n,para1[1],1);
  		iter_1(myRows[2],downRows[2],leftRows[2],n,para1[2],1);
  		iter_1(myRows[3],downRows[3],leftRows[3],n,para1[3],1);
  		iter_2(myRows[4],downRows[4],leftRows[4],rightRows[4],n,para1[4],para2[4]);	
  		iter_2(myRows[5],downRows[5],leftRows[5],rightRows[5],n,para1[5],para2[5]);	
  		iter_2(myRows[6],downRows[6],leftRows[6],rightRows[6],n,para1[6],para2[6]);	
  		iter_2(myRows[7],downRows[7],leftRows[7],rightRows[7],n,para1[7],para2[7]);	
  
  
  		for (i=1;i<=n;i++)
  		{
  			j = Ntri+i;
  			k = Nrec+i;
  			downRows[0][i] = myRows[1][j];
  			downRows[1][i] = myRows[0][j];
  			downRows[2][i] = myRows[3][j];
  			downRows[3][i] = myRows[2][j];
  			downRows[4][i] = myRows[5][k];
  			downRows[5][i] = myRows[4][k];
  			downRows[6][i] = myRows[7][k];
  			downRows[7][i] = myRows[6][k];
  			
  			j = i*n+n;
  			k = i*(i-1)/2+1;
  			leftRows[0][i] = myRows[4][j];
  			leftRows[1][i] = myRows[5][j];
  			leftRows[2][i] = myRows[6][j];
  			leftRows[3][i] = myRows[7][j];
  			rightRows[4][i] = myRows[0][k];
  			rightRows[5][i] = myRows[1][k];
  			rightRows[6][i] = myRows[2][k];
  			rightRows[7][i] = myRows[3][k];
  
  			j = n*i+1;
  			leftRows[4][i] = myRows[6][j];
  			leftRows[6][i] = myRows[4][j];
  			leftRows[5][i] = myRows[7][j];
  			leftRows[7][i] = myRows[5][j];
  		}
  		
  //		epi = x01[0] + x02[0] + x03[0] + x04[0] + 									x05[0] + x06[0] + x07[0] + x08[0];
  		epi = 0;
  		for(i=0;i<8;i++)
			epi = epi + myRows[i][0];
  		epi = epi/n/n/6;
  		printf("%.9lf  %d\n",epi,ite);	
  	}
	
	
	draw(myRows[0],myRows[1],myRows[2],myRows[3],myRows[4],myRows[5]								,myRows[6],myRows[7],n);

	return 0;
}

