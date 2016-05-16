// ConverterMain.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "Converter.h"

int main(int argc, char* argv[])
{
    Converter *converter = new Converter;
    int retValue = converter->run(argc, argv);

    converter->release();
    return retValue;
}

