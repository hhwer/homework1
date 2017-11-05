
#include "head.h"

int main(int argc, char ** argv)
{
	int n;
	if(argc==1)
	{	
		n = 25;	
	}
	else	
		n = atoi(argv[1]); 
	
	MPI_Status status;
	float* myRows[8],*downRows[8],*leftRows[8],*rightRows[8];
	int Nrec = n*n;
	int Ntri = n*(n-1)/2;
	int i,j,k,myid;
	float epi=0; 
	float total;
	int ite,rank ;
	int para1[] = {0,-5,-2,3,-1,-4,-1,-4};	
	int para2[] = {1,1,1,1,0,-5,-2,-3};	


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

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
//			rightRows[i] = x_init(n+1);
		}
	}

	for(i=0;i<8;i++)
	{
		downRows[i] = x_init(n+1);
		leftRows[i] = x_init(n+1);
		rightRows[i] = x_init(n+1);

	}


	

// printf("Hellow myid=%d  this is myRows initial\n",myid);
	
 	for(ite=0;ite<50000;ite++)
 	{
	printf("ite=%d\n",ite); 
 		//计算
 		for(rank=0;rank<4;rank++)
 		{
 			if(myid == rank)
			{
 				iter_1(myRows[rank],downRows[rank],leftRows[rank],n,								para1[rank],para2[rank]);
//				printf("myid=%d  this is calculate myRows\n",myid);

			}

 		}
 
 		for(rank=4;rank<8;rank++)
 		{
 			if(myid == rank)
			{
 				iter_2(myRows[rank],downRows[rank],leftRows[rank],								rightRows[rank],n,para1[rank],para2[rank]);
//				printf("myid=%d  this is calculate myRows\n",myid);
			}

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
//					printf("myid=%d  send downrows %d to %d\n",myid,i,rank+1);

 					MPI_Recv(&downRows[rank][i], 8, MPI_FLOAT, myid+1									, 0, MPI_COMM_WORLD, &status);
//					printf("myid=%d  receive downrows %d to %d\n",myid,i,rank+1);
					
 
 				}
 			}
 		
 			for(rank=1;rank<8;rank=rank+2)
 			{	
 				if(myid==rank)
 				{
 					downRows[rank-1][i] = myRows[rank][j];

 					MPI_Recv(&downRows[rank][i], 8, MPI_FLOAT, myid-1									, 0, MPI_COMM_WORLD, &status);

//					printf("myid=%d  receive downrows %d to %d\n",myid,i,rank-1);


 					MPI_Send(&downRows[rank-1][i], 8, MPI_FLOAT, myid-1, 0									, MPI_COMM_WORLD);
//					printf("myid=%d  send downrows %d to %d\n",myid,i,rank-1);
 				}
 			}
 			
 			j = i*n+n;
 			k = i*(i-1)/2+1;
			
//			printf("myid=%d  finish ith downrows transmission\n",myid);
 			for(rank=0;rank<4;rank++)
 			{

 				if(myid==rank)
 				{
//					printf("myid=%d  want to update rightrows\n",myid);

 					rightRows[rank+4][i]=myRows[rank][j];
					
//					printf("myid=%d  updated rightrows\n",myid);


 					MPI_Send(&rightRows[rank+4][i], 8, MPI_FLOAT, myid+4, 0									, MPI_COMM_WORLD);
//					printf("myid=%d  send rightrows %d to %d\n",myid,i,rank+4);


 					MPI_Recv(&leftRows[rank][i], 8, MPI_FLOAT, myid+4									, 0, MPI_COMM_WORLD, &status);
 
 
 				}
 				else if(myid==rank+4)
 				{
 					leftRows[myid-4][i]=myRows[myid][k];
 
 					MPI_Recv(&rightRows[myid-4][i], 8, MPI_FLOAT, myid-4									, 0, MPI_COMM_WORLD, &status);
 					MPI_Send(&leftRows[myid-4][i], 8, MPI_FLOAT, myid-4, 0									, MPI_COMM_WORLD);
//					printf("myid=%d  send leftrows %d to %d\n",myid,i,myid-4);



 		
 				}
 			}
 			j = n*i+1;
 
 			for(rank=4;rank<6;rank++)
 			{
 				if(myid==rank)
 				{
 					leftRows[rank+2][i]=myRows[rank][j];
 
 					MPI_Send(&leftRows[rank+2][i], 8, MPI_FLOAT, myid+2, 0									, MPI_COMM_WORLD);
//					printf("myid=%d  send leftrows %d to %d\n",myid,i,rank+2);

 					MPI_Recv(&leftRows[rank+2][i], 8, MPI_FLOAT, myid+2									, 0, MPI_COMM_WORLD, &status);
 
 				}
 				else if(myid==rank+2)
 				{
 					leftRows[rank-2][i]=myRows[rank][j];
 
 					MPI_Recv(&leftRows[rank-2][i], 8, MPI_FLOAT, myid-2									, 0, MPI_COMM_WORLD, &status);
 					MPI_Send(&leftRows[rank-2][i], 8, MPI_FLOAT, myid-2, 0									, MPI_COMM_WORLD);
					printf("myid=%d  send leftrows %d to %d\n",myid,i,rank-2); 
 				}
 			}
 		}
 		epi = myRows[myid][0];
		printf("myid=%d epi= %.9fl\n ",myid,epi);
		MPI_Reduce(&epi, &total, 8, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);	
		total = total/n/n/6;
		printf("%.13lf  %d\n",total,ite);

		if(total<1e-12)
		{
			break;

		}

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
 
 	}



//draw


//for(rank=0;rank<8;rank++)
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
		
	MPI_Finalize();
	

	
	
	return 0;
}

