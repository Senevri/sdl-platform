#pragma once
#include <string>

class Debug
{
public:
	static void Log(std::string logstring, ...);
	static bool SetLogFile(std::string logfile);
	

private:
	static void WriteToLogFile(std::string logfile, std::string logstring);	
	static std::string m_logfile;
	Debug();
	virtual ~Debug();
};

