#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sys/types.h>
#include "dirent.h"
#include <errno.h>
#include <vector>
#include <string>

using namespace std;

#pragma warning(disable: 4996)

#define ENCRYPT
//#define DECRYPT

void encrypt_data(FILE* input_file, FILE* output_file, char* key, char* key2, char* key3)
{
	int key_count = 0; //Used to restart key if strlen(key) < strlen(encrypt)
	int key_count2 = 0;
	int key_count3 = 0;
	int encrypt_byte;

	//Loop through each byte of file until EOF
	while( (encrypt_byte = fgetc(input_file)) != EOF) 
	{
		//XOR the data and write it to a file
		fputc(encrypt_byte ^ key[key_count], output_file);

		//Increment key_count and start over if necessary
		key_count++;
		if(key_count == strlen(key))
			key_count = 0;
	}
}


char* readFileBytes(const char *name, int &outlen)  
{  
	ifstream fl(name);  
	fl.seekg( 0, ios::end );  
	size_t len = fl.tellg();  
	char *ret = new char[len];  
	fl.seekg(0, ios::beg);  
	fl.read(ret, len);  
	fl.close();  
	outlen = len;
	return ret;  
}  

/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}



int main()
{
	string dir = string("guids");
	vector<string> files = vector<string>();
	vector<string> rfiles = vector<string>();

	getdir(dir,files);

	for (unsigned int i = 0;i < files.size();i++) {
		if(files[i] != "." && files[i] != "..")
		{
			string hardwareGuid = "";
			string nick = "";
			string filename = "guids/" + files[i];			

			printf("%s\n", filename.c_str());

			int outlen = 0;
			char * guidFile = readFileBytes(filename.c_str(), outlen);
			for(int i = 0; i < outlen; i++)
			{
				char t[5];
				sprintf(t, "%c", guidFile[i] ^ 0x42);
				hardwareGuid.append(t);
			}

			char * guid = new char[hardwareGuid.length()];
			guid = (char *)hardwareGuid.c_str();	


			FILE* input;
			FILE* output;

			string pathtofile;

			//Open input and output files
#ifdef ENCRYPT
			input = fopen("Z-Bot.exe", "rb");
			pathtofile = nick;
			output = fopen(files[i].c_str(), "wb");
#endif
#ifdef DECRYPT
			pathtofile = nick.append(".bih");
			input = fopen(pathtofile.c_str(), "rb");
			output = fopen("Z-Bot.exe", "wb");
#endif

			//Check input file
			if (input == NULL)
			{
				printf("Input file cannot be read.\n");
			}

			//Check output file
			if (output == NULL)
			{
				printf("Output file cannot be written to.\n");
			}

			char * key3 = new char[hardwareGuid.length()];
			key3 = guid;
			for(int i = 0; i < hardwareGuid.length(); i++)
			{
				key3[i] = key3[i] ^ 0x66;
			}
			encrypt_data(input, output, (char *)guid, (char *)nick.c_str(), key3);

			fclose(input);
			fclose(output);
		}
	}	

	getchar();

	return 0;
}

