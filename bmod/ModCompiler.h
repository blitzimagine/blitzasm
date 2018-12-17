#ifndef __MODCOMPILER_H_
#define __MODCOMPILER_H_

#include "linker.h"
#include "assem_x86.h"

#include <istream>

class ModCompiler
{
public:
	ModCompiler();
	~ModCompiler();

	void compile(std::istream* s, std::string filename);

	BBModule* getModule() const { return mModule; }

private:
	void compileLine(std::istream* stream, std::string line, Assem_x86 assembler);

	BBModule* mModule;
};

#endif // __MODCOMPILER_H_