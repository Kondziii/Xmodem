#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Checksum.h"
#include "ReaderWriter.h"
#include "signals.h"

class Receiver
{
public:
	void receive(std::string namePort);
};

