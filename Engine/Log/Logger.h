#pragma once

#include <fstream>

class Logger
{
private:
	Logger() {}

public:
	~Logger();

	static Logger& GetInstance();

	void Initialize(const char* filename);
	void Release();

	void Write(const char* message);

private:
	std::ofstream logFile;
};
