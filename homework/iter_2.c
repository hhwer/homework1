
/**
* @file iter_2.c
* @brief 计算矩形区域的迭代
* @author hh
* @version 1
* @date 2017-11-13
*/

#include "head.h"
/* --------------------------------------------------------------------------*/
/**
* @brief 计算中间的四个矩形区域
*
* @param x0 长度为(n+1)*n+1的数组,从第2个节点开始存储函数值,其前n个为虚拟节点\n
* @param x1 长度为n+1的数组,从第2个节点开始存储下侧边界值 \n
* @param y1 长度为n+1的数组,从第2个节点开始存储左侧边界值 \n
* @param y2 长度为n+1的数组,从第2个节点开始存储右侧边界值 \n
* @param n  问题规模\n
* @param f	左上部分的\f$-\Delta\f$值\n
* @param g  右上部分的\f$-\Delta\f$值
* @param K  是否强制边值 K=0强制
*
* @returns  计算结果直接在数组x0中更新,迭代步的距离存储在第一位
*/
/* ----------------------------------------------------------------------------*/
int iter_2(float* x0 ,float* x1 ,float* y1,float* y2, int n, int f,int g)
{
	float C = -5.0/4;
	int i,j,index;
	float alpha,beta;
	alpha = 3*n*n;
	beta = n*n;
	float epi = 0;
	float a;

	index = 0;
	for (i=1; i<=n; i++)
	{
		index ++;
		a = x0[index];
		///  虚拟节点
		x0[index] = C/n + x0[index+n]; 
	
		epi = epi + (a-x0[index])*(a-x0[index]);
		
	}
	for (i=1; i<n; i++){
		index ++;
		a = x0[index];

		///  i,1节点 在左边界
		x0[index] = ((alpha*(x0[index+1]+y1[i])												+ beta*(x0[index+n]+x0[index-n])) - f)/2 /									(alpha+beta);
		
		epi = epi + (a-x0[index])*(a-x0[index]);
		
		for (j=2; j<n+1-i; j++)
		{
			index ++;
			a = x0[index];
			
			///  i,j节点(内部)对角线左
			x0[index] = ((alpha*(x0[index+1]+x0[index-1])												+ beta*(x0[index+n]+x0[index-n])) - f)/2 /								(alpha+beta);
			
			epi = epi + (a-x0[index])*(a-x0[index]);
		}
		for (j=n+1-i; j<n; j++)
		{
			index ++;
			a = x0[index];
			
			///  i,j节点(内部)
			x0[index] = ((alpha*(x0[index+1]+x0[index-1])											+ beta*(x0[index+n]+x0[index-n])) - g)/2 /									(alpha+beta);
			epi = epi + (a-x0[index])*(a-x0[index]);
		
		}

		index ++;
		a = x0[index];

		///  i,n节点(右边)
//		new[index] = ((alpha*(y2[i]+new[index-1])													+ beta*(x0[index+n])+new[index-n]) - g)/2 /									(alpha+beta);
		x0[index] = ((alpha*(y2[i]+x0[index-1])													+ beta*(x0[index+n]+x0[index-n])) - g)/2 /									(alpha+beta);
		epi = epi + (a-x0[index])*(a-x0[index]);
		
	}
	index ++;
	///  n,1 节点
	a = x0[index];

	x0[index] = ((alpha*(x0[index+1]+y1[n])												+ beta*(x1[1]+x0[index-n])) - g)/2/(alpha+beta);			
	epi = epi + (a-x0[index])*(a-x0[index]);

	for (j=2; j<n; j++)
	{
		index ++;
		a = x0[index];

		///  n,j节点 下底边
		x0[index] = ((alpha*(x0[index+1]+x0[index-1])											+ beta*(x1[j]+x0[index-n])) - g)/2/(alpha+beta);

		epi = epi + (a-x0[index])*(a-x0[index]);

	}
	index ++;
	a = x0[index];

	///  n,n节点
	x0[index] = ((alpha*(y2[n]+x0[index-1])												+ beta*(x1[n]+x0[index-n])) - g)/2/(alpha+beta);			
	epi = epi + (a-x0[index])*(a-x0[index]);
	
	x0[0] = epi;
return 0;

}


