
#include "linker.h"



BBModule::BBModule():data(0),data_sz(0),pc(0),linked(false){
}

BBModule::~BBModule(){
	if( linked ) VirtualFree( data,0,MEM_RELEASE );
	else delete[] data;
}

int BBModule::getPC(){
	return pc;
}

void BBModule::emit( int byte ){
	ensure(1);data[pc++]=byte;
}

bool BBModule::addSymbol( const char *sym,int pc ){
	string t(sym);
	if( symbols.find( t )!=symbols.end() ) return false;
	symbols[t]=pc;return true;
}

bool BBModule::addReloc( const char *dest_sym,int pc,bool pcrel ){
	map<int,string> &rel=pcrel ? rel_relocs : abs_relocs;
	if( rel.find( pc )!=rel.end() ) return false;
	rel[pc]=string(dest_sym);return true;
}

bool BBModule::findSymbol( const char *sym,int *pc ){
	string t=string(sym);
	map<string,int>::iterator it=symbols.find( t );
	if( it==symbols.end() ) return false;
	*pc=it->second + (int)data;
	return true;
}
