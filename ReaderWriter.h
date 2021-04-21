#pragma once

#include <windows.h>
#include <string>
#include <stdexcept>

using namespace std;

class ReaderWriter
{
private:
	HANDLE com;
	int characterCount = 1;
	unsigned long characterSize = sizeof(char);
public:
	LPWSTR convertToLPWSTR(const string& s);

	ReaderWriter(std::string portName);
	~ReaderWriter();

	void write(const char data);
	char read();
};

class OpeningPortError : public std::runtime_error {
public:
	OpeningPortError(const std::string& what_arg) : std::runtime_error(what_arg) {}
};
//class ConnectionBrokenError : public std::runtime_error {
//public:
//	ConnectionBrokenError(const std::string& what_arg) : std::runtime_error(what_arg) {}
//};
