#include<iostream>
#include<sstream>
#include<fstream>
#include <bitset>
#include "md5.h"
using namespace std;

#define BUFFER_SIZE 4

bool GetMD5Key(string key,uint32_t md5_key);
int ReadBuffer(FILE *f,char buffer[BUFFER_SIZE]);
int WriteBuffer(FILE *f,char buffer[BUFFER_SIZE],int size);
bool Encrypt(FILE* input_file,FILE* output_file);
bool Decrypt(FILE* input_file,FILE* output_file);

static uint32_t hex_key;

int main(int argc, char ** argv)
{
    if(argc !=5)
    {
        cout << "Invalid arguments!\nNothing to do!"<<endl;
        return 0;
    }

    bool isEncrypt = false;
    string inputFileName = "";
    string key = "";
    FILE *input_file;
    FILE *output_file;

    try
    {
        //Check the first argument
        if(((string)argv[1]).compare("encrypt")== 0)
            isEncrypt = true;
        else if (((string)argv[1]).compare("decrypt")== 0)
            isEncrypt = false;
        else
        {
            cout << "The second input should be \"encrypt\" or \"decrypt\""<<endl;
            return 0;
        }

        key = argv[2];
        input_file = fopen(argv[3], "rb");
        output_file = fopen(argv[4], "w+b");

        if(input_file == NULL)
        {
           cerr<<"Open File " << argv[3] << " Failed!\nNothing to do!"<<endl;
           return 0;
        }

    }catch(exception& e)
    {
        cout<<"Exception occurred : " <<e.what()<<endl;
        return 0;
    }

    if (!GetMD5Key(key,hex_key))
    {
        cerr <<"Get MD5 key failed !"<<endl;
        return 0;
    }

    if(isEncrypt)
    {
        if(Encrypt(input_file,output_file))
            cout<<"Encrypt Success!"<<endl;
        else
            cerr << "Encrypt Failed! "<<endl;
    }
    else
    {
        if(Decrypt(input_file,output_file))
            cout<<"Decrypt Success!"<<endl;
        else
            cerr << "Decrypt Failed! "<<endl;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}


int ReadBuffer(FILE *f,char buffer[BUFFER_SIZE])
{
    return fread(buffer,sizeof(char),BUFFER_SIZE,f);
}

int WriteBuffer(FILE *f,char buffer[BUFFER_SIZE],int size)
{
    return fwrite(buffer,sizeof(char),size,f);
}

bool Encrypt(FILE* input_file,FILE* output_file)
{
    int size = 0;
    char buffer[BUFFER_SIZE];

    while(size = ReadBuffer(input_file,buffer))
    {
        (*(uint32_t*)buffer) ^= hex_key;
        WriteBuffer(output_file,buffer,size);
    }
    return true;
}

bool Decrypt(FILE* input_file,FILE* output_file)
{
    int size = 0;
    char buffer[BUFFER_SIZE];

    while(size = ReadBuffer(input_file,buffer))
    {
        (*(uint32_t*)buffer) ^= hex_key;
        WriteBuffer(output_file,buffer,size);
    }
    return true;
}

bool GetMD5Key(string key,uint32_t md5_key)
{
    try
    {
        string afterMD5S = md5(key);
        string first4byte = afterMD5S.substr(0,8);

        stringstream ss;
        ss << hex << first4byte;
        ss >> md5_key;
        ss.clear();

        hex_key = md5_key;

        return true;
    }catch(exception& e)
    {
        cerr << "Exception occurred : " << e.what()<<endl;
        return false;
    }
}









