#include "assem_x86.h"
#include "ex.h"
#include "linker.h"

#include "ModCompiler.h"

#define RETURN() if (waitKey) { cout << "Press any key to continue..."; getchar(); } return 0

#define MAJOR_VERSION	"1"
#define MINOR_VERSION	"0"
#define REVISION		"00"

void printAbout()
{
	cout << "BlitzMod created by Matthieu Parizeau" << endl << "Based on Blitz3D Assembler" << endl;
	cout << endl;
}

void printVersion()
{
	printAbout();
	cout << "BlitzMod v" << MAJOR_VERSION << "." << MINOR_VERSION << "_" << REVISION << endl;
}

void printHelp()
{
	printAbout();
	cout << "Usage: bmod [options] <inputfile.asm> [outputfile.bbc]" << endl;
	cout << endl;
	cout << "Options:" << endl;
	cout << "    Option    Name        Function" << endl;
	cout << "    ------------------------------" << endl;
	cout << "    -h        Help        Displays this help" << endl;
	cout << "    -a        About       Displays information about basm" << endl;
	cout << "    -v        Version     Displays the version number" << endl;
	cout << "    -q        Quiet       Disables console output" << endl;
	cout << "    -n        NoAbout     Disables displaying about" << endl;
	cout << "    -w        WaitKey     Waits for input before closing" << endl;
}

int main(int argc, char** argv)
{
	bool waitKey = false;
	bool quiet = false;
	bool noabout = false;

	int i;
	for (i = 1; i < argc; i++)
	{
		const char* arg = argv[i];
		if (arg[0] != '-')
			break;
		switch (arg[1])
		{
		case 'h':
			printHelp();
			RETURN();
			break;
		case 'a':
			printAbout();
			RETURN();
			break;
		case 'v':
			printVersion();
			RETURN();
			break;
		case 'w':
			waitKey = true;
			break;
		case 'q':
			quiet = true;
			break;
		case 'n':
			noabout = true;
			break;
		default:
			cout << "Unknown Option: " << arg << endl;
			RETURN();
		}
	}

	int startArg = i;
	int argCount = argc - startArg;

	if (argCount == 0)
	{
		printHelp();
		RETURN();
	}

	if (!quiet && !noabout)
	{
		printAbout();
	}

	string inputfile = argv[startArg];

	string outputfile = inputfile;
	int index = outputfile.find_last_of('.');
	outputfile = outputfile.substr(0, index);
	outputfile += ".bbc";

	if (argCount > 1)
	{
		outputfile = argv[startArg + 1];
	}

	ifstream file;
	file.open(inputfile);

	BBModule* bb;
	ModCompiler mod;

	try {
		mod.compile(&file, inputfile);
	}
	catch (Ex ex)
	{
		if (!quiet)
			cout << "[Line " << ex.line << "]: " << ex.ex << endl;
		file.close();
		RETURN();
	}

	bb = mod.getModule();

	file.close();

	ofstream out;
	out.open(outputfile, ios::out | ios::binary);

	int sz = bb->getDataSize();
	out.write((char*)&sz, 4);
	out.write(bb->getData(), sz);

	map<string, int>::iterator its;

	map<string, int> syms = bb->getSymbols();

	sz = syms.size();

	out.write((char*)&sz, 4);
	for (its = syms.begin(); its != syms.end(); ++its)
	{
		string name = its->first + '\0';
		out.write(name.data(), name.size());
		sz = its->second;out.write((char*)&sz, 4);
	}

	map<int, string>::iterator it;

	map<int, string> rels = bb->getRelRelocs();

	sz = rels.size();

	out.write((char*)&sz, 4);
	for (it = rels.begin(); it != rels.end(); ++it)
	{
		string name = it->second + '\0';
		out.write(name.data(), name.size());
		sz = it->first;out.write((char*)&sz, 4);
	}

	map<int, string> abs = bb->getAbsRelocs();

	sz = abs.size();

	out.write((char*)&sz, 4);
	for (it = abs.begin(); it != abs.end(); ++it)
	{
		string name = it->second + '\0';
		out.write(name.data(), name.size());
		sz = it->first;out.write((char*)&sz, 4);
	}
	out.flush();
	out.close();

	RETURN();
}