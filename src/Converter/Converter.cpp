#include "Converter.h"
#include <TCFoundation/TCVector.h>
#include <TCFoundation/mystring.h>
#include <LDLoader/LDLMainModel.h>
#include <LDLoader/LDLModelLine.h>
#include <TCFoundation/TCAlertManager.h>
#include <TCFoundation/TCStlIncludes.h>

#ifdef EXPORT_3DS
#include <LDExporter/LD3dsExporter.h>
#endif // EXPORT_3DS

#ifdef WIN32
#include <direct.h>
#else // WIN32
#include <unistd.h>
#endif // WIN32

Converter::Converter(void)
    :m_delete(false),
    m_overwrite(true),
    m_verbose(false),
    m_abort(false),
    m_optionsDone(false),
    m_fileProcessed(false),
    m_deleteCount(0),
    m_newFileCount(0)
{
}

Converter::~Converter(void)
{
}

void Converter::dealloc(void)
{
    TCObject::dealloc();
}

void Converter::ldlErrorCallback(LDLError *error)
{
    if (error && (m_verbose || error->getLevel() == LDLACriticalError))
    {
        char *filename = copyString(error->getFilename());

        fprintf(stderr, "%s", error->getTypeName());
        if (error->getLineNumber() >= 0)
        {
            fprintf(stderr, " on line %d", error->getLineNumber());
        }
        fprintf(stderr, " in: ");
        if (filename)
        {
            cleanFilename(filename);
            fprintf(stderr, "%s:\n", filename);
        }
        else
        {
            fprintf(stderr, "<Unknown file>:\n");
        }
        if (error->getMessage())
        {
            fprintf(stderr, "    %s\n", error->getMessage());
        }
        if (error->getFileLine())
        {
            fprintf(stderr, "    %s\n", error->getFileLine());
        }
        delete filename;
    }
}

static bool fileCaseCallback(char *filename)
{
    FILE *file;

    fprintf(stderr, "file case callback : %s\n", filename); // ???DEBUG

    convertStringToLower(filename);
    file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

#define ARG(str) (!strcmp(argv[i], str))
#define ARG_STARTS_WITH(str,len) (!strncmp(argv[i], str, len))
#define ARG_CONTAINS(str) strstr(argv[i], str)

int Converter::run(int argc, char *argv[])
{
    int i;
    int retValue = 0;
    char initialPath[1024];

    getcwd(initialPath, sizeof(initialPath));
    TCAlertManager::registerHandler(LDLError::alertClass(), this,
                                    (TCAlertCallback)&Converter::ldlErrorCallback);

    LDLModel::setFileCaseCallback(fileCaseCallback);

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (ARG("--help") || ARG("-h"))
            {
                printUsage();
                return 0;
            }
            else if (ARG("-v") || ARG("--verbose"))
            {
                m_verbose = true;
            }
            else if (ARG("--"))
            {
                ++i;
                break;
            }
            else
            {
                fprintf(stderr, "Invalid option %s", argv[i]);
                return 1;
            }
        }
        else
            break;
    }

    if (i+1 >= argc)
    {
        fprintf(stderr, "Two files as arguments are required\n\n");
        printUsage();
        return 1;
    }

    const char *srcFilename = argv[i];
    const char *dstFilename = argv[i+1];

    LDLMainModel *mainModel = new LDLMainModel;

    if (mainModel->load(srcFilename))
    {
        if (m_verbose)
            mainModel->print();

        float identMatrix[] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };

        LD3dsExporter *exporter = new LD3dsExporter;

    #if 0
        exporter->setBoundingBox(boundingMin, boundingMax);
        exporter->setCenter(center);
        exporter->setWidth((TCFloat)width);
        exporter->setHeight((TCFloat)height);
        exporter->setRadius(size / 2.0f);
        exporter->setBackgroundColor(backgroundR, backgroundG, backgroundB);
        exporter->setCamera(camera);
        exporter->setRotationMatrix(rotationMatrix);
        exporter->setFov(fov);
        exporter->setXPan(xPan);
        exporter->setYPan(yPan);
    #endif
        exporter->setAppUrl("http://ldview.sourceforge.net/");
        exporter->setAppName("LDConv");
        exporter->setFilename(dstFilename);

        if (exporter->doExport(mainModel))
        {
            retValue = 0;
            printf("Exported model to file %s\n", dstFilename);
        }
        else
        {
            retValue = 1;
            fprintf(stderr, "Could not export model to file %s\n", dstFilename);
        }
        exporter->release();
        exporter = NULL;
    }

    mainModel->release();

    return retValue;
}

void Converter::printUsage(void)
{
    printf("LDRConvert v0.1 Copyright (C) 2016 Dmitri Rubinstein, (C) 2003 Travis Cobbs.\n\n");
    printf("Usage:\n");
    printf("    ldrconvert [options] src_filename dest_filename\n\n");
    printf("Options:\n");
    printf("    -h | --help      Print this\n");
    printf("    -v | --verbose   Print all messages\n");
    printf("    --               No more options follow.  Use this as the final option to "
           "process a\n");
    printf("         file with a filename that begins with '-'.\n\n");
    printf("Multiple options must be separated by spaces.\n");
    printf("Filenames cannot include wildcard characters (such as '*' and '?')."
           "\n");
#ifdef WIN32
    printf("If you don't have an ldraw.ini file that specifies your LDraw "
           "directory, set\n");
    printf("the LDRAWDIR environment variable to contain the full path to your "
           "LDraw\n");
    printf("directory.\n");
#else // WIN32
    printf("Set the LDRAWDIR environment variable to contain the full path to "
           "your LDraw\n");
    printf("directory.\n");
#endif // WIN32
}

void Converter::cleanFilename(char *filename)
{
#ifdef WIN32
    replaceStringCharacter(filename, '/', '\\', 1);
#else
    replaceStringCharacter(filename, '\\', '/', 1);
#endif // WIN32
}

