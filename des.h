
#pragma once

#include <iostream>
#include <algorithm>    //  transform������ģʽǿ��ת��д
#include "constvar.h"
#include "strConversion.h"

string        _getPlText();
plainTextMode _getPlMode();
operateMode   _getOpMode();
//encodeMode    _getEnMode();
encodeMode    _getEnMode(string sent);

//����ԭʼ��Կ����16������Կ�� 
void getKeyTable(const bitset<SIZE_INPUT>& key, bitset<SIZE_SONKEY> Ki[]);
string _getText( char * filename);//�����ļ�������ļ����� 
void write(char * filename,string text);//��textд���ļ� 

void myDES(bitset<SIZE_INPUT>& plaintext, bitset<SIZE_SONKEY> Ki[],
    bitset<SIZE_OUTPUT>& ciphertext, operateMode opMode);//DES���� 

string DES(string str_plText, plainTextMode plMode, bitset<SIZE_SONKEY> Ki[],
	operateMode opMode, encodeMode enMode,char * IVname);//CBC��ECB��OFB 
	
//CFB 
string DES_CFB(string str_plText, plainTextMode plMode, bitset<SIZE_SONKEY> Ki[],
 operateMode opMode,char * IVname);

template <typename Input, typename Output>
void DES_Permutation(const Input& input, Output& output
        , const Byte Table[], const Byte tableSize)//�û� 
{
    for(Byte i = 0; i < tableSize; ++i)
        output[tableSize - i - 1] = input[input.size() - Table[i]];
}

template <typename Input>
void DES_LeftRotation(Input& bs, Byte count)//  ��ת����
{
    while( count-- )
    {
        Byte bit = bs[SIZE_DIVIDE - 1];
        bs <<= 1;
        bs[0] = bit;
    }
}


