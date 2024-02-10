#include "Logger.h"

#include "Engine/EngineUtility.h"

#include <chrono>
#include <ctime>

Logger::~Logger()
{
}

Logger& Logger::GetInstance()
{
	static Logger instance;
	return instance;
}

void Logger::Initialize(const char* filename)
{
	logFile.open(filename, std::ios::app);

	if (!logFile)
		DebugEngineTrap();
}

void Logger::Release()
{
	if (logFile) logFile.close();
}

void Logger::Write(const char* message)
{
	if (!logFile) return;

	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	logFile << std::ctime(&now) << message << "\n";
}
