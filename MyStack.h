/*
Author: aruyuna~
Date: 2018.7.31
*/

#ifndef _MYSTACK
#define _MYSTACK

#include<cstdio>
#include<iostream>
using namespace std;

#define _STACK_ACCEPTED 0							//无错误发生
#define _STACK_ERROR_TEMP_VAL -2147483648			//发生错误时弹出的值
#define _STACK_ERROR_INF_NOTTOP 101					//（无限栈）不是栈顶
#define _STACK_ERROR_INF_ISTOP 102					//（无限栈）是栈顶
#define _STACK_ERROR_INF_NOT2ND 103					//（无限栈）不是下一个栈顶（pop会导致弹出多个元素）
#define _STACK_ERROR_INF_EMPTY 104					//（无限栈）空栈
#define _STACK_ERROR_INF_MEMORY_OVERFLOW 105		//（无限栈）不能压入元素（计算机没有内存导致）
#define _STACK_ERROR_F_STACK_OVERFLOW 201			//（有限栈）溢出（已使用最大容量）
#define _STACK_ERROR_F_CONSTRUCT_FAIL 202			//（有限栈）构造失败（由于构造函数参数错误）
#define _STACK_ERROR_F_STACK_ALREADY_EMPTY 203		//（有限栈）已经为空（无法进行弹出/读取操作）

int _stack_errno=_STACK_ACCEPTED;

void _stack_print_error_message(){
	//打印错误信息（可用于cpp编码时调试，直接调用本函数即可，无需带参）
	switch(_stack_errno){
	case _STACK_ACCEPTED:
		printf("上次操作没有造成问题。\n");
		break;
	case _STACK_ERROR_INF_NOTTOP:
		printf("[无限栈]错误：不是栈顶元素。\n");
		break;
	case _STACK_ERROR_INF_ISTOP:
		printf("[无限栈]错误：是栈顶元素。\n");
		break;
	case _STACK_ERROR_INF_NOT2ND:
		printf("[无限栈]错误：不是栈顶第二元素（无法进行pop操作）。\n");
		break;
	case _STACK_ERROR_INF_EMPTY:
		printf("[无限栈]错误：空栈（无法进行弹出或查看栈顶操作）。\n");
		break;
	case _STACK_ERROR_INF_MEMORY_OVERFLOW:
		printf("[无限栈]错误：不能压入元素（计算机没有内存，空间申请失败）。\n");
		break;
	case _STACK_ERROR_F_STACK_OVERFLOW:
		printf("错误：栈溢出（已使用最大容量）。\n");
		break;
	case _STACK_ERROR_F_CONSTRUCT_FAIL:
		printf("错误：构栈失败（是否初始化栈时大小为0或负数？否则内存已满无法构栈）\n请使用try-catch块捕获有限栈构造时异常，否则会发生运行时错误而崩溃\n");
		break;
	case _STACK_ERROR_F_STACK_ALREADY_EMPTY:
		printf("错误：空栈（无法进行弹出或查看栈顶操作）\n");
		break;
	default:
		break;
	}
}

//无限制堆栈所用辅助结构：双向链表
template<typename T>
class StackLink{
private:
	T val;
	StackLink *pre;
	StackLink *next;
public:
	StackLink(int x=0){
		val=x;
		pre=NULL;
		next=NULL;
	}
	//元素入栈（后方连接新Node）
	void linkBack(T value){
		//如果当前结点不是栈顶（即该结点next是非空指针），报错
		if(next!=NULL){
			_stack_errno=_STACK_ERROR_INF_NOTTOP;
			_stack_print_error_message();
			return;
		} 
		next=new StackLink<T>(value);
		//没有给新栈顶申请到内存空间，报错
		if(next==NULL){
			_stack_errno=_STACK_ERROR_INF_MEMORY_OVERFLOW;
			_stack_print_error_message();
			return;
		}
		//把后面的pre指针指向自己（弹出栈顶时用）
		next->pre=this;
		_stack_errno=_STACK_ACCEPTED;
	}
	//弹出栈顶（栈顶指针指向倒数第2个Node，再移除倒数第1个Node，这些操作在栈类中进行，因为delete this操作麻烦风险大）
	void removeBack(){
		//你自己就是栈顶，不能弹
		if(next==NULL){
			_stack_errno=_STACK_ERROR_INF_ISTOP;
			_stack_print_error_message();
			return;
		}
		//下一个Node不是栈顶，不能弹
		if(next->next!=NULL){
			_stack_errno=_STACK_ERROR_INF_NOT2ND;
			_stack_print_error_message();
			return;
		}
		//删掉后方Node，即为弹出（此时栈顶指针会指向自己）
		delete next;
		//自己的next设空，表明栈顶
		next=NULL;
		_stack_errno=_STACK_ACCEPTED;
	}
	T getVal(){
		return val;
	}
	StackLink* getPre(){
		return pre;
	}
	StackLink* getNext(){
		return next;
	}
};

//抽象类Stack
template<typename T>
class Stack{
public:
	virtual void push(T s)=0;
	virtual T pop()=0;
	virtual T getTop()=0;
	virtual int size()=0;
	virtual bool isEmpty()=0;
};

//无限栈：元素个数无限，可以无限制压栈（只有内存堆区容量可以阻止其增长，俗称Stack Overflow
template<typename T>
class InfiniteStack: public Stack<T>{
private:
	StackLink<T> base;
	int used;
	StackLink<T> *topPointer;
public:
	InfiniteStack():used(0){
		//栈顶指针指向那个基底，表明当前栈为空
		topPointer=&base;
	}
	~InfiniteStack(){
		while(used>0){
			//析构时，弹出栈内所有元素释放空间
			topPointer=topPointer->getPre();
			topPointer->removeBack();
			used--;
		}
	}
	//入栈
	void push(T s){
		//在链后方加入新元素，表示入栈
		topPointer->linkBack(s);
		//栈顶指针更新为新压入栈的Node
		topPointer=topPointer->getNext();
		//如果成功压栈则栈内元素+1，否则压栈失败，有提示信息
		if(_stack_errno==_STACK_ACCEPTED)used++;
		else printf("压入操作失败，见上方原因\n");
	}
	//弹出栈
	T pop(){
		//栈为空，不能弹
		if(used==0){
			_stack_errno=_STACK_ERROR_INF_EMPTY;
			_stack_print_error_message();
			return _STACK_ERROR_TEMP_VAL;
		}
		//获取栈顶元素
		T res = getTop();
		//弹出栈顶：先把栈顶指针指向倒数第2个Node，再弹出倒数第1个
		topPointer=topPointer->getPre();
		topPointer->removeBack();
		//成功弹出则栈内元素-1，并返回原栈顶元素。否则失败，有提示信息。
		if(_stack_errno==_STACK_ACCEPTED){
			used--;
			return res;
		}
		else{
			printf("弹出操作失败，见上方原因\n");
			return _STACK_ERROR_TEMP_VAL;
		}
	}
	//获取栈顶元素
	T getTop(){
		//空栈，无法取元素
		if(used==0){
			_stack_errno=_STACK_ERROR_INF_EMPTY;
			_stack_print_error_message();
			return _STACK_ERROR_TEMP_VAL;
		}
		_stack_errno=_STACK_ACCEPTED;
		//栈顶元素：双链表倒数第1个Node的值
		return topPointer->getVal();
	}
	int size(){
		return used;
	}
	bool isEmpty(){
		return used==0;
	}
};

//有限栈：元素个数有限，容量在定义时提供，默认20。
template<typename T>
class FiniteStack: public Stack<T>{
private:
	T *buff;
	int maxSize;
	int used;
public:
	FiniteStack(int size=20){
		//如果size小于等于0，应该不构造这个栈，因为没有大小
		if(size<=0){
			_stack_errno=_STACK_ERROR_F_CONSTRUCT_FAIL;
			_stack_print_error_message();
			throw _STACK_ERROR_F_CONSTRUCT_FAIL;
		}
		buff=new T[size];
		//内存不足，同样构栈失败
		if(buff==NULL){
			_stack_errno=_STACK_ERROR_F_CONSTRUCT_FAIL;
			_stack_print_error_message();
			throw _STACK_ERROR_F_CONSTRUCT_FAIL;
		}
		maxSize=size;
		used=0;
	}
	~FiniteStack(){
		//释放栈所占空间
		if(_stack_errno!=_STACK_ERROR_F_CONSTRUCT_FAIL){
			delete []buff;
			buff=NULL;
		}
	}
	//入栈
	void push(T s){
		if(used>=maxSize){
			//栈满，不入，给错误提示
			_stack_errno=_STACK_ERROR_F_STACK_OVERFLOW;
			_stack_print_error_message();
			return;
		}
		//入栈，栈内元素+1
		buff[used++]=s;
		_stack_errno=_STACK_ACCEPTED;
	}
	//出栈
	T pop()
	{
		if(used<=0){
			//栈空，错误提示
			_stack_errno=_STACK_ERROR_F_STACK_ALREADY_EMPTY;
			_stack_print_error_message();
			return _STACK_ERROR_TEMP_VAL;
		}
		T res=getTop();
		//有限栈出栈时直接栈内元素-1即可，下次有元素压入该位置直接替换值并修改栈内元素个数
		used--;
		_stack_errno=_STACK_ACCEPTED;
		return res;
	}
	//取栈顶元素
	T getTop(){
		if(used<=0){
			//栈空，错误提示
			_stack_errno=_STACK_ERROR_F_STACK_ALREADY_EMPTY;
			_stack_print_error_message();
			return _STACK_ERROR_TEMP_VAL;
		}
		_stack_errno=_STACK_ACCEPTED;
		return buff[used-1];
	}
	//显示栈中元素个数
	int size(){
		return used;
	}
	//栈是否为空
	bool isEmpty(){
		return used==0;
	}
};

#endif //_MYSTACK
