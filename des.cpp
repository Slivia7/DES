#include "des.h"
#include <fstream>  
#include <string>  
#include <typeinfo>

string _getText( char * filename)//���ļ� 
{
	string str_plText;
	ifstream in(filename);    
    string line;  
    if(in) // �и��ļ�  
    {  
        while (getline (in, line)) // line�в�����ÿ�еĻ��з�  
        {   
            str_plText+=line;  
        }  
    }  
    else // û�и��ļ�  
    {  
        cout <<"no such file" << endl;  
    }   
    in.close();
//    cout<<filename<<"��"<<str_plText<<endl;
	return str_plText;
}


void write(char * filename,string text) {
      ofstream ofs;
      ofs.open(filename, ios::out);
      ofs <<text<< endl;
      ofs.close();
 }

encodeMode  _getEnMode(string sent)
{
	string enMode=sent;
	transform(enMode.begin(), enMode.end(), enMode.begin(), ::toupper);
	if( enMode == "CBC" ){
//		cout << "��ѡ����CBCģʽ����" << endl;
		return CBC;
	}else if( enMode == "CFB" ){
//		cout << "��ѡ����CFBģʽ����" << endl;
		return CFB;
	}else if( enMode == "OFB" ){
//		cout << "��ѡ����OFBģʽ����" << endl;
		return OFB;
	}else{
//		cout << "��ѡ����ECBģʽ����" << endl;
		return ECB;
	}
}

void getKeyTable(const bitset<SIZE_INPUT>& key, bitset<SIZE_SONKEY> Ki[])
{
//	cout << "ԭʼ��Կ��" << key << "\n\n";
    //  ��������56bit��K
    bitset<SIZE_DIVIDE*2> K;
    DES_Permutation(key, K, PC_Table1, SIZE_DIVIDE*2);
    string str_K = K.to_string();
    //  ѡ���û����ɳ�ʼC0, D0
    bitset<SIZE_DIVIDE> Ci(str_K.substr(0, SIZE_DIVIDE));
    bitset<SIZE_DIVIDE> Di(str_K.substr(SIZE_DIVIDE));
    //  ����16������Կ
    for(Byte i = 0; i < NUM_SONKEY; ++i){
        //  ��ת����
        DES_LeftRotation(Ci, LeftMove_Table[i]);
        DES_LeftRotation(Di, LeftMove_Table[i]);
        //  �ϲ��û�
        bitset<2*SIZE_DIVIDE> bs_merge(Ci.to_string() + Di.to_string());
        DES_Permutation(bs_merge, Ki[i], PC_Table2, SIZE_SONKEY);
//        cout << "K[" << i+1 << "] = " << Ki[i] << endl;
    }
    cout << endl;
}

//  true ���ܣ� false ����
void myDES(bitset<SIZE_INPUT>& plaintext, bitset<SIZE_SONKEY> Ki[],
    bitset<SIZE_OUTPUT>& ciphertext, operateMode opMode){
    //  ���ĳ�ʼ�û�
    bitset<SIZE_OUTPUT> IP;
    DES_Permutation(plaintext, IP, InitPerm_Table, SIZE_INPUT);
    //  ���ɳ�ʼL0, R0
    string str_IP = IP.to_string();
    bitset<SIZE_INPUT/2> Li(str_IP.substr(0, SIZE_INPUT/2)), tmpL;
    bitset<SIZE_INPUT/2> Ri(str_IP.substr(SIZE_INPUT/2))   , tmpR;

    //  16�μ���
    for(Byte i = 0; i < NUM_SONKEY; ++i){
        tmpL = Li; tmpR = Ri;
        bitset<SIZE_SONKEY> Ei; //  �����м���
        //  ��չ��48bit������Ei
        DES_Permutation(Ri, Ei, Expansion_Table, SIZE_SONKEY);
        //  ����ԿKi���
        if( opMode == ENCODE ) Ei ^= Ki[i];
        else Ei ^= Ki[15 - i];
        //  res�ݴ�ת������ַ���
        //  ת��ʱ��6λΪ1�飬ӳ�䵽S��
        string res = "";
        for(Byte j = 0; j < SIZE_SONKEY; j += 6){
            string str_col = Ei.to_string().substr(j+1, 4);
            //  ת���С���
            Byte row = Ei[SIZE_SONKEY-j-1]*2 + Ei[SIZE_SONKEY-j-6];
            Byte col = bitset<4>(str_col).to_ulong();
            //  ��ȡS-BOX��ֵ
            Byte value = SBox_Table[j/6][row][col];
            //printf("%d  ", value);
            res += bitset<4>(value).to_string();
        }
        //  ��ת����32bit
        bitset<32> bs_tmp(res);
        //  P���û�
        DES_Permutation(bs_tmp, Ri, PBox_Table, SIZE_INPUT/2);
        Ri ^= tmpL;
        //  ���һ�ֲ����ٽ���
        Li  = tmpR;
    }
    bitset<64> res(Ri.to_string() + Li.to_string());
    DES_Permutation(res, ciphertext, FinalPerm_Table, SIZE_OUTPUT);
}

string DES(string str_plText, plainTextMode plMode, bitset<SIZE_SONKEY> Ki[],operateMode opMode, encodeMode enMode,char * IVname){
	bitset<SIZE_INPUT> IV(strHex_To_strBinary(_getText(IVname))); //  ��ʼ IV
    int fillSize = strFillZero(str_plText, plMode);    //  8�ֽ�Ϊһ�飬��������0
    string str_binaryPlText = getStrBinary(str_plText, plMode);  //	��ȡ���ĵĶ������� 
    string str_binaryCpText = "";    //  ���Ķ����������
    for(int i = 0; i < str_binaryPlText.size(); i += SIZE_INPUT)
    {
		//  ÿ�ν�ȡ64bit���Ľ��м���
		string sub_binary = str_binaryPlText.substr(i, SIZE_INPUT);
		bitset<SIZE_INPUT>  plaintext(sub_binary);
		bitset<SIZE_OUTPUT> ciphertext;

		if( enMode == ECB ){	//  ECB �����ܱ�ģʽ
			myDES(plaintext, Ki, ciphertext, opMode);
		}
		else if( enMode == CBC )	//  CBC ���ķ�������ģʽ
		{
			if( opMode == ENCODE )
				plaintext ^= IV;
			myDES(plaintext, Ki, ciphertext, opMode);
			if( opMode == ENCODE )
				IV = ciphertext;
			else{
				ciphertext ^= IV;
				IV = plaintext;
			}
		}
        else if( enMode == OFB ){//  OFB �������ģʽ
        	//  OFBģʽ��/����ʱʹ����ԿKi��˳��һ��
            myDES(IV, Ki, ciphertext, ENCODE);
            IV = ciphertext;
            ciphertext ^= plaintext;
        }
//		cout << "���� : " << ciphertext << endl << endl;
		//  64bit���ļ��ܺ�������ۼ�
        str_binaryCpText += ciphertext.to_string();
    }
    //  OFBģʽ��Ҫ��ȥ����λ
    if( enMode == OFB ){
    	int size = str_binaryCpText.size();
    	str_binaryCpText = str_binaryCpText.substr(0, size - fillSize);
	}
    return strBinary_To_hex(str_binaryCpText);
}

//  CFB ���ķ���ģʽ
//  CFBһ�β�������������(8λ)��������ģʽ��ͻ
//  ǿ�г�����Ҫд�ܶ���жϣ��ʵ���дһ������
string DES_CFB(string str_plText, plainTextMode plMode, bitset<SIZE_SONKEY> Ki[]
    , operateMode opMode,char * IVname)
{
	//  ��ʼ IV
	bitset<SIZE_INPUT> IV(strHex_To_strBinary(_getText(IVname)));
   //	��ȡ���ĵĶ�������
    string str_binaryPlText = getStrBinary(str_plText, plMode);
    //  8λ����
    int size = str_binaryPlText.size() / 8;
    if( 8*size != str_binaryPlText.size() )
    	str_binaryPlText += string((size+1)*8 - str_binaryPlText.size(), '0'); 
    //  ���Ķ����������
    string str_binaryCpText = "";
    for(int i = 0; i < str_binaryPlText.size(); i += 8)
    {
		//  ÿ�ν�ȡ8bit���Ľ��м���
		string sub_binary = str_binaryPlText.substr(i, 8);
		bitset<8>  plaintext(sub_binary);
		bitset<SIZE_OUTPUT> IV_DES;
		
		myDES(IV, Ki, IV_DES, ENCODE);
        bitset<8> ciphertext(IV_DES.to_string().substr(0, 8));
        ciphertext ^= plaintext;
        str_binaryCpText += ciphertext.to_string();

        IV <<= 8;
        if( opMode == ENCODE )
            IV |= bitset<64>( ciphertext.to_string() );
        else
            IV |= bitset<64>( plaintext.to_string()  );
	}
    return strBinary_To_hex(str_binaryCpText);
}


