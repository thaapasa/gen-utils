#if !defined(UTIL_FILEREADER_HPP)
#define UTIL_FILEREADER_HPP

/*
 * A wrapper for reading files a line at a time.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-07-28
 */

#include "util/String.hpp"
#include "util/Producer.hpp"
#include "util/File.hpp"
#include <cstdio>
#include <fstream>

namespace util {

class FileReader: public Producer<String> {
public:
    /* Opens a file for reading */
    FileReader(String const & filename);
    FileReader(File const & file);
    virtual ~FileReader();

    /* Checks that file is open */
    bool isOpen() const;

    /* Checks for end of file */
    bool eof() const;
    /* Reads in a line from the file */
    String readLine();

    /* Closes the file */
    void close();

    /*
     * Support for Producer<String> interface
     */

    /* Tests whether more strings are available */
    virtual bool available() const;

    /* Returns the next string from the provider */
    virtual String produce();

    /*
     * Provides a textual description of the producer
     */
    virtual String toString() const;

private:
    mutable std::ifstream input;
    File const file;

};

}

#endif // !defined(UTIL_FILEREADER_HPP)
