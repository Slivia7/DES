//#include "des.h"
//#include "constvar.h"
//#include <time.h>
//
////  playruneterra.com
////DES.exe -p des_plain.txt -k key.txt -v IV.txt -m CBC -c des_cipherefile.txt
//
//int main(int argc, char* argv[])
//{
//	if(argc<9){
//		cout<<"��������ȷ��������"<<endl;
//		return 1;
//	}
//    cout << "------------------DES CODE------------------" << endl;
//	string key =strHex_To_strBinary( _getText(argv[4]));
//	bitset<SIZE_SONKEY> Ki[NUM_SONKEY];
//	//���ԭʼ��Կ�������ʼ��Կ��ѡ���û����ɳ�ʼC0��D0 ������16������Կ 
//	getKeyTable(bitset<SIZE_INPUT>(key), Ki);
//
//	    plainTextMode plMode = HEX;//ѡ������ģʽ 
// 		string str_plText    =_getText(argv[2]);
//	    operateMode   opMode = ENCODE;//ѡ����ܻ����
//		encodeMode    enMode = _getEnMode(argv[8]);//ѡ����ģʽ 
//		
//        string str_hexCpText;
//        if( enMode == CFB )
//            str_hexCpText = DES_CFB(str_plText, plMode, Ki, opMode,argv[6]);
//        else
//	        str_hexCpText = DES(str_plText, plMode, Ki, opMode, enMode,argv[6]);
//    	cout << "str_hexCpText : " << str_hexCpText << endl << endl; 
//    	write("des_Chipher.txt",str_hexCpText);//������д���ļ� 
//    return 0;
//}


