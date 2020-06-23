#include "util/Settings.hpp"

/*
 * General settings reading class. Reads settings from format
 * "Key = Value" into a hashmap. Values can be fetched with get(Key). 
 *
 * @author Tuukka Haapasalo
 * @created 2004-07-28
 * 
 * $Id: Settings.cpp,v 1.7 2005-03-03 16:33:57 tuska Exp $
 */

#include "util/File.hpp"
#include "util/FileReader.hpp"
#include "util/Producer.hpp"
#include "util/Iterator.hpp"
#include <iostream>

using namespace util;

/* 
 * Constructs empty settings 
 */
Settings::Settings(bool const caseSensitive_) :
    caseSensitive(caseSensitive_), valueIterator(0), keyIterator(0),
            keyValueIterator(0) {
}

/* 
 * Reads settings from a file 
 */
Settings::Settings(File const & file, bool const caseSensitive_) :
    caseSensitive(caseSensitive_), valueIterator(0), keyIterator(0),
            keyValueIterator(0) {
    FileReader reader(file);
    readFromProducer(reader);
    reader.close();
}

/* 
 * Reads settings from a string producer 
 */
Settings::Settings(Producer<String> & producer, bool const caseSensitive_) :
    caseSensitive(caseSensitive_), valueIterator(0), keyIterator(0),
            keyValueIterator(0) {
    readFromProducer(producer);
}

/* 
 * Parses settings from a list of strings 
 */
Settings::Settings(Iterator<String> & lines, bool const caseSensitive_) :
    caseSensitive(caseSensitive_), valueIterator(0), keyIterator(0),
            keyValueIterator(0) {
    while (lines.hasNext()) {
        parse(lines.next());
    }
}

/* 
 * Copies settings 
 */
Settings::Settings(Settings const & other) :
    caseSensitive(other.caseSensitive), values(other.values), valueIterator(0),
            keyIterator(0), keyValueIterator(0) {
}

/* 
 * Assignment 
 */
Settings const & Settings::operator=(Settings const & other) {
    caseSensitive = other.caseSensitive;
    clear();
    values = other.values;
    return *this;
}

/* 
 * Deletes the settings 
 */
Settings::~Settings() {
    clear();
}

void Settings::clear() {
    delete valueIterator;
    valueIterator = 0;
    delete keyIterator;
    keyIterator = 0;
    delete keyValueIterator;
    keyValueIterator = 0;
}

void Settings::readFromProducer(Producer<String> & producer) {
    while (producer.available()) {
        parse(producer.produce().trim());
    }
}

/* 
 * Adds the values from another Settings class to this class. 
 * Duplicates are added to the end of the corresponding lists.
 */
void Settings::addValues(Settings const & other) {
    for (Iterator<String const> * keys = other.getKeys(); keys->hasNext();) {
        String const & key = keys->next();
        for (Iterator < String > *values = other.getValues(key); values->hasNext();) {
            set(key, values->next());
        }
    }
}

/* 
 * Adds the values from another Settings class to this class. 
 * Duplicates are added to the beginning of the corresponding lists.
 */
void Settings::addValuesFirst(Settings const & other) {
    for (Iterator<String const> * keys = other.getKeys(); keys->hasNext();) {
        String const & key = keys->next();
        for (Iterator < String > *values = other.getValues(key); values->hasNext();) {
            setFirst(key, values->next());
        }
    }
}

void Settings::setValues(Settings const & other) {
    clear();
    values = other.values;
}

void Settings::parse(String const & line) {
    if (line.length() < 1)
        return;
    if (line[0] == '#')
        return;

    String::size_type eq = line.find("=");
    if (eq == line.npos)
        return;

    String name = String(line.substr(0, eq)).trim();
    String value = String(line.substr(eq + 1)).trim();

    if (name == "")
        return;
    set(name, value);
}

void Settings::set(String const & key, int const value) {
    if (key == "")
        return;
    set(key, String::toString(value));
}

void Settings::set(String const & key, double const value) {
    if (key == "")
        return;
    set(key, String::toString(value));
}

void Settings::set(String const & key, char const value) {
    if (key == "")
        return;
    set(key, String::toString(value));
}

void Settings::set(String const & key, bool const value) {
    if (key == "")
        return;
    set(key, value ? "true" : "false");
}

void Settings::set(String const & key, String const & value) {
    if (key == "")
        return;
    String const & useKey = getKey(key);
    if (values.find(useKey) == values.end()) {
        values[useKey] = StringList();
    }
    values[useKey].push_back(value);
}

/* 
 * Sets the string value to be the first value in the list 
 * (so that get() will return it, instead of a previous value)
 */
void Settings::setFirst(String const & key, String const & value) {
    if (key == "")
        return;
    String const & useKey = getKey(key);
    if (values.find(useKey) == values.end()) {
        values[useKey] = StringList();
    }
    values[useKey].push_front(value);
}

Iterator<Settings::StringList> * Settings::getValues() const {
    if (valueIterator == 0)
        valueIterator = new StringHashMapValueIterator<StringList> (values);
    valueIterator->reset();
    return valueIterator;
}

Iterator<String const> * Settings::getKeys() const {
    if (keyIterator == 0)
        keyIterator = new StringHashMapKeyIterator<StringList> (values);
    keyIterator->reset();
    return keyIterator;
}

Iterator<String> * Settings::getValues(String const & key) const {
    if (keyValueIterator != 0) {
        delete keyValueIterator;
        keyValueIterator = 0;
    }
    StringHashMap<StringList>::const_iterator i = values.find(getKey(key));
    if (i == values.end()) {
        WARN("String value not found: " << key);
        return 0;
    }

    keyValueIterator = new DequeIterator<String> (i->second);
    return keyValueIterator;
}
