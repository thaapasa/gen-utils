#if !defined(UTIL_SETTINGS_HPP)
#define UTIL_SETTINGS_HPP

/*
 * General settings reading class. Reads settings from format
 * "Key = Value" into a hashmap. Values can be fetched with get(Key). 
 * Multiple values per key are supported, and can be fetched via
 * getValues(key). Note that the value iterator that is returned is only
 * valid until the next call to getValues(key) (or until the class is 
 * destroyed). Caller must never delete the returned iterator.
 *
 * @author Tuukka Haapasalo
 * @created 2004-07-28
 */

#include "util/String.hpp"
#include "util/Log.hpp"
#include "util/StringHashMap.hpp"

#include <iostream>
#include <deque>

namespace util {

using std::deque;

class File;
template<typename T> class Producer;
template<typename T> class Iterator;

class Settings {

public:
    /* Constructs empty settings */
    Settings(bool const caseSensitive = false);
    /* Reads settings from a file */
    Settings(File const & file, bool const caseSensitive = false);
    /* Reads settings from a string producer */
    Settings(Producer<String> & file, bool const caseSensitive = false);
    /* Parses settings from a list of strings */
    Settings(Iterator<String> &, bool const caseSensitive = false);
    /* Copies settings */
    Settings(Settings const &);

    /* Deletes the settings */
    virtual ~Settings();
    /* Assignment */
    virtual Settings const & operator=(Settings const &);

    /* Value fetching functions */
    inline int getInt(String const & key) const;
    inline double getDouble(String const & key) const;
    inline char getChar(String const & key) const;
    inline bool getBoolean(String const & key) const;
    inline String getString(String const & key) const;
    inline String get(String const & key) const {
        return getString(key);
    }

    Iterator<String> * getValues(String const & key) const;

    /* Checks for testing whether a value with given key exists */
    inline bool has(String const & key) const;
    inline size_t countValues(String const & key) const;

    /* Value setters */
    void set(String const & key, int const value);
    void set(String const & key, double const value);
    void set(String const & key, char const value);
    void set(String const & key, bool const value);
    void set(String const & key, String const & value);
    void set(String const & key, std::string const & value) {
        set(key, String(value));
    }
    void set(String const & key, char const * const value) {
        set(key, String(value));
    }

    /* 
     * Sets the string value to be the first value in the list 
     * (so that get() will return it, instead of a previous value)
     */
    void setFirst(String const & key, String const & value);

    typedef deque<String> StringList;

    /* Returns iterators for values */
    Iterator<StringList> * getValues() const;

    /* Returns iterators for keys */
    Iterator<String const> * getKeys() const;

    /* Parses a line into the settings */
    void parse(String const & str);

    bool isCaseSensitive() const {
        return caseSensitive;
    }
    void setCaseSensitive(bool const state) {
        caseSensitive = state;
    }

    /* 
     * Adds the values from another Settings class to this class. 
     * Duplicates are added to the end of the corresponding lists.
     */
    void addValues(Settings const & other);
    /* 
     * Adds the values from another Settings class to this class. 
     * Duplicates are added to the beginning of the corresponding lists.
     */
    void addValuesFirst(Settings const & other);

protected:
    void setValues(Settings const & other);

    String getKey(String const & key) const {
        return caseSensitive ? key : key.toLowerCase();
    }

private:
    void clear();

    void readFromProducer(Producer<String> & producer);

private:
    bool caseSensitive;

    StringHashMap<StringList> values;

    mutable Iterator<StringList> * valueIterator;

    mutable Iterator<String const> * keyIterator;

    mutable Iterator<String> * keyValueIterator;

};

/*
 * Definitions for value getting and testing for value existence
 */

int Settings::getInt(String const & key) const {
    return getString(key).toInt();
}

double Settings::getDouble(String const & key) const {
    return getString(key).toDouble();
}

char Settings::getChar(String const & key) const {
    String const & val = getString(key);
    if (val.size() < 1)
        return 0;
    return val[0];
}

bool Settings::getBoolean(String const & key) const {
    return getString(key).toLowerCase() == "true";
}

String Settings::getString(String const & key) const {
    StringHashMap<StringList>::const_iterator i = values.find(getKey(key));
    if (i == values.end()) {
        WARN("String value not found: " << key);
        return "";
    }
    ASSERT(i->second.size() > 0);
    return i->second[0];
}

size_t Settings::countValues(String const & key) const {
    StringHashMap<StringList>::const_iterator i = values.find(getKey(key));
    if (i == values.end()) {
        return 0;
    }
    return i->second.size();
}

bool Settings::has(String const & key) const {
    return values.find(getKey(key)) != values.end();
}

}

#endif // !defined(UTIL_SETTINGS_HPP)
