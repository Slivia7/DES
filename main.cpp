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
//		cout<<"请输入正确的行命令"<<endl;
//		return 1;
//	}
//    cout << "------------------DES CODE------------------" << endl;
//	string key =strHex_To_strBinary( _getText(argv[4]));
//	bitset<SIZE_SONKEY> Ki[NUM_SONKEY];
//	//获得原始密钥，输出初始密钥，选择置换生成初始C0，D0 ，生成16个子密钥 
//	getKeyTable(bitset<SIZE_INPUT>(key), Ki);
//
//	    plainTextMode plMode = HEX;//选择明文模式 
// 		string str_plText    =_getText(argv[2]);
//	    operateMode   opMode = ENCODE;//选择加密或解密
//		encodeMode    enMode = _getEnMode(argv[8]);//选操作模式 
//		
//        string str_hexCpText;
//        if( enMode == CFB )
//            str_hexCpText = DES_CFB(str_plText, plMode, Ki, opMode,argv[6]);
//        else
//	        str_hexCpText = DES(str_plText, plMode, Ki, opMode, enMode,argv[6]);
//    	cout << "str_hexCpText : " << str_hexCpText << endl << endl; 
//    	write("des_Chipher.txt",str_hexCpText);//将密文写入文件 
//    return 0;
//}


