// XModem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "ReaderWriter.h"
#include "Transmiter.h"
#include "Receiver.h"

using namespace std;

int main()
{
    string portName;
    cout << "Choose Port: ";
    getline(cin, portName);

    int choice;
    cout << "Choose the function: \n1.Send\n2.Receive\n";
    cin >> choice;

    switch(choice)
    {
        case 1: 
        {
            Transmiter transmiter;
            transmiter.transmit(portName);
            break;
            //sending
        }
        case 2:
        {
            Receiver receiver;
            receiver.receive(portName);
            break;
            //receiving
        }
        default: 
        {
            cout << "Bad choice !\n";
            break;
        }
    }

    return 0;
}

