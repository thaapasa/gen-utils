#include "util/File.hpp"

/*
 * Class for handling file descriptions.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-08-02
 * 
 * $Id: File.cpp,v 1.1 2004-08-02 20:09:13 tuska Exp $
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <cstdio>

using namespace util;
using namespace std;

File::File(char const * path_) :
    path(String(path_)), initialized(false) {
}

File::File(String const & path_) :
    path(path_), initialized(false) {
}

File::File(File const & other) :
    path(other.path), initialized(other.initialized), size(other.size), valid(
            other.valid) {
}

bool File::exists() const {
    if (initialized) {
        return valid;
    }
    readInfo();
    return valid;
}

long File::getSize() const {
    if (initialized) {
        return size;
    }
    readInfo();
    return size;
}

String const & File::getPath() const {
    return path;
}

bool File::deleteFile() {
    initialized = false;
    return remove(path.c_str()) == 0;
}

void File::readInfo() const {
    struct stat info;
    int result = stat(path.c_str(), &info);
    if (result == 0) {
        valid = true;
        size = info.st_size;
    } else {
        valid = false;
        size = -1;
    }
}

String File::getName() const {
#ifdef WIN32
    return path.afterLast("\\").afterLast("/");
#else
    return path.afterLast("/");
#endif
}

bool File::writeString(std::string const & contents) {
    ofstream ofile(path.c_str());
    if (!ofile)
        return false;
    ofile << contents;
    return true;
}

String File::readAsString() {
    ifstream ifile(path.c_str());
    return string((istreambuf_iterator<char> (ifile)),
            istreambuf_iterator<char> ());
}
