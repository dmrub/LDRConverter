// ConverterMain.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "Converter.h"
#include <TCFoundation/TCLocalStrings.h>
#include "LDViewMessages.h"

int main(int argc, char* argv[])
{
    int stringTableSize = sizeof(LDViewMessages_bytes);
    char *stringTable = new char[sizeof(LDViewMessages_bytes) + 1];

    memcpy(stringTable, LDViewMessages_bytes, stringTableSize);
    stringTable[stringTableSize] = 0;
    TCLocalStrings::setStringTable(stringTable);

    Converter *converter = new Converter;
    int retValue = converter->run(argc, argv);

    converter->release();
    delete [] stringTable;
    return retValue;
}
