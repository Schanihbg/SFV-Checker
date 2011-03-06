#ifndef _TESTAPP_CPP
#define _TESTAPP_CPP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS     //Disables strcpy() security warning on Microsoft compilers.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <cStdIO>
#include <cStdLib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CCRC32.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;

void splitstring(string str, string separator, string &first, string &second);

int main(int iArg, char *sArg[])
{
	char sSourceFile[MAX_PATH];

	memset(sSourceFile, 0, sizeof(sSourceFile));

	if(iArg > 1)
	{
		//Get the passed in file name.
		strcpy(sSourceFile, sArg[1]);
	}
	else{
		//No file name was passed in, ask the user for the file name.
		cout << "Enter a file name: ";
		cin >> sSourceFile;
	}

	if(strlen(sSourceFile) > 0) //Do we have a file name?
	{
            string line, file, crc;

            int good = 0, bad = 0;

            ifstream sfv (sSourceFile);
            if (sfv.is_open())
            {
                while ( sfv.good() )
                {
                    getline (sfv,line);

                    if (line == "") continue;
                    if (";"  == line.substr(0, 1)) continue;

                    splitstring(line, " ", file, crc);

                    CCRC32 MyCRC32;
                    MyCRC32.Initialize(); //Only have to do this once.

                    unsigned long ulCRC = 0, check = 0;

                    if(MyCRC32.FileCRC(file.c_str(), &ulCRC))
                    {
                        stringstream ss;
                        stringstream sf(crc);

                        ss << hex << ulCRC;

                        sf.clear();
                        sf.str(crc);

                        if ( ss.str().length() == 7 )
                        {
                            string temp;

                            ss >> temp;
                            ss.clear();
                            ss.str("");
                            ss << 0 << temp;
                        }


                        cout << "Source File: " << ss.str() << endl;
                        cout << "SFV file: " << sf.str() << endl;

                        if ( ss.str() == sf.str() )
                        {
                            good++;
                            cout << "GOOD!" << endl;
                        }
                        else
                        {
                            bad++;
                            cout << "BAD!" << endl;

                            int result;
                            char* oldname;
                            ss >> oldname;
                            char newname[] = "TEMP";
                            result = rename( oldname , newname );
                        }

                        ss.clear();
                        sf.clear();

                    }
                    else
                    {
                        cout << "File not found or access denied.\n";
                    }

                    ulCRC = 0;

                }
                sfv.close();
            }
            cout << "Good files: "<< good << endl << "Bad files: " << bad << endl;
	}
	else{
		cout << "No input file was specified.\n";
	}

	system("pause");

	return 0;
}

void splitstring(string str, string separator, string &first, string &second)
{
    size_t i = str.find(separator);
    if(i != string::npos)
    {
        size_t y = 0;
        if(!str.empty())
        {
            first=""; second="";
            while(y != i)
            {
                first += str[y++];
            }
            y += separator.length();
            while(y != str.length())
            {
                second += str[y++];
            }
        }
    }
    else
    {
        first = str;
        second = "";
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
