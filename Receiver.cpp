#include "Receiver.h"

using namespace std;

void Receiver::receive(std::string namePort)
{
	char character;

	ReaderWriter rw(namePort);

	cout << "Name of the file to save: ";
	string fileName;
	cin >> fileName;

	cout << "Choose type of checksum: \n1.Algebric checksum\n2.CRC\n";
	int typeChecksum;
	cin >> typeChecksum;
	char beginChar;

	switch (typeChecksum)
	{
		case 1:
		{
			beginChar = NAK;
			break;
		}
		case 2:
		{
			beginChar = C;
			break;
		}
		default:
		{
			cout << "Bad choice !\n";
			break;
		}
	}

	bool startedTransmission = false;
	for (int i = 0; i < 20; i++)
	{
		rw.write(beginChar);
		cout << "Waiting for SOH\n";
		character = rw.read();
		if (character == SOH)
		{
			startedTransmission = true;
			break;
		}
	}

	if (!startedTransmission)
	{
		cout << "Transmition error\n";
		getchar();
		exit(2);
	}

	ofstream file;
	file.open(fileName, ios::binary);

	cout << "Receiving the file\n";

	while (true)
	{

		character = rw.read();
		int packetNumber = (int)character;

		character = rw.read();
		char complementTo255 = character;

		char dataBlock[128];
		for (int i = 0; i < 128; i++)
		{
			character = rw.read();
			dataBlock[i] = character;
		}
		bool isPacketCorrect = true;

		if ((char)(255 - packetNumber) != complementTo255) // check checksum
		{
			cout << "Bad packet number\n";
			rw.write(NAK);
			isPacketCorrect = false;
		}
		else
		{
			Checksum chSum;
			int calculatedCheckstyle;
			bool correctChecksum = true;

			if (typeChecksum == 1) // algebric checksum
			{
				int check;
				character = rw.read();

				check = character;
				char checksum = chSum.simpleChecksum(dataBlock);
				if (checksum != check) correctChecksum = false;
			}

			else if (typeChecksum == 2) // CRC chcecksum 
			{
				char readChecksum[2];
				character = rw.read();

				readChecksum[0] = character;
				character = rw.read();

				readChecksum[1] = character;

				calculatedCheckstyle = chSum.calculateCRC(dataBlock, 128);

				if (chSum.calculateCharacterCRC(calculatedCheckstyle, 1) != readChecksum[0]
					|| chSum.calculateCharacterCRC(calculatedCheckstyle, 2) != readChecksum[1]) // check CRC
				{
					correctChecksum = false;
				}
			}

			if (!correctChecksum) // check correct of checksum
			{
				cout << "Bad checksum\n";
				rw.write(NAK);
				isPacketCorrect = false;
			}

		}

		if (isPacketCorrect)
		{
			for (int i = 0; i < 128; i++)
			{
				//if (dataBlock[i] != 26) // sometimes causes problems
				file << dataBlock[i];
			}
			cout << "Packet received successfully!\n";
			rw.write(ACK);
		}

		character = rw.read();
		if (character == EOT || character == CAN) break;
	}

	rw.write(ACK);

	file.close();
	if (character == CAN)
	{
		cout << "Connection was interrupted\n";
	}
	else
	{
		cout << "Transmission completed\n";
	}
	system("Pause");
}
