#pragma once

#include "constvar.h"
#include <bitset>
#include <string>
using namespace std;

int strFillZero(string& str, plainTextMode plMode);//填充明文 
string str_To_strBinary(const string& str);//字符串转二进制 
string strBinary_To_hex(const string& str_binary);//二进制转16进制 
string strHex_To_strBinary(const string& str_hex);//十六进制转二进制 
string getStrBinary(const string& str_plText, plainTextMode plMode);


