#include "util/ArgumentReader.hpp"

/*
 * This class reads command line arguments and parses them into
 * settings strings (see settings.hpp).
 *
 * @author Tuukka Haapasalo
 * @created 2004-07-25
 * 
 * $Id: ArgumentReader.cpp,v 1.6 2005-03-01 10:11:35 tuska Exp $
 */

#include "util/Log.hpp"

using namespace util;

/* 
 * Reads the parameters from a char* table 
 */
ArgumentReader::ArgumentReader(bool caseSensitive, Settings const * defaults) :
    Settings(), currentStored(false) {
    setCaseSensitive(caseSensitive);
    if (defaults != 0) {
        setValues(*defaults);
    }
}

/* 
 * Adds a program argument 
 */
void ArgumentReader::addArgument(String const & key, String const & shortKey) {
    if (key == "")
        return;
    ASSERT(!isKey(key));
    keys[getKey(key)] = key;
    if (shortKey != "") {
        ASSERT(!isShortKey(shortKey));
        shortKeys[getKey(shortKey)] = getKey(key);
    }
}

/* 
 * Checks whether an argument is a valid program argument 
 */
bool ArgumentReader::isKey(String const & key) const {
    return keys.find(getKey(key)) != keys.end();
}

/* 
 * Checks whether an argument is a valid short program argument 
 */
bool ArgumentReader::isShortKey(String const & key) const {
    return shortKeys.find(getKey(key)) != shortKeys.end();
}

/* 
 * Reads the command line arguments 
 */
void ArgumentReader::read(int argc, char const * const * argv) {
    currentStored = true;
    for (int i = 0; i < argc; i++) {
        parseArgument(argv[i]);
    }
    if (!currentStored) {
        storeCurrent("");
    }
}

void ArgumentReader::storeCurrent(String const & arg) {
    set(currentKey, arg);
    currentKey = "__default";
    currentStored = true;
}

/*
 * Parses a command line argument. Must be called in order of
 * parameter appearance on command line.
 */
void ArgumentReader::parseArgument(String const & arg) {
    if (arg.length() >= 2 && arg.substr(0, 2) == "--") {
        String const & key = arg.substr(2, arg.length());
        if (isKey(key)) {
            if (!currentStored) {
                storeCurrent("");
            }
            currentKey = getKey(key);
            currentStored = false;
        } else {
            WARN("Invalid program argument \"--" << key << "\"");
        }
    } else if (arg.length() >= 1 && arg[0] == '-') {
        String const & key = arg.substr(1, arg.length());
        if (isShortKey(key)) {
            if (!currentStored) {
                storeCurrent("");
            }
            currentKey = shortKeys[getKey(key)];
            currentStored = false;
        } else {
            WARN("Invalid program argument \"-" << key << "\"");
        }
    } else {
        // Store given argument
        storeCurrent(arg);
    }
}

