#include "Checksum.h"

char Checksum::simpleChecksum(char* packet)
{
	char checksum = 0;
	for (int i = 0; i < 128; i++)
	{
		checksum += packet[i] % 256;
	}
	return checksum;
}

int Checksum::calculateCRC(char* wsk, int count)
{
	int ChecksumCRC = 0;
	while (--count >= 0)
	{
		ChecksumCRC = ChecksumCRC ^ (int)*wsk++ << 8;
		for (int i = 0; i < 8; ++i)
		{
			if (ChecksumCRC & 0x8000)
			{
				ChecksumCRC = ChecksumCRC << 1 ^ 0x1021;
			}
			else
			{
				ChecksumCRC = ChecksumCRC << 1;
			}
		}
	}
	return (ChecksumCRC & 0xFFFF);
}

int Checksum::checkIfEven(int x, int y)
{
	if (y == 0) { return 1; }
	if (y == 1) { return x; }

	int result = x;

	for (int i = 2; i <= y; i++)
	{
		result = result * x;
	}

	return result;
}

char Checksum::calculateCharacterCRC(int n, int characterNumber)
{
	int x, binary[16];

	for (int i = 0; i < 16; i++)
	{
		binary[i] = 0;
	}

	for (int i = 0; i < 16; i++)
	{
		x = n % 2;
		if (x == 1) { n = (n - 1) / 2; }
		if (x == 0) { n = n / 2; }

		binary[15 - i] = x;
	}

	int result = 0;
	int k{};

	if (characterNumber == 1) { k = 7; }
	if (characterNumber == 2) { k = 15; }

	for (int i = 0; i < 8; i++)
	{
		result += checkIfEven(2, i) * binary[k - i];
	}
	return (char)result;
}
