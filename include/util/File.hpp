#if !defined(UTIL_FILE_HPP)
#define UTIL_FILE_HPP

/*
 * Class for handling file descriptions.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-08-02
 */

#include "util/String.hpp"

namespace util {

class File {
public:
    File(char const * path);
    File(String const & path);
    File(File const & other);

    bool exists() const;

    bool deleteFile();

    long getSize() const;

    String const & getPath() const;

    String getName() const;

    bool writeString(std::string const & contents);
    String readAsString();

private:
    void readInfo() const;

    /* 
     * Assignment operator not available (private & not defined)
     */
    void operator=(File const & other);

private:
    String const path;
    mutable bool initialized;
    mutable long size;
    mutable bool valid;

};

}

#endif // !defined(UTIL_FILE_HPP)
