#include "ReaderWriter.h"

LPWSTR ReaderWriter::convertToLPWSTR(const string& s)
{
	LPWSTR ws = new wchar_t[s.size() + 1]; // +1 for zero at the end
	copy(s.begin(), s.end(), ws);
	ws[s.size()] = 0; // zero at the end
	return ws;
}

ReaderWriter::ReaderWriter(std::string portName)
{
	LPWSTR selectedPort = convertToLPWSTR(portName);
	//open port
	com = CreateFile(selectedPort,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (com == INVALID_HANDLE_VALUE)
		throw OpeningPortError("Can't open port: " + portName);

	//configuration of port
	DCB dcbSerialParams = { 0 };
	GetCommState(com, &dcbSerialParams);
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	SetCommState(com, &dcbSerialParams);

	//configuration of timeouts
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 5000;		//in ms
	timeouts.ReadTotalTimeoutConstant = 5000;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts(com, &timeouts);
}

ReaderWriter::~ReaderWriter()
{
	CloseHandle(com);
}

void ReaderWriter::write(const char data)
{
	WriteFile(com, &data, characterCount, &characterSize, NULL);
	/*if (characterSize != characterCount) {
		throw ConnectionBrokenError("Character not send !");
	}*/
}

char ReaderWriter::read()
{
	char receiveChar;
	ReadFile(com, &receiveChar, characterCount, &characterSize, NULL);
	/*if (characterSize == 0) {
		throw ConnectionBrokenError("Character not received !");
	}*/
	return receiveChar;
}

