

#include "head.h"
//x0为长度为(n+1)*n+1的数组,前n个为虚拟节点
//x1,y1,y2长度为n+1
//x0,x1,y1,y2均从第二位(即[1])开使存储有效值
//计算中间四个矩形
int iter_2(float* x0 ,float* x1 ,float* y1,float* y2, int n, int f,int g)
{
	float C = -5.0/4;
//	float *new;
	int i,j,index;
	float alpha,beta;
	alpha = 3*n*n;
	beta = n*n;
	float epi = 0;
	float a;

//	new = (float *) malloc(sizeof(float)*(n*(n+1)+1));
	index = 0;
	for (i=1; i<=n; i++)
	{
		index ++;
		a = x0[index];
		//虚拟节点
//		new[index] = C/n + x0[index+n]; 
		x0[index] = C/n + x0[index+n]; 
	
		epi = epi + (a-x0[index])*(a-x0[index]);
		
	}
	for (i=1; i<n; i++){
		index ++;
		a = x0[index];

		//i,1节点 在左边界
//		new[index] = ((alpha*(x0[index+1]+y1[i])												+ beta*(x0[index+n])+new[index-n]) - f)/2 /									(alpha+beta);
		x0[index] = ((alpha*(x0[index+1]+y1[i])												+ beta*(x0[index+n]+x0[index-n])) - f)/2 /									(alpha+beta);
		
		epi = epi + (a-x0[index])*(a-x0[index]);
		
		for (j=2; j<n+1-i; j++)
		{
			index ++;
			a = x0[index];
			
			//i,j节点(内部)对角线左
//			new[index] = ((alpha*(x0[index+1]+new[index-1])												+ beta*(x0[index+n]+new[index-n])) - f)/2 /								(alpha+beta);
			x0[index] = ((alpha*(x0[index+1]+x0[index-1])												+ beta*(x0[index+n]+x0[index-n])) - f)/2 /								(alpha+beta);
			
			epi = epi + (a-x0[index])*(a-x0[index]);
		}
		for (j=n+1-i; j<n; j++)
		{
			index ++;
			a = x0[index];
			
			//i,j节点(内部)
//			new[index] = ((alpha*(x0[index+1]+new[index-1])											+ beta*(x0[index+n])+new[index-n]) - g)/2 /									(alpha+beta);
			x0[index] = ((alpha*(x0[index+1]+x0[index-1])											+ beta*(x0[index+n]+x0[index-n])) - g)/2 /									(alpha+beta);
			epi = epi + (a-x0[index])*(a-x0[index]);
		
		}

		index ++;
		a = x0[index];

		//i,n节点(右边)
//		new[index] = ((alpha*(y2[i]+new[index-1])													+ beta*(x0[index+n])+new[index-n]) - g)/2 /									(alpha+beta);
		x0[index] = ((alpha*(y2[i]+x0[index-1])													+ beta*(x0[index+n]+x0[index-n])) - g)/2 /									(alpha+beta);
		epi = epi + (a-x0[index])*(a-x0[index]);
		
	}
	index ++;
	// n,1 节点
	a = x0[index];

//	new[index] = ((alpha*(x0[index+1]+y1[n])												+ beta*(x1[1])+new[index-n-1]) - g)/2/(alpha+beta);			
	x0[index] = ((alpha*(x0[index+1]+y1[n])												+ beta*(x1[1]+x0[index-n-1])) - g)/2/(alpha+beta);			
	epi = epi + (a-x0[index])*(a-x0[index]);

	for (j=2; j<n; j++)
	{
		index ++;
		a = x0[index];

		//n,j节点 下底边
//		new[index] = ((alpha*(x0[index+1]+new[index-1])											+ beta*(x1[j])+new[index-n]) - g)/2/(alpha+beta);
		x0[index] = ((alpha*(x0[index+1]+x0[index-1])											+ beta*(x1[j]+x0[index-n])) - g)/2/(alpha+beta);

		epi = epi + (a-x0[index])*(a-x0[index]);

	}
	index ++;
	a = x0[index];

	//n,n节点
//	new[index] = ((alpha*(y2[n]+new[index-1])												+ beta*(x1[n])+new[index-n]) - g)/2/(alpha+beta);			
	x0[index] = ((alpha*(y2[n]+x0[index-1])												+ beta*(x1[n]+x0[index-n])) - g)/2/(alpha+beta);			
	epi = epi + (a-x0[index])*(a-x0[index]);
	
	x0[0] = epi;
return 0;

}


