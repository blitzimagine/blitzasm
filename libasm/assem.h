
#ifndef ASSEM_H
#define ASSEM_H

#include "linker.h"

class Assem{
public:
	BBModule *mod;
	Assem(BBModule *mod ):mod( mod ){}
	//virtual void assemble()=0;
};

#endif