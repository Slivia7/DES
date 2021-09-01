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
int generator(string filename)//随机生成5M文件 
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
		cout<<"随机生成5M测试数据，存储到"<<filename<<endl; 
		writeFile.close();
	}
	   return 0;
}

int main()
{	
	string filename;
	cout<<"随机生成5M测试数据到：";
	cin>>filename; 
	char files[10];
    strcpy(files, filename.c_str());
	generator(filename);
	ifstream file(files);//以输入方式打开文件 
	string key =strHex_To_strBinary( _getText("key.txt"));
	bitset<SIZE_SONKEY> Ki[NUM_SONKEY];
	//获得原始密钥，输出初始密钥，选择置换生成初始C0，D0 ，生成16个子密钥 
	getKeyTable(bitset<SIZE_INPUT>(key), Ki);
	plainTextMode plMode = HEX;//选择明文模式 
  	string str_plText; 
    operateMode   opMode;//选择加密或解密
	encodeMode    enMode = _getEnMode("CFB");//选操作模式 
	
	double time_taken_en,time_taken_de,time_temp; 
	LARGE_INTEGER t1,t2,feq;
	QueryPerformanceFrequency(&feq);//每秒跳动次数 
	for(int i=1;i<=20;i++){
		string str;
		getline(file, str);
	    cout << "------------------DES CODE"<<i<<"------------------" << endl;
		cout<<"读取的第"<<i<<"组明文："<<str<<endl;
 		str_plText    =str;
	    opMode = ENCODE;//选择加密或解密
	    
		//加密 
        string str_hexCpText; 
		QueryPerformanceCounter(&t1);//测前跳动次数 
        if( enMode == CFB )
            str_hexCpText = DES_CFB(str_plText, plMode, Ki, opMode,"IV.txt");
        else
	        str_hexCpText = DES(str_plText, plMode, Ki, opMode, enMode,"IV.txt");
		QueryPerformanceCounter(&t2);//测后跳动次数 
    	cout << endl<<"加密获得的密文 : " << str_hexCpText << endl;
		time_taken_en +=time_temp; 
    	cout<<"加密用时："<<setprecision(4)<<time_temp<<"ns"<<endl<<endl; //输出加密/解密用时
		 
		//解密
		str_plText    = str_hexCpText;
		 opMode = DECODE;
		QueryPerformanceCounter(&t1);//测前跳动次数 
        if( enMode == CFB )
            str = DES_CFB(str_plText, plMode, Ki, opMode,"IV.txt");
        else
	        str = DES(str_plText, plMode, Ki, opMode, enMode,"IV.txt");
		QueryPerformanceCounter(&t2);//测后跳动次数 
		time_temp=1000*((double)t2.QuadPart-(double)t1.QuadPart)/((double)feq.QuadPart);//时间差秒 
		time_taken_de +=time_temp; 
    	cout << "加密获得的明文 : " << str << endl;
    	cout<<"解密用时："<<time_temp<<"ns"<<endl<<endl<<endl; //输出加密/解密用时 
    }
    	file.close();
    	cout<<"CFB"<<endl;
    	cout<<"加密总用时："<<time_taken_en<<"ms"<<endl; //输出加密/解密用时 
    	cout<<"解密总用时："<<time_taken_de<<"ms"<<endl; //输出加密/解密用时 
    	cout<<"加密速度："<< double(625/time_taken_en/1024)<<"MB/s"<<endl;
		//640B/time ms=640*1000/1024/1024/timeMB/s=625/1024/time
		cout<<"解密速度："<< double(625/time_taken_de/1024)<<"MB/s"<<endl;
		//640B/time ms=640*1000/1024/1024/timeMB/s=625/1024/time

    return 0;
}

