#include "Logger.h"
#include <iostream>
#include <windows.h>
#include <chrono>
#include <ctime>

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
	// Green
	/*HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 2);
	
	std::cout << "LOG:[" << CurrentDateTimeAsString() << "]: " << message << std::endl;*/

	std::string output = "LOG: [" + CurrentDateTimeAsString() + "]: " + message;

	std::cout << "\x1B[32m" << output << "\033[0m" << std::endl;


}

void Logger::Error(const std::string& message)
{
	/* Red
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 4);
	std::cerr << "ERROR:[" << CurrentDateTimeAsString() << "]: " << message << std::endl;*/

	std::string output = "ERROR: [" + CurrentDateTimeAsString() + "]: " + message;
	std::cout << "\x1B[31m" << output << "\033[0m" << std::endl;
}

void Logger::Warn(const std::string& message)
{
	std::string output = "WARNING: [" + CurrentDateTimeAsString() + "]: " + message;

	std::cout << "\x1B[33m" << output << "\033[0m" << std::endl;
}

