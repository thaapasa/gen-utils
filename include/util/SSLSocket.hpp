#if !defined(UTIL_SSLSOCKET_HPP)
#define UTIL_SSLSOCKET_HPP

/*
 * Wrapper for TCP and UDP sockets, encrypted with SSL encryption. 
 * The implementation wraps OpenSSL to a nice, clean interface.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-10-12
 */

#include "util/Socket.hpp"

#ifndef SSL
#error Please specify -DSSL when building with SSL support
#endif

// Forward definition for the SSL context struct
typedef struct ssl_ctx_st SSL_CTX;
// Forward definition for the SSL type
typedef struct ssl_st SSL;

namespace util {

class SSLSocket {
public:
    SSLSocket();
    virtual ~SSLSocket();

protected:
    static SSL_CTX * context;

private:
    static bool initialized;

};

/*
 * SSL TCP socket
 */
class SSLTCPSocket: public virtual SSLSocket, public TCPSocket {
public:
    /* Creates a TCP socket to given host & port */
    SSLTCPSocket(String const & host, unsigned int port);
    /* Creates a TCP socket to given ip & port */
    SSLTCPSocket(unsigned short ip1, unsigned short ip2, unsigned short ip3,
            unsigned short ip4, unsigned int port);
    /* Creates a TCP socket wrapper for an existing socket descriptor */
    SSLTCPSocket(String const & host, unsigned int port, int descriptor);

    /* Deletes the socket, closing it if it's still connected */
    virtual ~SSLTCPSocket();

    /* Connects the socket */
    virtual bool connect();

    /* Closes the connection */
    virtual bool close();

    /* Sends a string to the socket */
    virtual bool writeString(String const & string)
            throw (ConnectionClosedException);

    /* Reads a character from the socket */
    virtual char readChar() throw (ConnectionClosedException);
    /* Writes a character to the socket */
    virtual bool writeChar(char character) throw (ConnectionClosedException);

    /* Reads a number of characters into a buffer */
    virtual bool read(char * buffer, unsigned int length)
            throw (ConnectionClosedException);
    /* Writes a buffer into the socket */
    virtual bool write(char const * buffer, unsigned int length)
            throw (ConnectionClosedException);

private:
    /* Reads data from the socket */
    virtual String readStringFromSocket() throw (ConnectionClosedException);

private:
    SSL * ssl;
    String buffer;

};

}

#endif // !defined(UTIL_SSLSOCKET_HPP)
