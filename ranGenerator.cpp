
#include<fstream>

#include<string>

#include<iostream>

//fseek函数是 用来设定文件的当前读写位置.
//函数原型: int fseek(FILE *fp,long offset,int origin);
//函数功能:把fp的文件读写位置指针移到指定的位置.
//fseek(fp,20,SEEK_SET); 意思是把fp文件读写位置指针从文件开始后移20个字节.

//7.fgets() 
//　　从流中读一行或指定个字符，原型是char *fgets(char *s, int n, FILE *stream); 
//从流中读取n-1个字符，除非读完一行，参数s是来接收字符串，如果成功则返回s的指针，否则返回NULL。 
//
//using namespace std;
//int main(){
//	ifstream file("random1.txt");//以输入方式打开文件 
//	string str;
//	getline(file, str);
//	file.close();
//	cout<<str;
//}
//#include <windows.h>
//#include <iostream>
//
//int main(){
//LARGE_INTEGER t1,t2,feq; 
//QueryPerformanceFrequency(&feq);//每秒跳动次数 
//QueryPerformanceCounter(&t1);//测前跳动次数 
//for(int i = 0;i<10000;i++);
//QueryPerformanceCounter(&t2);//测后跳动次数 
//double d=1000*((double)t2.QuadPart-(double)t1.QuadPart)/((double)feq.QuadPart);//时间差秒 
//std::cout<<d<<std::endl;
//}
