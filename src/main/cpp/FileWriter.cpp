#include "util/FileWriter.hpp"

/*
 * A wrapper for writing files a line at a time.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-08-02
 * 
 * $Id: FileWriter.cpp,v 1.2 2004-08-07 15:44:39 tuska Exp $
 */

using namespace util;

using std::flush;

FileWriter::FileWriter(String const & filename) :
    output(filename.c_str()), file(File(filename)) {
}

FileWriter::FileWriter(File const & file_) :
    output(file_.getPath().c_str()), file(file_) {
}

FileWriter::~FileWriter() {
    close();
}

/* 
 * Checks that file is open 
 */
bool FileWriter::isOpen() const {
    return output.is_open();
}

/* 
 * Writes a string to the file 
 */
void FileWriter::write(String const & str) {
    output << str << flush;
}

/* 
 * Closes the file 
 */
void FileWriter::close() {
    if (isOpen()) {
        output << flush;
        output.close();
    }
}

/*
 * Support for StringConsumer
 */

/*
 * Consumes the string, appending it to the file
 */
void FileWriter::consume(String const & src) {
    write(src);
}

/*
 * Provides a textual description of the consumer
 */
String FileWriter::toString() const {
    return String::format("FileWriter for \"%s\"", file.getPath().c_str());
}
