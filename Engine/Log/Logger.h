#pragma once

#include <fstream>
#include <shared_mutex>

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
	std::shared_mutex logMutex;
};
