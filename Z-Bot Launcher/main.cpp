#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <WinInet.h>
#include <Urlmon.h>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <sstream>

struct TestEOL
{
	bool operator()(char c)
	{
		last    = c;
		return last == '\n';
	}
	char    last;
};


using namespace std;

#pragma warning(disable: 4996)
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "urlmon.lib")

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

int main()
{
	SetConsoleTitle("Z-Bot Launcher");

	// First get the unique GUID
	HW_PROFILE_INFO hwProfileInfo;
	string hardwareGuid = "";
	string nick = "";
	
	// Base URL for downloading .bih datafiles
	string baseURL = "http://mafioso.nihplod.com/";

	// Get the number of multies
	std::fstream file("accounts.txt");

	TestEOL       test;
	std::size_t   imultiCount   = std::count_if(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), test);
	
	if (test.last != '\n')  
	{                       
		imultiCount++;            
	}       

	std::string multiCount;
	std::stringstream out;
	out << imultiCount;
	multiCount = out.str();

	// Insert URL
	string insertURL = "http://mafioso.nihplod.com/zbot.php?action=insert&m=" + multiCount + "&nick=";

	// Go offline URL
	string goOfflineURL = "http://mafioso.nihplod.com/zbot.php?action=goOffline&m=" + multiCount + "&nick=";	 

	// Temp path
	char ctempPath[555];
	GetTempPath(555, ctempPath);
	string tempPath = ctempPath;
	string tempPath2 = tempPath;

	// Get nick
	char cnick[256];
	printf("Please enter your nickname: ");
	cin >> cnick;
	printf("\n");
	nick = cnick;
	insertURL += nick;
	goOfflineURL += nick;	
	// Open internet handle
	hINet = InternetOpen("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );

	// Let the server know we came online
	InternetOpenUrl(hINet, (char*)insertURL.c_str(), NULL, 0, 0, 0);
	printf("Authenticating ...\n");

	// Get hardware GUID
	if(GetCurrentHwProfile(&hwProfileInfo) != NULL)
	{
		hardwareGuid = hwProfileInfo.szHwProfileGuid;
	}

	hardwareGuid = hardwareGuid.substr(1, hardwareGuid.length() - 2);
	char * guid = new char[hardwareGuid.length()];
	guid = (char *)hardwareGuid.c_str();	

	FILE* input;
	FILE* output;

	string pathtofile = tempPath.append(nick).append(".bih");

	string dlURL  = baseURL.append(nick).append(".bih").append("?do=yes");

	printf("Getting ready ...\n");
	printf("Please wait a few seconds (max. 30 - close if it timeouts) ...\n");
	URLDownloadToFile(0, dlURL.c_str(), pathtofile.c_str(), 0, 0);
	printf("Got ready ...\n");

	input = fopen(pathtofile.c_str(), "rb");

	string dlToURL = tempPath2; dlToURL.append("zb.exe");
	output = fopen(dlToURL.c_str(), "wb");

	//Check input file
	if (input == NULL)
	{
		return 0;
	}

	char * key3 = new char[hardwareGuid.length()];
	key3 = guid;
	for(int i = 0; i < (int)hardwareGuid.length(); i++)
	{
		key3[i] = key3[i] ^ 0x66;
	}
	encrypt_data(input, output, (char *)guid, (char *)nick.c_str(), key3);

	fclose(input);
	fclose(output);

	string accounts = tempPath2; accounts.append("accounts.txt");
	string captcha = tempPath2; captcha.append("captcha.wav");
	string curl = tempPath2; curl.append("curl.exe");
	string mshtml = tempPath2; mshtml.append("microsoft.mshtml.dll");
	string proxy = tempPath2; proxy.append("proxy.txt");
	string decaptcher = tempPath2; decaptcher.append("decaptcher.dll");
	CopyFile("accounts.txt", accounts.c_str(), false);
	CopyFile("captcha.wav", captcha.c_str(), false);
	CopyFile("curl.exe", curl.c_str(), false);
	CopyFile("microsoft.mshtml.dll", mshtml.c_str(), false);
	CopyFile("proxy.txt", proxy.c_str(), false);
	CopyFile("decaptcher.dll", decaptcher.c_str(), false);
	printf("Now doing it ...\n");
	HWND hWnd = GetConsoleWindow();
	ShowWindow( hWnd, SW_HIDE );
	system(dlToURL.append(" ").append(nick).c_str());
	remove(pathtofile.c_str()); 
	remove(dlToURL.c_str());
	remove(accounts.c_str());
	remove(captcha.append("captcha.wav").c_str());
	remove(mshtml.c_str());
	remove(curl.c_str());
	InternetOpenUrl(hINet, (char*)goOfflineURL.c_str(), NULL, 0, 0, 0);

	return 0;
}
