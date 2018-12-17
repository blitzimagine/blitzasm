#include "ModCompiler.h"

#include "ex.h"

#include <iostream>
#include <string>

using namespace std;

ModCompiler::ModCompiler() : mModule(new BBModule)
{

}

ModCompiler::~ModCompiler()
{
	if (mModule != nullptr)
		delete mModule;
}

void ModCompiler::compileLine(std::istream* stream, std::string line, Assem_x86 assembler)
{
	int i = 0;
	string name;
	std::vector<string> args;

	while (isspace(line[i])) i++;
	if (line[i] != '#')
	{
		try {
			assembler.assemLine(line);//.substr(1, line.length() - 1));
		}
		catch (Ex ex)
		{
			throw Ex(ex.ex);
		}
		return;
	}
	else {
		i++;
	}

	/*if (line.length() > 1 && line[line.length() - 2] == ':')
	{
		while (isspace(line[i])) i++;
		int from = i;
		for (++i; !isspace(line[i]) && isalnum(line[i]); ++i) {}
		string lbl = line.substr(from, i - from);
		if (i < line.length() - 2)
			throw Ex("Invalid Label");
		assembler.assemLine(lbl + ":\n");
		return;
	}*/

	//bool var = false;

	while (isspace(line[i]) && line[i] != '\n') i++;
	if (line[i] == '\n' || (line[i] == '/' && i + 1 < line.length() && line[i+1] == '/'))
		return;

	int from = i;
	for (++i; !isspace(line[i]) && isalnum(line[i]); ++i) {}
	name = line.substr(from, i - from);

	while (isspace(line[i]) && line[i] != '\n') i++;
	if (line[i] == '\n')
		throw Ex("Expected '(' or value");

	if (line[i] == '(')
	{
		i++;
		while (true)
		{
			while (isspace(line[i])) i++;
			from = i;
			if (line[i] == ')')
				break;
			if (line[i] == '"')
			{
				string s;
				while (line[++i] != '"' && line[i] != '\n')
				{
					s += line[i];
				}
				if (line[i] == '\n')
					throw Ex("Expected '\"'");
				args.push_back(s);
				while (isspace(line[i])) i++;
				i++;
				if (line[i] == ',')
					i++;
				else if (line[i] == ')')
					break;
				else
					throw Ex("Expected ')'");
				continue;
			}

			for (; !isspace(line[i]) && line[i] != ',' && line[i] != ')'; ++i) {}
			//if (from - i <= 0)
			//	throw Ex("Internal Error");
			string a = line.substr(from, i - from);
			args.push_back(a);
			while (isspace(line[i])) i++;
			if (line[i] == ',')
				i++;
			else if (line[i] == ')')
				break;
			else
				throw Ex("Expected ')'");
		}
	}
	/*else if (isspace(line[i]))
	{
		var = true;
		from = ++i;
		for (++i; !isspace(line[i]) && isalnum(line[i]); ++i) {}
		args.push_back(line.substr(from, from - 1));
	}*/

	/*if (var)
	{
		//assembler.assemLine("");
	}
	else {*/
		//try {
			
		if (name == "call")
		{
			if (args.size() < 1)
				throw Ex("Not enough arguments for " + name);
			int argSize = (args.size() - 1) * 4;
			assembler.assemLine("sub esp, " + to_string(argSize) + "\n");
			for (int i = 1; i < args.size(); i++)
			{
				assembler.assemLine("mov [esp-" + to_string(i * 4) + "], " + args[i] + "\n");
			}
			assembler.assemLine("call " + args[0] + "\n");
		}
		/*else if (name == "jump")
		{
			if (args.size() < 1)
				throw Ex("Not enough arguments for " + name);
			assembler.assemLine("jmp " + args[0] + "\n");
		}
		else if (name == "setvar")
		{
			if (args.size() < 3)
				throw Ex("Not enough arguments for " + name);
			if (args[1] == "1")
				assembler.assemLine(args[0] + ": .db " + args[2] + "\n");
			else if (args[1] == "2")
				assembler.assemLine(args[0] + ": .dw " + args[2] + "\n");
			else if (args[1] == "4")
				assembler.assemLine(args[0] + ": .dd " + args[2] + "\n");
			else
				throw Ex("Invalid variable size");
		}
		else if (name == "setstring")
		{
			if (args.size() < 2)
				throw Ex("Not enough arguments for " + name);
			assembler.assemLine(args[0] + ": .db \"" + args[1] + "\", 0\n");
		}*/
		else {
			throw Ex("Unknown macro: " + name);
		}
		/*}
		catch (Ex ex)
		{
			throw Ex("ASM: " + ex.ex);
		}*/
	//}
}

void ModCompiler::compile(std::istream* in, std::string filename)
{
	Assem_x86 assem(mModule);
	string line;

	int lIndex = 0;
	while (!(*in).eof()) {
		lIndex++;
		try {
			getline(*in, line);
			//if (line == "end_asm")
			//	break;
			line += '\n';
#ifdef LOG
			clog << line;
#endif
			compileLine(in, line, assem);
#ifdef LOG
			clog << endl;
#endif
		}
		catch (Ex &x) {
			throw Ex(x.ex/* + ": " +line.substr(0, line.length() - 1)*/, lIndex, filename);
		}
	}
}