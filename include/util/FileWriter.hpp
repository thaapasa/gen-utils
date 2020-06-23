#if !defined(UTIL_FILEWRITER_HPP)
#define UTIL_FILEWRITER_HPP

/*
 * A wrapper for writing files a line at a time.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-08-02
 */

#include "util/String.hpp"
#include "util/File.hpp"
#include "util/Consumer.hpp"
#include <fstream>

namespace util {

class FileWriter: public Consumer<String> {
public:
    FileWriter(String const & filename);
    FileWriter(File const & file);
    virtual ~FileWriter();

    /* Checks that file is open */
    bool isOpen() const;

    /* Writes a string to the file */
    void write(String const & str);

    /* Closes the file */
    void close();

    /*
     * Support for StringConsumer
     */

    /*
     * Consumes the string, appending it to the file
     */
    virtual void consume(String const & src);

    /*
     * Provides a textual description of the consumer
     */
    virtual String toString() const;

private:

    mutable std::ofstream output;
    File const file;

};

}

#endif // !defined(UTIL_FILEWRITER_HPP)
