
#include "head.h"
#define N 25

int main(int argc, char ** argv)
{
	int a=1;
	int n;
	if(argc==1)
	{	
		n = 25;	
	}
	else	
		n = atoi(argv[1]); 


	MPI_Status status;
//	float* myRows[8],*downRows[8],*leftRows[8],*rightRows[8];
//	float** myRows = (float**)malloc(sizeof(float*)*8);
//	float** leftRows = (float**)malloc(sizeof(float*)*8);
//	float** rightRows = (float**)malloc(sizeof(float*)*8);
//	float** downRows = (float**)malloc(sizeof(float*)*8);
	int Nrec = n*n;
	int Ntri = n*(n-1)/2;
	int i,j,k,myid;
	float epi; 
	float total;
	int rank ;
	int para1[] = {0,-5,-2,3,-1,-4,-1,-4};	
	int para2[] = {1,1,1,1,0,-5,-2,-3};	

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


//调试用
// 	int n0,n1,n2;
//调试死循环		
//while(a==1)
//{}



// 初始化

	int ite;
	
//
//	for(i=0;i<4;i++)
//	{	
//		if(myid == i)
//		myRows[i] = x_init(n*(n+1)/2+1);
//
//	}
//
//	for(i=4;i<8;i++)
//	{
//		if(myid == i)
//		{
//			myRows[i] = x_init(n*(n+1)+1);
////			rightRows[i] = x_init(n+1);
//		}
//	}
//
//	for(i=0;i<8;i++)
//	{
//		downRows[i] = x_init(n+1);
//		leftRows[i] = x_init(n+1);
//		rightRows[i] = x_init(n+1);
//
//	}

	
	

// printf("Hellow myid=%d  this is myRows initial\n",myid);
	
 	for(ite=0;ite<50000;ite=ite+1)
 	{
//	printf("ite=%d\n",ite); 
 		//计算
 		for(rank=0;rank<4;rank++)
 		{
 			if(myid == rank)
			{
//				printf("myid=%d ite=%d  before caculate\n",myid,ite);
				
//				n0 = sizeof(myRows[rank]);
//				n1 = sizeof(downRows[rank]);
 				iter_1(myRows[rank],downRows[rank],leftRows[rank],n,								para1[rank],para2[rank]);
//				printf("myid=%d ite=%d  after caculate\n",myid,ite);

			}
 		}
 
 		for(rank=4;rank<8;rank++)
 		{
 			if(myid == rank)
			{
//				printf("myid=%d ite=%d  before caculate\n",myid,ite);

 				iter_2(myRows[rank],downRows[rank],leftRows[rank],								rightRows[rank],n,para1[rank],para2[rank]);
//				printf("myid=%d ite=%d  after caculate\n",myid,ite);

			}

 		}
 		//传递数据

//边界的提取
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
//		printf("myid=%d ite=%d  after caculate bound\n",myid,ite);

		
		MPI_Barrier(MPI_COMM_WORLD);
		

//边界的传递
		
		//底边的交互
		for(rank=0;rank<8;rank=rank+2)
		{	
			if(myid==rank)
			{
				MPI_Send(&downRows[myid+1], n+1, MPI_FLOAT, myid+1, 1									, MPI_COMM_WORLD);
//				printf("myid=%d ite=%d down send to myid+1\n",myid,ite);
				
				MPI_Recv(&downRows[myid], n+1, MPI_FLOAT, myid+1									, 2, MPI_COMM_WORLD, &status);
			}
			else if(myid==rank+1)
			{
				MPI_Recv(&downRows[myid], n+1, MPI_FLOAT, myid-1									, 1, MPI_COMM_WORLD, &status);
//				printf("myid=%d ite=%d down recv from myid-1\n",myid,ite);	
				MPI_Send(&downRows[myid-1], n+1, MPI_FLOAT, myid-1, 2									, MPI_COMM_WORLD);
			}

		}
		
		//三角形左边 矩形右边
		for(rank=0;rank<4;rank++)
		{
			if(myid==rank)
			{
 					MPI_Send(&rightRows[myid+4], n+1, MPI_FLOAT, myid+4, 3									, MPI_COMM_WORLD);
//					printf("myid=%d ite=%d right send to myid+4\n",myid,ite);	

					MPI_Recv(&leftRows[myid], n+1, MPI_FLOAT, myid+4									, 4, MPI_COMM_WORLD, &status);
//					printf("myid=%d ite=%d left recv from myid+4\n",myid,ite);	
							
			}
			else if(myid==rank+4)
			{
					MPI_Recv(&rightRows[myid], n+1, MPI_FLOAT, myid-4									, 3, MPI_COMM_WORLD, &status);
//					printf("myid=%d ite=%d right recv from myid-4\n",myid,ite);	

 					MPI_Send(&leftRows[myid-4], n+1, MPI_FLOAT, myid-4, 4									, MPI_COMM_WORLD);
//					printf("myid=%d ite=%d left send to myid-4\n",myid,ite);	

			}
		}

		//
 		for(rank=4;rank<6;rank++)
		{
			if(myid==rank)
			{
 					MPI_Send(&leftRows[myid+2], n+1, MPI_FLOAT, myid+2, 5									, MPI_COMM_WORLD);
//					printf("myid=%d ite=%d left send to myid+2\n",myid,ite);	

 					MPI_Recv(&leftRows[myid], n+1, MPI_FLOAT, myid+2									, 6, MPI_COMM_WORLD, &status);
//					printf("myid=%d ite=%d left recv from myid+2\n",myid,ite);	

			}
			else if(myid==rank+2)
			{
 					MPI_Recv(&leftRows[myid], n+1, MPI_FLOAT, myid-2									, 5, MPI_COMM_WORLD, &status);
//					printf("myid=%d ite=%d left recv from myid-2\n",myid,ite);	
					
 					MPI_Send(&leftRows[myid-2], n+1, MPI_FLOAT, myid-2, 6									, MPI_COMM_WORLD);
//					printf("myid=%d ite=%d left send to myid-2\n",myid,ite);	
			}
		}
		


 		epi = myRows[myid][0];
//		printf("myid=%d ite=%d epi= %.9lf\n ",myid,ite,epi);
//		MPI_Reduce(&epi,&total, 1, MPI_DOUBLE, MPI_SUM,											 0, MPI_COMM_WORLD);	
		if(myid==0)
 		{
//			printf("myid0\n");
//			printf("total=0\n");
			total = myRows[myid][0];

			for(rank=1;rank<8;rank++)
			{
//				printf("want to receive epi from %d\n",rank);
 				MPI_Recv(&epi, 1, MPI_FLOAT, rank
				, 0, MPI_COMM_WORLD, &status);

//				printf("myid=0 received epi from %d\n",rank);
 				total = total + epi;
//				printf("total=%.13f\n",total);
			}
 		}
		else
		{
 			epi = myRows[myid][0];
 			MPI_Send(&epi, 1, MPI_FLOAT, 0, 0,														MPI_COMM_WORLD);
//			printf("myid=%d  sended epi\n",myid);
		}


//		while(a==1)
//		{
//		}


		if(myid==0)
		{
			total = total/n/n/6;
			printf("total=%.13f  ite=%d\n",total,ite);
			if(total<1e-12)
			{
				printf("myid=%d total break\n",myid);
				a = 0;				
			}	
			else
			{}

			for(rank=1;rank<8;rank++)
			{
//				printf("0 send a to %d\n",rank);
 				MPI_Send(&a, 1, MPI_INT, rank, 1									, MPI_COMM_WORLD);	
			}

		}
			
		else
		{
//			printf("%d want to received a\n",myid);
			MPI_Recv(&a, 1, MPI_INT, 0, 1, MPI_COMM_WORLD,&status);
//			printf("%d received a\n",myid);
		}
		if(a==0)
			break;

//		printf("myid=%d is waiting 1  a=%d \n",myid,a);


//		MPI_Barrier(MPI_COMM_WORLD);

	}




//		死循环
//		while(a==1)
//		{}


//		if(total<1e-12 && myid == 0)
//		{
//			printf("myid=%d total break\n",myid);
//			MPI_Abort(MPI_COMM_WORLD, 250);
//		}
//		MPI_Barrier(MPI_COMM_WORLD);

//		if(myid==8)
// 		{
//			printf("myid=8\n");
//			total = 0;
//			printf("total=0\n");
//
//			for(rank=0;rank<8;rank++)
//			{
// 				printf("want to receive epi from %d\n",rank);
// 				MPI_Recv(&rightRows[0][0], 8, MPI_FLOAT, rank
//				, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//
//				printf("myid=8 received epi from %d\n",rank);
// 				total = total + rightRows[0][0];
//			}
// 		}
//		else
//		{
// 			epi = myRows[myid][0];
//			rightRows[0][0] = myRows[myid][0];
// 			MPI_Send(&rightRows[0][0], 8, MPI_FLOAT, 8, 1,														MPI_COMM_WORLD);
//			printf("myid=%d  sended epi\n",myid);
//				
//		}
// 		if(myid==8)
// 		{
// 			total = total/n/n/6;	
//			printf("epi=%.14lf  ite=%d\n",total,ite);
// 			if(total<1e-14)	
// 				break;
// 		}
 
 	



//draw
printf("myid=%d want to draw\n",myid);
	if(myid!=0)
	{
		MPI_Send(&myRows[myid], N*(N+1)+1, MPI_FLOAT, 0, 0									, MPI_COMM_WORLD);
		printf("%d send myrows to 0\n",myid);

	}
	else
	{
		for(rank=1;rank<8;rank++)
		{
		MPI_Recv(&myRows[rank], N*(N+1)+1, MPI_FLOAT,rank 									, 0, MPI_COMM_WORLD, &status);
		printf("0 received myrows from %d\n",rank);

		}
		draw(myRows[0],myRows[1],myRows[2],myRows[3],myRows[4],myRows[5]								,myRows[6],myRows[7],n);

//		while(a==0)
//		{}
	}
	
		
	MPI_Finalize();	
		printf("hello\n");

		
	
	return 0;
}

