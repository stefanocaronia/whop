#include <iostream>
#include <algorithm>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <windows.h>
#include "Shlwapi.h"

using namespace std;


bool loadConfig();
vector<string> split(string, char);
TCHAR* GetThisPath(TCHAR* dest, size_t destSize);

string file;
string HTTP_PATH="";
string SOURCE_PATH="";
const char* CONFIG_FILENAME = "whop.ini";
const int maxpath = 512;
TCHAR EXE_PATH[maxpath];

typedef map<string,string> conf;
conf CONFIG;

int main(int argc, char* argv[]) {

	if (argv[1]) file=argv[1];
    else return 0;

    loadConfig();
    if (SOURCE_PATH==""||HTTP_PATH=="") return 0;

	regex e("\\\\");   // matches words beginning by "sub"
	file = regex_replace (file,e,"/");
	file.erase(0,SOURCE_PATH.length());
	ShellExecute(NULL, "open", (HTTP_PATH+file).c_str(),  NULL, NULL, SW_SHOWNORMAL);

    return 0;
}


/* UTILITY FUNCTIONS */

bool loadConfig() {
	ifstream cfile;

	GetThisPath(EXE_PATH,maxpath);
	string exepath = EXE_PATH;
	cfile.open(exepath+"\\"+CONFIG_FILENAME);

	if (!cfile.is_open()) return false;

	string key,value,line,f;

	while(getline(cfile, line)) {
		if (line.size()==0) continue;
        f=line.substr(0,1);
        if (f=="#"||f=="/"||f=="["||f=="*") continue;
        vector<string> parts = split(line,'=');
        if (parts.size()>1) CONFIG[parts[0]]=parts[1];
    }

	cfile.close();

	try {
		if (CONFIG["HTTP_PATH"]!="") 	HTTP_PATH = CONFIG["HTTP_PATH"];
		if (CONFIG["SOURCE_PATH"]!="") 	SOURCE_PATH = CONFIG["SOURCE_PATH"];

	} catch (...) {
		return false;
	}

	return true;
}


vector<string> split(string text, char delim) {
	vector<string> elements;
	stringstream stream(text);
	string item;
	while (getline(stream, item, delim)) {
		item.erase(0, item.find_first_not_of(" \n\r\t"));
		item.erase(item.find_last_not_of(" \n\r\t")+1);
		elements.push_back(item);
	}
	return elements;
}

TCHAR* GetThisPath(TCHAR* dest, size_t destSize)
{
    if (!dest) return NULL;
    if (MAX_PATH > destSize) return NULL;

    GetModuleFileName(NULL, dest, destSize);
    PathRemoveFileSpec(dest);
    return dest;
}

