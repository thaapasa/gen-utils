#include "util/FileReader.hpp"

/*
 * A wrapper for reading files a line at a time.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-07-28
 * 
 * $Id: FileReader.cpp,v 1.5 2004-08-22 12:22:57 tuska Exp $
 */

#include "util/Log.hpp"

using namespace util;

/*
 * Opens a file reader to a file
 */
FileReader::FileReader(String const & filename) :
    input(filename.c_str()), file(filename) {
    if (!input.is_open()) {
        WARN("Could not open file " << filename << " for reading");
    }
}

/*
 * Opens a file reader to a file
 */
FileReader::FileReader(File const & file_) :
    input(file_.getPath().c_str()), file(file_) {
    if (!input.is_open()) {
        WARN("Could not open file " << file.getPath() << " for reading");
    }
}

/*
 * Destroys the file reader, forces closing of the file
 */
FileReader::~FileReader() {
    close();
}

/* Checks that file is open */
bool FileReader::isOpen() const {
    return input.is_open();
}

/*
 * Check whether the file pointer is at the end of the file
 */
bool FileReader::eof() const {
    return input.eof();
}

/*
 * Read a line from the file
 */
String FileReader::readLine() {
    String result;
    getline(input, result);
    // At eof, return empty string
    return result;
}

/*
 * Closes the file
 */
void FileReader::close() {
    if (isOpen()) {
        input.close();
    }
}

/*
 * Support for Producer<String> interface
 */

/* 
 * Tests whether more strings are available 
 */
bool FileReader::available() const {
    return isOpen() && !eof();
}

/* 
 * Returns the next string from the producer 
 */
String FileReader::produce() {
    ASSERT(available());
    return readLine();
}

/*
 * Provides a textual description of the producer
 */
String FileReader::toString() const {
    return String::format("FileReader for \"%s\"", file.getPath().c_str());
}
