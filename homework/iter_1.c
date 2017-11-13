
/**
* @file iter_1.c
* @brief 三角形分块的迭代  
* @author hh
* @version 1
* @date 2017-11-13
*/

#include "head.h"
//x0为长度为n*(n+1)/2+1的数组,x1,y1长度为n+1,均从第二位(即[1])开使存储有效值
//计算左右四个三角形
/* --------------------------------------------------------------------------*/
/**
* @brief  计算左右共四个三角形区域的迭代,完成后直接更新输入的各节点值
*
* @param x0 长度为n*(n+1)/2+1的数组 从第二位开始存储三角形中各节点值 第一位存储迭代步之间的距离\n
* @param x1 长度为n+1的数组,从第二位开始存储外部下侧边界的n个值\n
* @param y1	长度位n+1的数组,从第二位开始存储外部左侧边界的n个值\n
* @param n  问题规模 \n
* @param f  -\f$\Delta\f$的值 \n
* @param K  是否使用强制边界条件  K=0为使用\\
*
* @returns  计算结果直接在数组x0中更新
*/
/* ----------------------------------------------------------------------------*/
int iter_1(float* x0 ,float* x1 ,float* y1, int n, int f,int K)
{
	float C = -5.0/4;
	int i,j,index;
	float alpha,beta,alpha_1,beta_1;
	alpha_1 = 3*n/2.0;
	beta_1 = n/2.0;
	alpha = 3*n*n;
	beta = n*n;
	float epi = 0;
	float a;


	index = 1;
	a = x0[index];
	
	///  1,1节点
	x0[1] = (C + alpha_1*y1[1] + beta_1*x0[2]) / (alpha_1 + beta_1);


	epi = epi + (a-x0[index])*(a-x0[index]);
	for (i=2; i<n; i++)
	{
		index ++;
		///  i,1节点 在左边界
		a = x0[index];
		x0[index] = ((alpha*(x0[index+1]+y1[i])												+ beta*(x0[index+i]+x0[index-i+1])) - f)/2 /								(alpha+beta);
		epi = epi + (a-x0[index])*(a-x0[index]);
		for (j=2; j<i; j++)
		{
			index ++;
			a = x0[index];

			///  i,j节点(内部)
			x0[index] = ((alpha*(x0[index+1]+x0[index-1])										+ beta*(x0[index+i]+x0[index-i+1])) - f)/2 /								(alpha+beta);

			epi = epi + (a-x0[index])*(a-x0[index]);
			
		}
		index ++;
		///  i,i节点(斜边)
		a = x0[index];

		x0[index] = (C + alpha_1*x0[index-1] + beta_1*x0[index+i]) /                                         (alpha_1 + beta_1);

		epi = epi + (a-x0[index])*(a-x0[index]);
		
	}
	index ++;
	a = x0[index];

	///  n,1 节点
	x0[index] = ((alpha*(x0[index+1]+y1[n])													+ beta*(x1[1]+x0[index-n+1])) - f)/2/(alpha+beta);
	epi = epi + (a-x0[index])*(a-x0[index]);
	
	for (j=2; j<n; j++)
	{
		index ++;
		a = x0[index];
		///  n,j节点 下底边
		x0[index] = ((alpha*(x0[index+1]+x0[index-1])											+ beta*(x1[j]+x0[index-n+1])) - f)/2/(alpha+beta);
		epi = epi + (a-x0[index])*(a-x0[index]);
		
	}
	index ++;
	a = x0[index];
	///  n,n节点
	x0[index] = (C + alpha_1*x0[index-1] + beta_1*x1[n]) / 								(alpha_1 + beta_1);

	if (K <= 0)
		x0[index] = 0;
	epi = epi + (a-x0[index])*(a-x0[index]);
	
	x0[0] = epi;
return 0;

}

