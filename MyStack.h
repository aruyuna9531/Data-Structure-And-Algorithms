/*
Author: aruyuna~
Date: 2018.7.31
*/
#include "MyStack.h"
#include<cstdio>
#include<iostream>
using namespace std;

void testCases(int &stackSize, int &pushElements, int &popElements){
	int cases;
	printf("选择测试用例：\n1.基本功能测试\n2.边缘值测试\n3.错误测试（容量超出）\n4.错误测试（空栈弹出）\n5.错误测试（空栈读顶）\n6.错误测试（容量非法）\n7.(Bonus)错误测试（容量超内存）");
	do{
	scanf("%d",&cases);
	switch(cases){
	case 1://基本功能（60分）：有限栈容量>0，入栈元素数量<=栈容量，出栈元素数量<=入栈元素数量
		stackSize=10;
		pushElements=8;
		popElements=6;
		return;
	case 2://边缘测试（20分）：有限栈容量>0，入栈元素数量=栈容量，出栈元素数量=入栈元素数量
		stackSize=10;
		pushElements=10;
		popElements=10;
		return;
	case 3://[有限栈]错误测试1（5分）：入栈元素数量>栈容量 【无限栈不会报错】
		stackSize=10;
		pushElements=11;
		popElements=10;
		return;
	case 4://错误测试2（5分）：出栈元素数量>入栈元素数量 【所有栈均会报错】
		stackSize=10;
		pushElements=8;
		popElements=10;
		return;
	case 5://错误测试3（5分）：空栈直接读栈顶 【所有栈均会报错】
		stackSize=1;
		pushElements=0;
		popElements=0;
		return;
	case 6://错误测试4（5分）：初始化容量不合法（危险！使用try-catch） 【无限栈不会报错】
		stackSize=0;
		pushElements=1;
		popElements=1;
		return;
	case 7://Bonus:错误测试5（10分）：初始化容量过大，无法申请内存（危险！使用try-catch） 【无限栈不报错】
		stackSize=1000000000;
		pushElements=1;
		popElements=1;
		return;
	default:
		printf("不合法编号，重输：");
		break;
	}
	}while(cases<1 || cases>7);
}

int main(){
	int i;
	int stackSize=1,pushElements=1,popElements=1;
	testCases(stackSize, pushElements, popElements);
	try{
		FiniteStack<int> s1(stackSize);
		cout<<"[1]初始化参数测试："<<endl;
		cout<<"有限栈当前大小="<<s1.size()<<endl;
		cout<<"有限栈为空："<<(s1.isEmpty()?"yes":"no")<<endl;
		cout<<"[2]入栈测试："<<endl;
		for(i=0;i<pushElements;i++)s1.push(i);
		cout<<"有限栈当前大小="<<s1.size()<<endl;
		cout<<"有限栈为空："<<(s1.isEmpty()?"yes":"no")<<endl;
		cout<<"[3]栈顶测试："<<endl;
		printf("栈顶=%d\n",s1.getTop());
		cout<<"[4]出栈测试："<<endl;
		for(i=0;i<popElements;i++)printf("弹出%d\n",s1.pop());
		cout<<"有限栈当前大小="<<s1.size()<<endl;
		cout<<"有限栈为空："<<(s1.isEmpty()?"yes":"no")<<endl;
	}catch(int errno){
		switch(errno){
		case 1:
			printf("有限栈构造失败，原因：初始化容量为非法数值（0或负数）\n");
			break;
		case 2:
			printf("有限栈构造失败，原因：未能申请标示容量大小的内存（请更换内存或调整初始化数值）\n");
			break;
		default:
			printf("有限栈构造失败，未知原因\n");
			break;
		}
	}
	printf("\n");
	InfiniteStack<int> s2;
	cout<<"[1]初始化参数测试："<<endl;
	cout<<"无限栈的当前元素个数="<<s2.size()<<endl;
	cout<<"无限栈为空："<<(s2.isEmpty()?"yes":"no")<<endl;
	cout<<"[2]入栈测试："<<endl;
	for(i=0;i<pushElements;i++)s2.push(i);
	cout<<"无限栈的当前元素个数="<<s2.size()<<endl;
	cout<<"无限栈为空："<<(s2.isEmpty()?"yes":"no")<<endl;
	cout<<"[3]栈顶测试："<<endl;
	printf("栈顶=%d\n",s2.getTop());
	cout<<"[4]出栈测试："<<endl;
	for(i=0;i<popElements;i++)printf("弹出%d\n",s2.pop());
	cout<<"无限栈的当前元素个数="<<s2.size()<<endl;
	cout<<"无限栈为空："<<(s2.isEmpty()?"yes":"no")<<endl;
	return 0;
}
