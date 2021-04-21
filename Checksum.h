#pragma once
class Checksum
{
public:
	char simpleChecksum(char* packet);
	int calculateCRC(char* wsk, int count);
	int checkIfEven(int x, int y);
	char calculateCharacterCRC(int n, int characterNumber);
};

