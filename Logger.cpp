#include "Logger.h"
#include <iostream>
#include <windows.h>
#include <chrono>
#include <ctime>


std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeAsString()
{
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::string output(30, '\0');

	struct tm timeinfo;

	localtime_s(&timeinfo, &now);

	std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", &timeinfo);

	return output;
}

void Logger::Log(const std::string& message)
{
	LogEntry logEntry;

	logEntry.type = LOG_INFO;

	logEntry.message = "LOG: [" + CurrentDateTimeAsString() + "]: " + message;


	std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;

	messages.push_back(logEntry);

}

void Logger::Error(const std::string& message)
{
	LogEntry logEntry;

	logEntry.type = LOG_ERROR;

	logEntry.message = "ERROR: [" + CurrentDateTimeAsString() + "]: " + message;

	std::cerr << "\x1B[31m" << logEntry.message << "\033[0m" << std::endl;

	messages.push_back(logEntry);
}

void Logger::Warn(const std::string& message)
{
	LogEntry logEntry;

	logEntry.type = LOG_WARNING;

	logEntry.message = "WARNING: [" + CurrentDateTimeAsString() + "]: " + message;

	std::cout << "\x1B[33m" << logEntry.message << "\033[0m" << std::endl;

	messages.push_back(logEntry);
}

