#include "Debug.h"
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <cstring>


using namespace std;

string Debug::m_logfile;



void Debug::Log(std::string logstring, ...)
{
	/*typedef union {
		char c;
		int i;
		float f;
		const char * s;
	} argtype;*/
	std::string out = "";
	int substr_start = 0;
	va_list val;
	auto cstr = logstring.c_str();
	va_start(val, logstring);
	int i = 0;
	for (i = 0; cstr[i] != '\0'; ++i) {		
		if (cstr[i]=='%') {	

			switch (cstr[i+1]) {
			case '\0':
				i--; // because we always increment at bottom
				break;
				case 'i':
					out.append(logstring.substr(substr_start, i - substr_start));
					substr_start = i;
					out.append(to_string(va_arg(val, int)));
					break;
				case 'f':
					out.append(logstring.substr(substr_start, i - substr_start));
					substr_start = i;
					out.append(to_string(va_arg(val, float)));
					break;
				case 'c':
					out.append(logstring.substr(substr_start, i - substr_start));
					substr_start = i;
					out.append(to_string(va_arg(val, char)));
					break;
				case 's':
					out.append(logstring.substr(substr_start, i - substr_start));
					substr_start = i;
					const char * arg;
					arg = va_arg(val, const char *);
					out.append(arg);
					break;
			}
			++i;			
		}
	}
	va_end(val);
	if (i > substr_start) {
		out.append(logstring.substr(substr_start, i - substr_start));
	}

	
	if (!Debug::m_logfile.empty()) {
		Debug::WriteToLogFile(m_logfile, out);
	} else {
		cout << out << endl;
	}
}

bool Debug::SetLogFile(std::string logfile)
{
	ofstream fs(logfile);
	if (!fs) {
		std::cerr << "Cannot open logfile:" << logfile << endl;
		return false;
	} else {
		m_logfile = logfile;
		fs.close();
		return true;
	}
}

void Debug::WriteToLogFile(std::string logfile, std::string logstring) 
{
	ofstream fs(logfile, ios::app);
	if (!fs) {
		std::cerr << "Cannot open logfile:" << logfile << endl;
	}
	else {		
		fs << logstring << endl;
		fs.close();	
	}
}

Debug::Debug()
{
	Debug::m_logfile = string();
}


Debug::~Debug()
{
}
