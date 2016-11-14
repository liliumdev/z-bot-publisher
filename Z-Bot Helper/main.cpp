#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <WinInet.h>

using namespace std;

#pragma warning(disable: 4996)
#pragma comment(lib, "wininet.lib")

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

HINTERNET hINet;

void OpenURL(char * URL)
{
	InternetOpenUrl(hINet, URL, NULL, 0, 0, 0);
}

int main()
{
	// First get the unique GUID
	HW_PROFILE_INFO hwProfileInfo;
	string hardwareGuid = "";	

	if(GetCurrentHwProfile(&hwProfileInfo) != NULL)
	{
		hardwareGuid = hwProfileInfo.szHwProfileGuid;
	}

	hardwareGuid = hardwareGuid.substr(1, hardwareGuid.length() - 2);

	char * guid = new char[hardwareGuid.length()];
	guid = (char *)hardwareGuid.c_str();	

	for(int i = 0; i < hardwareGuid.length(); i++)
	{
		guid[i] = guid[i] ^ 0x42;
	}

	FILE * pFile;
	pFile = fopen("data.bih", "wb");

	for(int i = 0; i < hardwareGuid.length(); i++)
	{
		fprintf(pFile, "%c", guid[i]);
	}

	fclose (pFile);
	
	printf("Thank you! Please send the file \"data.bih\" to the author !");

	getchar();

	return 0;
}

