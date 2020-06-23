#include "util/SSLSocket.hpp"

/*
 * Wrapper for standard TCP and UDP sockets.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-10-12
 * 
 * $Id: SSLSocket.cpp,v 1.6 2005-02-23 21:32:24 tuska Exp $
 */

#include "util/Debug.hpp"

using namespace util;

extern "C" {
#include <openssl/ssl.h>
}
#include <sstream>

#define STRING_BUFFER_LENGTH 1024

// SSLSocket
// --------------------------------------------------------------------------

bool SSLSocket::initialized = false;
SSL_CTX * SSLSocket::context = 0;

SSLSocket::SSLSocket() {
    if (!initialized) {
        SSL_load_error_strings();
        SSL_library_init();

        // TODO: Check the method parameter
        context = SSL_CTX_new(SSLv3_method());

        initialized = true;
    }
}

SSLSocket::~SSLSocket() {
}

// SSLTCPSocket
// --------------------------------------------------------------------------

/* 
 * Creates a SSL TCP socket to given host & port 
 */
SSLTCPSocket::SSLTCPSocket(String const & host, unsigned int port) :
    Socket(host, port), TCPSocket(host, port) {
    ssl = SSL_new(context);
    SSL_set_fd(ssl, getDescriptor());
}

/* 
 * Creates a TCP socket to given ip & port 
 */
SSLTCPSocket::SSLTCPSocket(unsigned short ip1, unsigned short ip2,
        unsigned short ip3, unsigned short ip4, unsigned int port) :
    Socket(String::format("%d.%d.%d.%d", ip1, ip2, ip3, ip4), port), TCPSocket(
            ip1, ip2, ip3, ip4, port) {
    ssl = SSL_new(context);
    SSL_set_fd(ssl, getDescriptor());
}

/* 
 * Creates a TCP socket wrapper for an existing socket descriptor 
 */
SSLTCPSocket::SSLTCPSocket(String const & host, unsigned int port,
        int descriptor) :
    Socket(host, port), TCPSocket(host, port, descriptor) {
    ssl = SSL_new(context);
    SSL_set_fd(ssl, getDescriptor());
}

/*
 * Deletes the socket, closing it if it's still connected
 */
SSLTCPSocket::~SSLTCPSocket() {
    if (isConnected()) {
        close();
    }
    // Free ssl!
    SSL_free( ssl);
}

/* 
 * Connects the socket 
 */
bool SSLTCPSocket::connect() {
    if (!TCPSocket::connect())
        return false;
    if (SSL_connect(ssl) != 1) {
        setConnected(false);
    }
    return isConnected();
}

/* 
 * Closes the connection 
 */
bool SSLTCPSocket::close() {
    if (!isConnected())
        return false;
    if (!SSL_shutdown(ssl))
        return false;
    return TCPSocket::close();
}

/* 
 * Reads data from the socket 
 */
String SSLTCPSocket::readStringFromSocket() throw (ConnectionClosedException) {
    if (!isConnected())
        return "";
    char buf[STRING_BUFFER_LENGTH] = { 0 };

    std::stringstream result("");

    int received = STRING_BUFFER_LENGTH;
    while (received == STRING_BUFFER_LENGTH) {
        received = SSL_read(ssl, buf, STRING_BUFFER_LENGTH);
        if (received > 0) {
            result << std::string(buf, received);
        } else {
            // TODO: This might not mean that the connection is closed
            setConnected(false);
            throw ConnectionClosedException(
                    "Connection was closed while reading data");
        }
    }

    return result.str();
}

/* 
 * Sends a string to the socket 
 */
bool SSLTCPSocket::writeString(String const & string)
        throw (ConnectionClosedException) {
    if (!isConnected())
        return false;
    return SSL_write(ssl, string.c_str(), string.length())
            == (int) string.length();
}

/* 
 * Reads a character from the socket 
 */
char SSLTCPSocket::readChar() throw (ConnectionClosedException) {
    if (!isConnected())
        return 0;
    char character;

    if (SSL_read(ssl, &character, 1) != 1) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while reading a character");
    }
    return character;
}

/* 
 * Writes a character to the socket 
 */
bool SSLTCPSocket::writeChar(char character) throw (ConnectionClosedException) {
    if (!isConnected())
        return false;
    if (SSL_write(ssl, &character, 1) != 1) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while writing a character");
    }
    return isConnected();
}

/* 
 * Reads a number of characters into a buffer 
 */
bool SSLTCPSocket::read(char * buffer, unsigned int length)
        throw (ConnectionClosedException) {
    if (!isConnected())
        return false;
    int recvd = SSL_read(ssl, buffer, length);
    if (recvd <= 0 || recvd != (int) length) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while reading data");
    }
    return isConnected();
}

/* 
 * Writes a buffer into the socket 
 */
bool SSLTCPSocket::write(char const * buffer, unsigned int length)
        throw (ConnectionClosedException) {
    if (!isConnected())
        return false;
    int sent = SSL_write(ssl, buffer, length);
    if (sent <= 0 || sent != (int) length) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while writing data");
    }
    return isConnected();
}
