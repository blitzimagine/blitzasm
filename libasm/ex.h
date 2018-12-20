#ifndef EX_H
#define EX_H

struct Ex : std::exception
{
    string ex; //what happened
    int line; //source offset
    string file;
    Ex(const string& ex): ex(ex), line(-1) {}
    Ex(const string& ex, const string& name) : ex(ex + ": " + name), line(-1) {}
    Ex(const string& ex, int line, const string& t): ex(ex), line(line), file(t) {}
};

#endif
