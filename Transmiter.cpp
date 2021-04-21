#include "Transmiter.h"

using namespace std;

void Transmiter::transmit(std::string namePort)
{
	char character;

	ReaderWriter rw(namePort);

	cout << "File name to send: ";
	string fileName;
	cin >> fileName;

	cout << "Waiting for transmittion." << endl;
	bool chosedCRC;
	bool startedTransmission = false;

	for (int i = 0; i < 20; i++)
	{
		character = rw.read();
		if (character == C)//C -> CRC
		{
			chosedCRC = true;
			startedTransmission = true;
			break;
		}
		else if (character == NAK)//NAK -> algebric checksum
		{
			chosedCRC = false;
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

	ifstream file;
	file.open(fileName, ios::binary);

	int packetNumber = 1;
	char packet[128];
	while (!file.eof())
	{
		for (int i = 0; i < 128; i++)
		{
			packet[i] = (char)26;
		}

		int i = 0;
		while (i < 128 && !file.eof())
		{
			packet[i] = file.get();
			if (file.eof())
			{
				packet[i] = (char)26;
			}
			i++;
		}
		bool isPacketCorrect = false;

		while (!isPacketCorrect)
		{
			cout << "Sending the packet" << endl;
			rw.write(SOH);
			character = (char)packetNumber;
			rw.write(character);
			character = (char)255 - packetNumber;
			rw.write(character);

			for (int i = 0; i < 128; i++)
			{
				rw.write(packet[i]);
			}

			Checksum chSum;

			if (!chosedCRC) //algebric checksum
			{
				char checksum = chSum.simpleChecksum(packet);
				rw.write(checksum);
			}
			else if (chosedCRC) //checksum CRC
			{
				int CRC = chSum.calculateCRC(packet, 128);
				character = chSum.calculateCharacterCRC(CRC, 1);
				rw.write(character);
				character = chSum.calculateCharacterCRC(CRC, 2);
				rw.write(character);
			}

			while (1)
			{
				character = ' ';
				character = rw.read();

				if (character == ACK) //Everything ok
				{
					isPacketCorrect = true;
					cout << "Packet transmitted successfully" << endl;
					break;
				}
				if (character == NAK) // Error with checksum
				{
					isPacketCorrect = true;
					cout << "Checksum error" << endl;
					break;
				}
				if (character == CAN) // Sth went wrong
				{
					cout << "Transmition failed" << endl;
					getchar();
					exit (0);
				}
			}
		}
		if (packetNumber < 255) { packetNumber++; }
		else { packetNumber = 1; } // next packet inc., increasing the number
	}
	file.close();


	character = ' ';
	while (character != ACK) //closing the transmittion, sending EOT and waiting for ACK
	{
		character = EOT;
		rw.write(character);
		character = rw.read();
	}


	cout << endl << "Transmission completed" << endl;
	system("Pause");

}
