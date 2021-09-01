#include "des.h"
#include <fstream>  
#include <string>  
#include <typeinfo>

string _getText( char * filename)//读文件 
{
	string str_plText;
	ifstream in(filename);    
    string line;  
    if(in) // 有该文件  
    {  
        while (getline (in, line)) // line中不包括每行的换行符  
        {   
            str_plText+=line;  
        }  
    }  
    else // 没有该文件  
    {  
        cout <<"no such file" << endl;  
    }   
    in.close();
//    cout<<filename<<"："<<str_plText<<endl;
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
//		cout << "（选择了CBC模式！）" << endl;
		return CBC;
	}else if( enMode == "CFB" ){
//		cout << "（选择了CFB模式！）" << endl;
		return CFB;
	}else if( enMode == "OFB" ){
//		cout << "（选择了OFB模式！）" << endl;
		return OFB;
	}else{
//		cout << "（选择了ECB模式！）" << endl;
		return ECB;
	}
}

void getKeyTable(const bitset<SIZE_INPUT>& key, bitset<SIZE_SONKEY> Ki[])
{
//	cout << "原始密钥：" << key << "\n\n";
    //  交换生成56bit的K
    bitset<SIZE_DIVIDE*2> K;
    DES_Permutation(key, K, PC_Table1, SIZE_DIVIDE*2);
    string str_K = K.to_string();
    //  选择置换生成初始C0, D0
    bitset<SIZE_DIVIDE> Ci(str_K.substr(0, SIZE_DIVIDE));
    bitset<SIZE_DIVIDE> Di(str_K.substr(SIZE_DIVIDE));
    //  生成16个子密钥
    for(Byte i = 0; i < NUM_SONKEY; ++i){
        //  旋转左移
        DES_LeftRotation(Ci, LeftMove_Table[i]);
        DES_LeftRotation(Di, LeftMove_Table[i]);
        //  合并置换
        bitset<2*SIZE_DIVIDE> bs_merge(Ci.to_string() + Di.to_string());
        DES_Permutation(bs_merge, Ki[i], PC_Table2, SIZE_SONKEY);
//        cout << "K[" << i+1 << "] = " << Ki[i] << endl;
    }
    cout << endl;
}

//  true 加密， false 解密
void myDES(bitset<SIZE_INPUT>& plaintext, bitset<SIZE_SONKEY> Ki[],
    bitset<SIZE_OUTPUT>& ciphertext, operateMode opMode){
    //  明文初始置换
    bitset<SIZE_OUTPUT> IP;
    DES_Permutation(plaintext, IP, InitPerm_Table, SIZE_INPUT);
    //  生成初始L0, R0
    string str_IP = IP.to_string();
    bitset<SIZE_INPUT/2> Li(str_IP.substr(0, SIZE_INPUT/2)), tmpL;
    bitset<SIZE_INPUT/2> Ri(str_IP.substr(SIZE_INPUT/2))   , tmpR;

    //  16次加密
    for(Byte i = 0; i < NUM_SONKEY; ++i){
        tmpL = Li; tmpR = Ri;
        bitset<SIZE_SONKEY> Ei; //  保存中间结果
        //  扩展至48bit保存至Ei
        DES_Permutation(Ri, Ei, Expansion_Table, SIZE_SONKEY);
        //  与密钥Ki异或
        if( opMode == ENCODE ) Ei ^= Ki[i];
        else Ei ^= Ki[15 - i];
        //  res暂存转化后的字符串
        //  转化时以6位为1组，映射到S盒
        string res = "";
        for(Byte j = 0; j < SIZE_SONKEY; j += 6){
            string str_col = Ei.to_string().substr(j+1, 4);
            //  转化行、列
            Byte row = Ei[SIZE_SONKEY-j-1]*2 + Ei[SIZE_SONKEY-j-6];
            Byte col = bitset<4>(str_col).to_ulong();
            //  获取S-BOX的值
            Byte value = SBox_Table[j/6][row][col];
            //printf("%d  ", value);
            res += bitset<4>(value).to_string();
        }
        //  再转换回32bit
        bitset<32> bs_tmp(res);
        //  P盒置换
        DES_Permutation(bs_tmp, Ri, PBox_Table, SIZE_INPUT/2);
        Ri ^= tmpL;
        //  最后一轮不用再交换
        Li  = tmpR;
    }
    bitset<64> res(Ri.to_string() + Li.to_string());
    DES_Permutation(res, ciphertext, FinalPerm_Table, SIZE_OUTPUT);
}

string DES(string str_plText, plainTextMode plMode, bitset<SIZE_SONKEY> Ki[],operateMode opMode, encodeMode enMode,char * IVname){
	bitset<SIZE_INPUT> IV(strHex_To_strBinary(_getText(IVname))); //  初始 IV
    int fillSize = strFillZero(str_plText, plMode);    //  8字节为一组，不足的填充0
    string str_binaryPlText = getStrBinary(str_plText, plMode);  //	获取明文的二进制流 
    string str_binaryCpText = "";    //  密文二进制流输出
    for(int i = 0; i < str_binaryPlText.size(); i += SIZE_INPUT)
    {
		//  每次截取64bit明文进行加密
		string sub_binary = str_binaryPlText.substr(i, SIZE_INPUT);
		bitset<SIZE_INPUT>  plaintext(sub_binary);
		bitset<SIZE_OUTPUT> ciphertext;

		if( enMode == ECB ){	//  ECB 电子密本模式
			myDES(plaintext, Ki, ciphertext, opMode);
		}
		else if( enMode == CBC )	//  CBC 密文分组链接模式
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
        else if( enMode == OFB ){//  OFB 输出反馈模式
        	//  OFB模式加/解密时使用密钥Ki的顺序一致
            myDES(IV, Ki, ciphertext, ENCODE);
            IV = ciphertext;
            ciphertext ^= plaintext;
        }
//		cout << "密文 : " << ciphertext << endl << endl;
		//  64bit明文加密后的密文累加
        str_binaryCpText += ciphertext.to_string();
    }
    //  OFB模式需要舍去填充的位
    if( enMode == OFB ){
    	int size = str_binaryCpText.size();
    	str_binaryCpText = str_binaryCpText.substr(0, size - fillSize);
	}
    return strBinary_To_hex(str_binaryCpText);
}

//  CFB 密文反馈模式
//  CFB一次操作的是流密码(8位)，与其它模式冲突
//  强行抽象需要写很多的判断，故单独写一个函数
string DES_CFB(string str_plText, plainTextMode plMode, bitset<SIZE_SONKEY> Ki[]
    , operateMode opMode,char * IVname)
{
	//  初始 IV
	bitset<SIZE_INPUT> IV(strHex_To_strBinary(_getText(IVname)));
   //	获取明文的二进制流
    string str_binaryPlText = getStrBinary(str_plText, plMode);
    //  8位补齐
    int size = str_binaryPlText.size() / 8;
    if( 8*size != str_binaryPlText.size() )
    	str_binaryPlText += string((size+1)*8 - str_binaryPlText.size(), '0'); 
    //  密文二进制流输出
    string str_binaryCpText = "";
    for(int i = 0; i < str_binaryPlText.size(); i += 8)
    {
		//  每次截取8bit明文进行加密
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


