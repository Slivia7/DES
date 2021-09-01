#include "des.h"
#include "constvar.h"
//  playruneterra.com
//TEST.exe -p des_plain.txt -k key.txt -v IV.txt -m CBC -c des_cipherefile.txt
#include "des.h"
#include <fstream>  
#include <string>  
#include <typeinfo>
#include <time.h>
#include <iomanip>
#include <windows.h>
#include <iostream>
#include<string>
int generator(string filename)//�������5M�ļ� 
{
	char* c;
	strcpy(c,filename.c_str());
    srand((unsigned)time(NULL));
    ofstream writeFile(c,ios::app);
	if(!writeFile.is_open()){
		cout<<"no such file"<<endl;
		return 1;
	}else
	{
		for(int m=0;m<10;m++){
			for(int j=0;j<512;j++){
				for(int m=0;m<20;m++){
			    for (int i = 0; i < 32; ++i)
			    {
			        unsigned ran = rand()%16;
			        char let[] = { 'A', 'B', 'C', 'D', 'E', 'F' };
			        if (ran > 9)
			        {
			            	writeFile<<let[ran - 10];
			        }
			        else
			        {
			            writeFile<< ran;
			        }
			    }
			    writeFile<<"\n";
			}
			}
		}
		cout<<"�������5M�������ݣ��洢��"<<filename<<endl; 
		writeFile.close();
	}
	   return 0;
}

int main()
{	
	string filename;
	cout<<"�������5M�������ݵ���";
	cin>>filename; 
	char files[10];
    strcpy(files, filename.c_str());
	generator(filename);
	ifstream file(files);//�����뷽ʽ���ļ� 
	string key =strHex_To_strBinary( _getText("key.txt"));
	bitset<SIZE_SONKEY> Ki[NUM_SONKEY];
	//���ԭʼ��Կ�������ʼ��Կ��ѡ���û����ɳ�ʼC0��D0 ������16������Կ 
	getKeyTable(bitset<SIZE_INPUT>(key), Ki);
	plainTextMode plMode = HEX;//ѡ������ģʽ 
  	string str_plText; 
    operateMode   opMode;//ѡ����ܻ����
	encodeMode    enMode = _getEnMode("CFB");//ѡ����ģʽ 
	
	double time_taken_en,time_taken_de,time_temp; 
	LARGE_INTEGER t1,t2,feq;
	QueryPerformanceFrequency(&feq);//ÿ���������� 
	for(int i=1;i<=20;i++){
		string str;
		getline(file, str);
	    cout << "------------------DES CODE"<<i<<"------------------" << endl;
		cout<<"��ȡ�ĵ�"<<i<<"�����ģ�"<<str<<endl;
 		str_plText    =str;
	    opMode = ENCODE;//ѡ����ܻ����
	    
		//���� 
        string str_hexCpText; 
		QueryPerformanceCounter(&t1);//��ǰ�������� 
        if( enMode == CFB )
            str_hexCpText = DES_CFB(str_plText, plMode, Ki, opMode,"IV.txt");
        else
	        str_hexCpText = DES(str_plText, plMode, Ki, opMode, enMode,"IV.txt");
		QueryPerformanceCounter(&t2);//����������� 
    	cout << endl<<"���ܻ�õ����� : " << str_hexCpText << endl;
		time_taken_en +=time_temp; 
    	cout<<"������ʱ��"<<setprecision(4)<<time_temp<<"ns"<<endl<<endl; //�������/������ʱ
		 
		//����
		str_plText    = str_hexCpText;
		 opMode = DECODE;
		QueryPerformanceCounter(&t1);//��ǰ�������� 
        if( enMode == CFB )
            str = DES_CFB(str_plText, plMode, Ki, opMode,"IV.txt");
        else
	        str = DES(str_plText, plMode, Ki, opMode, enMode,"IV.txt");
		QueryPerformanceCounter(&t2);//����������� 
		time_temp=1000*((double)t2.QuadPart-(double)t1.QuadPart)/((double)feq.QuadPart);//ʱ����� 
		time_taken_de +=time_temp; 
    	cout << "���ܻ�õ����� : " << str << endl;
    	cout<<"������ʱ��"<<time_temp<<"ns"<<endl<<endl<<endl; //�������/������ʱ 
    }
    	file.close();
    	cout<<"CFB"<<endl;
    	cout<<"��������ʱ��"<<time_taken_en<<"ms"<<endl; //�������/������ʱ 
    	cout<<"��������ʱ��"<<time_taken_de<<"ms"<<endl; //�������/������ʱ 
    	cout<<"�����ٶȣ�"<< double(625/time_taken_en/1024)<<"MB/s"<<endl;
		//640B/time ms=640*1000/1024/1024/timeMB/s=625/1024/time
		cout<<"�����ٶȣ�"<< double(625/time_taken_de/1024)<<"MB/s"<<endl;
		//640B/time ms=640*1000/1024/1024/timeMB/s=625/1024/time

    return 0;
}

