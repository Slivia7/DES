
#include<fstream>

#include<string>

#include<iostream>

//fseek������ �����趨�ļ��ĵ�ǰ��дλ��.
//����ԭ��: int fseek(FILE *fp,long offset,int origin);
//��������:��fp���ļ���дλ��ָ���Ƶ�ָ����λ��.
//fseek(fp,20,SEEK_SET); ��˼�ǰ�fp�ļ���дλ��ָ����ļ���ʼ����20���ֽ�.

//7.fgets() 
//���������ж�һ�л�ָ�����ַ���ԭ����char *fgets(char *s, int n, FILE *stream); 
//�����ж�ȡn-1���ַ������Ƕ���һ�У�����s���������ַ���������ɹ��򷵻�s��ָ�룬���򷵻�NULL�� 
//
//using namespace std;
//int main(){
//	ifstream file("random1.txt");//�����뷽ʽ���ļ� 
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
//QueryPerformanceFrequency(&feq);//ÿ���������� 
//QueryPerformanceCounter(&t1);//��ǰ�������� 
//for(int i = 0;i<10000;i++);
//QueryPerformanceCounter(&t2);//����������� 
//double d=1000*((double)t2.QuadPart-(double)t1.QuadPart)/((double)feq.QuadPart);//ʱ����� 
//std::cout<<d<<std::endl;
//}
