#if !defined(UTIL_ARGUMENTS_HPP)
#define UTIL_ARGUMENTS_HPP

/*
 * This class reads command line arguments and parses them into
 * settings strings (see settings.hpp).
 *
 * @author Tuukka Haapasalo
 * @created 2004-07-25
 */

#include "util/Settings.hpp"

namespace util {

class ArgumentReader: public Settings {
public:
    /* Reads the parameters from a char * table */
    ArgumentReader(bool caseSensitive = false, Settings const * defaults = 0);

    /* Adds a valid program argument */
    void addArgument(String const & key, String const & shortKey = "");

    /* Checks whether an argument is a valid program argument */
    bool isKey(String const & key) const;

    /* Checks whether an argument is a valid program argument */
    bool isShortKey(String const & key) const;

    /* Reads the command line arguments */
    void read(int argc, char const * const * argv);

    /* Checks whether the default parameter was given */
    bool hasDefault() const {
        return has("__default");
    }
    /* Return the default parameter */
    String getDefault() const {
        return get("__default");
    }

private:
    void parseArgument(String const & line);

    void storeCurrent(String const & arg);

private:
    String currentKey;
    StringHashMap<String> keys;
    StringHashMap<String> shortKeys;
    bool currentStored;

};

}

#endif // !defined(UTIL_ARGUMENTS_HPP)
