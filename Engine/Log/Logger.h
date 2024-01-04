#pragma once

#include <fstream>

class Logger
{
public:
	Logger() {}
	~Logger();

	static Logger& GetInstance();

	void Initialize(const char* filename);

	void Write(const char* message);

private:
	std::ofstream logFile;
};
