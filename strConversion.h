#pragma once

#include "constvar.h"
#include <bitset>
#include <string>
using namespace std;

int strFillZero(string& str, plainTextMode plMode);//������� 
string str_To_strBinary(const string& str);//�ַ���ת������ 
string strBinary_To_hex(const string& str_binary);//������ת16���� 
string strHex_To_strBinary(const string& str_hex);//ʮ������ת������ 
string getStrBinary(const string& str_plText, plainTextMode plMode);


