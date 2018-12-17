
#ifndef ASSEM_X86_H
#define ASSEM_X86_H

#include "assem.h"

#include <string>
#include <vector>
#include <iostream>

#include "insts.h"
#include "operand.h"

using namespace std;

class Assem_x86 : public Assem{
public:
	Assem_x86(BBModule *mod );//istream &in,

	virtual void assemble(std::istream* in, std::string filename);
	//void assem(std::istream* in);
	void assemLine(const string &line);

private:

	void align( int n );
	void emit( int n );
	void emitw( int n );
	void emitd( int n );
	void emitImm( const string &s,int size );
	void emitImm( const Operand &o,int size );
	void r_reloc( const string &dest );
	void a_reloc( const string &dest );
	void assemDir( const string &name,const string &op );
	void assemInst( const string &name,const string &lhs,const string &rhs );
};

#endif
