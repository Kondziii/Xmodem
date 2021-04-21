#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ReaderWriter.h"
#include "Checksum.h"
#include "signals.h"

class Transmiter
{
public:
	void transmit(std::string namePort);
};

