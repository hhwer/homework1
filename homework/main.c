
/**
* @file main.c
* @brief  函数主体 给定N,计算对应网格的问题的解
* @author hh
* @version 1
* @date 2017-11-13
*/
#include<time.h>
#include "head.h"
#define N 20
int main(int argc, char ** argv)
{
	int a=1;
	int n;
	if(argc==1)
	{	
		n = N;	
	}
	else	
		n = atoi(argv[1]); 
	MPI_Status status;
	int Nrec = n*n;
	int Ntri = n*(n-1)/2;
	int i,j,k,myid;
	float epi; 
	float total;
	int rank ;
	int para1[] = {0,-5,-2,-3,-1,-4,-1,-4};	
	int para2[] = {0,1,1,1,0,-5,-2,-3};	
	clock_t start,finish;
float myRows[8][N*(N+1)+1],downRows[8][N+1],leftRows[8][N+1],rightRows[8][N+1];
for(i=0;i<8;i++)
	{
		for(j=0;j<N+1;j++)
		{
			downRows[i][j] = 0;
			leftRows[i][j] = 0;
			rightRows[i][j] = 0;
		}
		for(j=0;j<N*(N+1)+1;j++)
			myRows[i][j] = 0;
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);



	int ite;
	
	start = clock();
 	for(ite=0;ite<100000;ite=ite+1)
 	{
 		/**计算每一分块的迭代*/
 		for(rank=0;rank<4;rank++)
 		{
 			if(myid == rank)
			{
				
 				iter_1(myRows[rank],downRows[rank],leftRows[rank],n,								para1[rank],para2[rank]);

			}
 		}
 
 		for(rank=4;rank<8;rank++)
 		{
 			if(myid == rank)
			{

 				iter_2(myRows[rank],downRows[rank],leftRows[rank],								rightRows[rank],n,para1[rank],para2[rank]);

			}

 		}
 		/**进程间传递数据*/

/**边界的提取*/
 		for(i=1;i<=n;i++)
 		{
 			j = Ntri+i;
 			k = Nrec+i;
 			for(rank=0;rank<4;rank=rank+2)
 			{	
 				if(myid==rank)
 				{
 					downRows[myid+1][i] = myRows[myid][j];
 				}
				else if(myid==rank+1)
					downRows[myid-1][i] = myRows[myid][j];
 			}
 		
 			for(rank=4;rank<8;rank=rank+2)
 			{	
 				if(myid==rank)
 				{
 					downRows[myid+1][i] = myRows[myid][k];
 				}
				else if(myid==rank+1)
					downRows[myid-1][i] =myRows[myid][k];
 			}
 			j = i*n+n;
 			k = i*(i-1)/2+1;
 			for(rank=0;rank<4;rank++)
 			{

 				if(myid==rank)
 				{
 					rightRows[myid+4][i]=myRows[myid][k];
 				}
 				else if(myid==rank+4)
 				{
 					leftRows[myid-4][i]=myRows[myid][j];
 				}
 			}
 			j = n*i+1;
 
 			for(rank=4;rank<6;rank++)
 			{
 				if(myid==rank)
 				{
 					leftRows[myid+2][i]=myRows[myid][j];
 				}
 				else if(myid==rank+2)
 				{
 					leftRows[myid-2][i]=myRows[myid][j];
 				}
 			}
 		}

		
//		MPI_Barrier(MPI_COMM_WORLD);
		

/**边界信息的传递*/

		
		/**底边的传递*/
		for(rank=0;rank<8;rank=rank+2)
		{	
			if(myid==rank)
			{
				MPI_Send(&downRows[myid+1], n+1, MPI_FLOAT, myid+1, 1									, MPI_COMM_WORLD);
				
				MPI_Recv(&downRows[myid], n+1, MPI_FLOAT, myid+1									, 2, MPI_COMM_WORLD, &status);
			}
			else if(myid==rank+1)
			{
				MPI_Recv(&downRows[myid], n+1, MPI_FLOAT, myid-1									, 1, MPI_COMM_WORLD, &status);
				MPI_Send(&downRows[myid-1], n+1, MPI_FLOAT, myid-1, 2									, MPI_COMM_WORLD);
			}

		}
		
		/**三角形左边以及矩形右边*/
		for(rank=0;rank<4;rank++)
		{
			if(myid==rank)
			{
 					MPI_Send(&rightRows[myid+4], n+1, MPI_FLOAT, myid+4, 3									, MPI_COMM_WORLD);

					MPI_Recv(&leftRows[myid], n+1, MPI_FLOAT, myid+4									, 4, MPI_COMM_WORLD, &status);
							
			}
			else if(myid==rank+4)
			{
					MPI_Recv(&rightRows[myid], n+1, MPI_FLOAT, myid-4									, 3, MPI_COMM_WORLD, &status);

 					MPI_Send(&leftRows[myid-4], n+1, MPI_FLOAT, myid-4, 4									, MPI_COMM_WORLD);

			}
		}

		/**矩形左边界*/
 		for(rank=4;rank<6;rank++)
		{
			if(myid==rank)
			{
 					MPI_Send(&leftRows[myid+2], n+1, MPI_FLOAT, myid+2, 5									, MPI_COMM_WORLD);

 					MPI_Recv(&leftRows[myid], n+1, MPI_FLOAT, myid+2									, 6, MPI_COMM_WORLD, &status);

			}
			else if(myid==rank+2)
			{
 					MPI_Recv(&leftRows[myid], n+1, MPI_FLOAT, myid-2									, 5, MPI_COMM_WORLD, &status);
					
 					MPI_Send(&leftRows[myid-2], n+1, MPI_FLOAT, myid-2, 6									, MPI_COMM_WORLD);
			}
		}
		
 		epi = myRows[myid][0];

		/* --------------------------------------------------------------------------*/
		/**
		* @brief 对所有区域的误差求和
		*
		* @param epi  单个区域的误差
		* @param total 总误差
		* @param 1    epi是一个float
		* @param MPI_FLOAT 
		* @param MPI_SUM   求和符
		* @param 0    在0进程中求和
		* @param MPI_COMM_WORLD  
		*/
		/* ----------------------------------------------------------------------------*/
		MPI_Reduce(&epi, &total, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

		if(myid==0)
		{
			total = total/n/n/6;
			printf("ite=%d total = %.14f\n",ite,total);
			if(total<1e-14)
			{
				a = 0;				
			}	
		}

		/* --------------------------------------------------------------------------*/
		/**
		* @brief   0进程用Bcast将标示a传给所有进程
		*
		* @param a  0进程检验终止条件,若终止a=0,否则a=1
		* @param 1  a是1个整数
		* @param MPI_INT
		* @param 0  从0进程Bcast
		* @param MPI_COMM_WORLD 
		*/
		/* ----------------------------------------------------------------------------*/
		MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);		
		if(a==0)
		{
			break;
		}
		
	}   // end of the iteration 
finish=clock(); 
//draw
	if(myid!=0)
	{
		

		MPI_Send(&myRows[myid], N*(N+1)+1, MPI_FLOAT, 0, 0									, MPI_COMM_WORLD);

	}
	else
	{
		printf("time=%f\n",(double)(finish-start)/CLOCKS_PER_SEC);
				
		printf("ite = %d\n total=%.14f\n",ite,total);

		for(rank=1;rank<8;rank++)
		{
		MPI_Recv(&myRows[rank], N*(N+1)+1, MPI_FLOAT,rank 									, 0, MPI_COMM_WORLD,&status);

		}
		draw(myRows[0],myRows[1],myRows[2],myRows[3],myRows[4],myRows[5]								,myRows[6],myRows[7],n);
	}
	MPI_Finalize();	
	
	return 0;
}

