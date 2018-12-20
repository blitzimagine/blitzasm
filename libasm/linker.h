#ifndef LINKER_H
#define LINKER_H

#include "std.h"
#include <map>

class BBModule
{
public:
    BBModule();
    ~BBModule();

    int getPC();

    void emit(int byte);
    bool addSymbol(const char* sym, int pc);
    bool addReloc(const char* dest_sym, int pc, bool pcrel);

    bool findSymbol(const char* sym, int* pc);

    bool checkSymbols();

    std::map<std::string, int> getSymbols()
    {
        return symbols;
    };

    int getDataSize()
    {
        return pc;
    }

    char* getData()
    {
        return data;
    }

    map<int, string> getRelRelocs()
    {
        return rel_relocs;
    }

    map<int, string> getAbsRelocs()
    {
        return abs_relocs;
    }

private:
    char* data;
    int data_sz, pc;
    bool linked;

    map<string, int> symbols;
    map<int, string> rel_relocs, abs_relocs;

    bool findSym(const string& t, BBModule* libs, int* n)
    {
        if (findSymbol(t.c_str(), n)) return true;
        if (libs && libs->findSymbol(t.c_str(), n)) return true;

        if (t.length() > 0 && t[0] != '_')
        {
            string err = "Symbol '" + t + "' not found";
            cout << "Blitz Linker Warning: " << err << endl;
            //MessageBox(GetDesktopWindow(), err.c_str(), "Blitz Linker Error", MB_TOPMOST | MB_SETFOREGROUND);
        }
        return false;
    }

    void ensure(int n)
    {
        if (pc + n <= data_sz) return;
        data_sz = data_sz / 2 + data_sz;
        if (data_sz < pc + n) data_sz = pc + n;
        char* old_data = data;
        data = d_new char[data_sz];
        memcpy(data, old_data, pc);
        delete old_data;
    }
};

#endif
