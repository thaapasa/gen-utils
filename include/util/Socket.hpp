#if !defined(UTIL_SOCKET_HPP)
#define UTIL_SOCKET_HPP

/*
 * Wrapper for standard TCP and UDP sockets.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-10-12
 */

#include "util/String.hpp"
#include "util/exception/SocketException.hpp"

namespace util {

/*
 * The base class for all sockets
 */
class Socket {
public:
    static String const linefeed;

    /* Opens connection to a given port on the host */
    Socket(String const & host, unsigned int port);
    /* Destroys the socket */
    virtual ~Socket();

    /* Connects the socket */
    virtual bool connect() = 0;
    /* Closes the connection */
    virtual bool close() = 0;

    /* Binds a local address to the socket */
    virtual bool bind() = 0;

    /* Reads a sent packet, returning it as a string */
    virtual String readString() throw (ConnectionClosedException) = 0;
    /* 
     * Reads all data up to a string delimiter. 
     * The returned data does not contain the delimiter. 
     */
    virtual String readString(String const & delimiter)
            throw (ConnectionClosedException) = 0;
    /* Reads a line of text from the socket. */
    virtual String readLine() throw (ConnectionClosedException) {
        return readString(linefeed);
    }
    /* Sends a string to the socket */
    virtual bool writeString(String const & string)
            throw (ConnectionClosedException) = 0;

    /* Reads a character from the socket */
    virtual char readChar() throw (ConnectionClosedException) = 0;
    /* Writes a character to the socket */
    virtual bool writeChar(char character) throw (ConnectionClosedException) = 0;

    /* Reads a number of characters into a buffer */
    virtual bool read(char * buffer, unsigned int length)
            throw (ConnectionClosedException) = 0;
    /* Writes a buffer into the socket */
    virtual bool write(char const * buffer, unsigned int length)
            throw (ConnectionClosedException) = 0;

    /* Tests whether the socket is connected */
    bool isConnected() const {
        return connected;
    }
    /* Tests whether the socket is bound */
    bool isBound() const {
        return bound;
    }

    /* Returns the address of the remote host, as given to the constructor. */
    String const & getHost() const {
        return host;
    }
    /* Returns the IP of the remote host, if it could be resolved. */
    String const & getIP() {
        if (ip == "")
            setIP(host);
        return ip;
    }
    /* Returns the port of the remote host */
    unsigned int getPort() const {
        return port;
    }

protected:
    /* Sets the socket's connection state */
    void setConnected(bool state) {
        connected = state;
    }
    /* Sets the socket's bound state */
    void setBound(bool state) {
        bound = state;
    }
    /* Sets the socket's IP from a hostname (or IP) */
    void setIP(String const & host);

private:
    String host;
    String ip;
    unsigned int port;
    bool bound;
    bool connected;

#if defined(WIN32)
    static bool wsaInitialized;
#endif // defined(WIN32)
};

/*
 * Base class for server sockets.
 */
class ServerSocket: public virtual Socket {
public:
    /* Creates a server socket that listens to given port */
    ServerSocket(unsigned int port);
    /* Destroys the socket */
    virtual ~ServerSocket();

    /* Accepts an incoming socket connection */
    virtual Socket * accept() = 0;
    /* Starts to listen to the socket */
    virtual bool listen() = 0;

};

/*
 * TCP socket
 */
class TCPSocket: public virtual Socket {
public:
    /* Creates a TCP socket to given host & port */
    TCPSocket(String const & host, unsigned int port);
    /* Creates a TCP socket to given ip & port */
    TCPSocket(unsigned short ip1, unsigned short ip2, unsigned short ip3,
            unsigned short ip4, unsigned int port);
    /* Creates a TCP socket wrapper for an existing socket descriptor */
    TCPSocket(String const & host, unsigned int port, int descriptor);

    /* Deletes the socket, closing it if it's still connected */
    virtual ~TCPSocket();

    /* Connects the socket */
    virtual bool connect();

    /* Closes the connection */
    virtual bool close();

    /* Binds a local address to the socket */
    virtual bool bind();

    /* Reads a sent packet, returning it as a string */
    virtual String readString() throw (ConnectionClosedException);
    /* 
     * Reads all data up to a string delimiter. 
     * The returned data does not contain the delimiter. 
     */
    virtual String readString(String const & delimiter)
            throw (ConnectionClosedException);

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

protected:
    /* Returns the socket descriptor */
    int getDescriptor() {
        return descriptor;
    }

    /* Reads data from the socket */
    virtual String readStringFromSocket() throw (ConnectionClosedException);

private:
    String buffer;
    int descriptor;

};

/*
 * TCP server socket
 */
class TCPServerSocket: public ServerSocket, public TCPSocket {
public:
    /* Creates a TCP server socket that listens to given port */
    TCPServerSocket(unsigned int port);

protected:
    /* 
     * Creates a TCP server socket that listens to given port from an already
     * created descriptor 
     */
    TCPServerSocket(unsigned int port, int descriptor);

public:
    /* Binds the local port to the socket */
    virtual bool bind();

    /* Accepts an incoming socket connection */
    virtual Socket * accept();

    /* Starts to listen to the socket */
    virtual bool listen();

};

/*
 * UDP sockets.
 *
 * The implementation only differs from TCPSocket in the creation of the
 * socket descriptor. Remember to use connect() to define the end-point
 * for the communication (this allows the usage of ::send() instead of
 * ::sendto())
 */
class UDPSocket: public TCPSocket {
public:
    UDPSocket(String const & ip, unsigned int port);
    UDPSocket(unsigned short, unsigned short, unsigned short, unsigned short,
            unsigned int port);
    UDPSocket(String const & ip, unsigned int port, int descriptor);

    /* Sends a string to the socket */
    virtual bool writeString(String const & string)
            throw (ConnectionClosedException);
    /* Writes a character to the socket */
    virtual bool writeChar(char character) throw (ConnectionClosedException);
    /* Writes a buffer into the socket */
    virtual bool write(char const * buffer, unsigned int length)
            throw (ConnectionClosedException);

    /* Reads a character from the socket */
    virtual char readChar() throw (ConnectionClosedException);
    /* Reads a number of characters into a buffer */
    virtual bool read(char * buffer, unsigned int length)
            throw (ConnectionClosedException);

protected:
    /* Reads data from the socket */
    virtual String readStringFromSocket() throw (ConnectionClosedException);

};

/*
 * UDP server sockets
 */
class UDPServerSocket: public ServerSocket, public virtual Socket {
public:
    UDPServerSocket(unsigned int port);

    /* Connect */
    virtual bool connect() {
        setConnected(true);
        return true;
    }
    /* Starts listening on the socket by calling bind */
    virtual bool listen();
    /* Closes the connection - ignored with UDP sockets */
    virtual bool close() {
        setConnected(false);
        return true;
    }

    /* Binds the local address to the socket */
    virtual bool bind();

    /* 
     * Accepts an incoming transmission (since there are no connections with
     * UDP sockets, just returns this socket instance
     */
    virtual Socket * accept() {
        return this;
    }

    /* Reads a sent packet, returning it as a string */
    virtual String readString() throw (ConnectionClosedException);

    /* 
     * Reads all data up to a string delimiter. 
     * The returned data does not contain the delimiter. 
     */
    virtual String readString(String const & delimiter)
            throw (ConnectionClosedException);

    /* Reads a character from the socket */
    virtual char readChar() throw (ConnectionClosedException);

    /* Reads a number of characters into a buffer */
    virtual bool read(char * buffer, unsigned int length)
            throw (ConnectionClosedException);

    /*
     * The writing operations will fail as UDP is connectionless.
     */
    virtual bool writeString(String const & string)
            throw (ConnectionClosedException);
    virtual bool writeChar(char character) throw (ConnectionClosedException);
    virtual bool write(const char * source, unsigned int size)
            throw (ConnectionClosedException);

private:
    /* Reads data from the socket */
    virtual String readStringFromSocket() throw (ConnectionClosedException);

private:
    String buffer;
    int descriptor;

};

}

#endif // !defined(UTIL_SOCKET_HPP)
