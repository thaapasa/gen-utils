#include "util/Socket.hpp"

/*
 * Wrapper for standard TCP and UDP sockets.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-10-12
 * 
 * $Id: Socket.cpp,v 1.21 2005-03-01 11:08:40 tuska Exp $
 */

#if defined(WIN32)
#include <winsock2.h>
#include <sys/types.h>
typedef int socklen_t;
#else // defined(WIN32)
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif // defined(WIN32)
#include <unistd.h>
#include <sstream>
#include <cstring>

#include "util/Log.hpp"

#define STRING_BUFFER_LENGTH 1024
#define MAX_PENDING_CONNECTIONS 5

using namespace util;

#if defined(WIN32)
bool Socket::wsaInitialized = 0;
#endif // defined(WIN32)
// Miscellaneous
// --------------------------------------------------------------------------

/*
 * Gets a part of an IP address (for example, part 2 of "194.65.233.23" 
 * would be 65
 */
static unsigned short getIPPart(struct in_addr * addr, int part) {
    switch (part) {
#if defined(WIN32)
    case 1:
    return addr->S_un.S_un_b.s_b1;
    case 2:
    return addr->S_un.S_un_b.s_b2;
    case 3:
    return addr->S_un.S_un_b.s_b3;
    case 4:
    return addr->S_un.S_un_b.s_b4;
#else // defined(WIN32)
    case 1:
        return addr->s_addr & 0xFF;
    case 2:
        return (addr->s_addr >> 8) & 0xFF;
    case 3:
        return (addr->s_addr >> 16) & 0xFF;
    case 4:
        return (addr->s_addr >> 24) & 0xFF;
#endif // defined(WIN32)
    default:
        break;
    }

    ERROR("Invalid IP part requested");
    return 0;
}

/*
 * Gets the IP from a hostname. If host is directly in IP format, just returns
 * it, otherwise tries to determine the IP with ::gethostbyname()
 */
static String getIPFromHost(String const & host) {
#if defined(WIN32) 
    unsigned long ipNum = 0;
    ipNum = inet_addr(host.c_str());
    if (ipNum != INADDR_NONE) {
#else // defined(WIN32) 
    in_addr ipNum;
    if (inet_aton(host.c_str(), &ipNum) != 0) {
#endif // defined(WIN32) 
        return host;
    } else {
        struct hostent * h = gethostbyname(host.c_str());
        if (h == 0) {
            return "";
        }

        struct in_addr addr;
        memcpy((char *) &(addr.s_addr), h->h_addr_list[0],
                sizeof(struct in_addr));

        return String::format("%d.%d.%d.%d", getIPPart(&addr, 1), getIPPart(
                &addr, 2), getIPPart(&addr, 3), getIPPart(&addr, 4));
    }
}

/*
 * Binds a local port for a client socket
 */
static bool bindClient(int descriptor) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    // Fill in address struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(0);

    // Bind the local port
    if (::bind(descriptor, (struct sockaddr *) &address, sizeof(address)) != 0) {
        WARN("Cannot bind local port for client");
        return false;
    }
    return true;
}

/*
 * Binds the given port for a server socket
 */
static int bindServer(int descriptor, int port) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    // Fill in address struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    // Bind the local port
    if (::bind(descriptor, (struct sockaddr *) &address, sizeof(address)) != 0) {
        WARN("Cannot bind local port " << port << " for server");
        return false;
    }
    return true;
}

/*
 * Connects to the given remote host
 */
static bool connectRemote(int descriptor, String const & ip, int port) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;

#if defined(WIN32) 
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    if (address.sin_addr.s_addr == INADDR_NONE) {
#else // defined(WIN32) 
    if (inet_aton(ip.c_str(), &address.sin_addr) == 0) {
#endif // defined(WIN32) 
        WARN("An invalid IP address given to connectRemote");
        return false;
    }
    address.sin_port = htons(port);

    if (::connect(descriptor, (struct sockaddr *) &address, sizeof(address))
            != 0) {
        // Could not connect to the remote host
        // WARNING("Could not connect to remote host");
        return false;
    }

    return true;
}

/*
 * Sends a message to the given remote host
 */
static bool sendToRemote(int descriptor, char const * buffer, size_t length,
        String const & ip, int port) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;

#if defined(WIN32) 
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    if (address.sin_addr.s_addr == INADDR_NONE) {
#else // defined(WIN32) 
    if (inet_aton(ip.c_str(), &address.sin_addr) == 0) {
#endif // defined(WIN32) 
        WARN("An invalid IP address given to sendToRemote");
        return false;
    }
    address.sin_port = htons(port);

    if (::sendto(descriptor, buffer, length, 0, (struct sockaddr *) &address,
            sizeof(address)) != (int) length) {
        // Could not send message to the remote host
        WARN("Could not send message to remote host");
        return false;
    }

    return true;
}

/*
 * Receives a message from a given remote host
 */
static int receiveFromRemote(int descriptor, char * buffer, size_t length,
        String const & ip, int port) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;

#if defined(WIN32) 
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    if (address.sin_addr.s_addr == INADDR_NONE) {
#else // defined(WIN32) 
    if (inet_aton(ip.c_str(), &address.sin_addr) == 0) {
#endif // defined(WIN32) 
        WARN("An invalid IP address given to sendToRemote");
        return -1;
    }
    address.sin_port = htons(port);

    socklen_t size = sizeof(address);
    return ::recvfrom(descriptor, buffer, length, 0,
            (struct sockaddr *) &address, &size);
}

// Socket
// --------------------------------------------------------------------------

String const Socket::linefeed = "\r\n";

/* 
 * Opens connection to a given port on the host 
 */
Socket::Socket(String const & host_, unsigned int port_) :
    host(host_), port(port_), bound(false), connected(false) {

#if defined(WIN32)
    if (!wsaInitialized) {
        LOG("Initializing Winsock (WSA)");
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR) {
            WARN("Error in initializing Winsock");
        }
        wsaInitialized = true;
    }
#endif // defined(WIN32)
}

/* 
 * Destroys the socket 
 */
Socket::~Socket() {
}

/* 
 * Sets the socket's IP from a hostname (or IP)
 */
void Socket::setIP(String const & host) {
    ip = getIPFromHost(host);
    if (ip == "") {
        WARN("Could not determine IP from \"" << host << "\"");
    }
}

// ServerSocket
// --------------------------------------------------------------------------

/* 
 * Creates a server socket that listens to given port 
 */
ServerSocket::ServerSocket(unsigned int port) :
    Socket("", port) {
}

/* 
 * Destroys the socket 
 */
ServerSocket::~ServerSocket() {
}

// TCPSocket
// --------------------------------------------------------------------------

/* 
 * Creates a TCP socket to given host & port 
 */
TCPSocket::TCPSocket(String const & host, unsigned int port) :
    Socket(host, port), buffer("") {
    descriptor = socket(AF_INET, SOCK_STREAM, 0);
}

/* 
 * Creates a TCP socket to given ip & port 
 */
TCPSocket::TCPSocket(unsigned short ip1, unsigned short ip2,
        unsigned short ip3, unsigned short ip4, unsigned int port) :
    Socket(String::format("%d.%d.%d.%d", ip1, ip2, ip3, ip4), port), buffer("") {
    descriptor = socket(AF_INET, SOCK_STREAM, 0);
}

/* 
 * Creates a TCP socket wrapper for an existing socket descriptor 
 */
TCPSocket::TCPSocket(String const & host, unsigned int port, int descriptor_) :
    Socket(host, port), buffer("") {
    descriptor = descriptor_;
    if (descriptor < 0) {
        WARN("Created a socket for invalid descriptor (" << descriptor << "%d)");
    }
}

/*
 * Deletes the socket, closing it if it's still connected
 */
TCPSocket::~TCPSocket() {
    if (isConnected()) {
        close();
    }
}

/* 
 * Binds the local address to the socket
 */
bool TCPSocket::bind() {
    if (descriptor < 0)
        return false;
    if (isBound())
        return true;
    if (isConnected())
        return false;

    if (!::bindClient(descriptor)) {
        return false;
    }

    setBound(true);
    return true;
}

/* 
 * Connects the socket 
 */
bool TCPSocket::connect() {
    if (descriptor < 0)
        return false;
    if (isConnected())
        return false;

    if (!isBound() && !bind()) {
        WARN("Could not bind local socket for client in connect");
        return false;
    }

    setIP(getHost());
    if (!connectRemote(descriptor, getIP(), getPort())) {
        return false;
    }

    setConnected(true);
    return true;
}

/* 
 * Closes the connection 
 */
bool TCPSocket::close() {
    if (!isConnected())
        return false;
    ::close(descriptor);
    setConnected(false);
    return true;
}

/* 
 * Reads a sent packet, returning it as a string 
 */
String TCPSocket::readString() throw (ConnectionClosedException) {
    String result = buffer;
    buffer = "";
    return result + readStringFromSocket();
}

/* 
 * Reads data from the socket 
 */
String TCPSocket::readStringFromSocket() throw (ConnectionClosedException) {
    if (!isConnected())
        return "";
    char buf[STRING_BUFFER_LENGTH] = { 0 };
    memset(buf, STRING_BUFFER_LENGTH, 0);

    std::stringstream result;

    int received = STRING_BUFFER_LENGTH;
    while (received == STRING_BUFFER_LENGTH) {
        received = ::recv(descriptor, buf, STRING_BUFFER_LENGTH, 0);
        if (received > 0) {
            result << std::string(buf, received);
        }
        if (received == -1) {
            // TODO: This might not mean that the connection is closed
            setConnected(false);
            throw ConnectionClosedException(
                    "Connection was closed while reading data");
        }
    }

    return result.str();
}

/* 
 * Reads all data up to a string delimiter. 
 * The returned data does not contain the delimiter. 
 */
String TCPSocket::readString(String const & delimiter)
        throw (ConnectionClosedException) {
    // TODO: This must be very slow...
    String::size_type pos = buffer.find(delimiter);
    while (pos == String::npos) {
        if (!isConnected())
            return "";
        // Try to get more data
        buffer += readStringFromSocket();
        pos = buffer.find(delimiter);
    }
    String result = buffer.substr(0, pos);
    buffer = buffer.substr(pos + delimiter.length());
    return result;
}

/* 
 * Sends a string to the socket 
 */
bool TCPSocket::writeString(String const & string)
        throw (ConnectionClosedException) {
    if (!isConnected()) {
        WARN("Trying to write to a non-connected socket");
        return false;
    }
    int sent = ::send(descriptor, string.c_str(), string.length(), 0);
    if (sent != (int) string.length()) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while sending data");
    }
    return isConnected();
}

/* 
 * Reads a character from the socket 
 */
char TCPSocket::readChar() throw (ConnectionClosedException) {
    if (!isConnected())
        return 0;
    char character = 0;
    int recvd = ::recv(descriptor, &character, 1, 0);
    if (recvd != 1) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while reading a character");
    }
    return character;
}

/* 
 * Writes a character to the socket 
 */
bool TCPSocket::writeChar(char character) throw (ConnectionClosedException) {
    if (!isConnected()) {
        WARN("Trying to write to a non-connected socket");
        return false;
    }
    int sent = ::send(descriptor, &character, 1, 0);
    if (sent != 1) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while sending a character");
    }
    return isConnected();
}

/* 
 * Reads a number of characters into a buffer 
 */
bool TCPSocket::read(char * buffer, unsigned int length)
        throw (ConnectionClosedException) {
    if (!isConnected())
        return false;
    int recvd = ::recv(descriptor, buffer, length, 0);
    if (recvd < 0 || recvd != (int) length) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while reading data");
    }
    return isConnected();
}

/* 
 * Writes a buffer into the socket 
 */
bool TCPSocket::write(char const * buffer, unsigned int length)
        throw (ConnectionClosedException) {
    if (!isConnected()) {
        WARN("Trying to write to a non-connected socket");
        return false;
    }
    int sent = ::send(descriptor, buffer, length, 0);
    if (sent < 0 || sent != (int) length) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while writing data");
    }
    return isConnected();
}

// TCPServerSocket
// --------------------------------------------------------------------------

/*
 * Creates a TCP server socket that listens to given port   
 */
TCPServerSocket::TCPServerSocket(unsigned int port) :
    Socket("", port), ServerSocket(port), TCPSocket("", port) {
}

/*
 * Creates a TCP server socket that listens to given port from an already 
 * created descriptor
 */
TCPServerSocket::TCPServerSocket(unsigned int port, int descriptor) :
    Socket("", port), ServerSocket(port), TCPSocket("", port, descriptor) {
}

/* 
 * Binds the local address to the socket
 */
bool TCPServerSocket::bind() {
    if (getDescriptor() < 0)
        return false;
    if (isBound())
        return true;
    if (isConnected())
        return false;

    if (!::bindServer(getDescriptor(), getPort())) {
        return false;
    }

    setBound(true);
    return true;
}

/* 
 * Starts to listen to the socket
 */
bool TCPServerSocket::listen() {
    if (getDescriptor() < 0)
        return false;
    if (isConnected())
        return false;

    if (!isBound()) {
        if (!bind())
            return false;
    }

    // Start listening
    setConnected(::listen(getDescriptor(), MAX_PENDING_CONNECTIONS) == 0);
    return isConnected();
}

/* 
 * Accepts an incoming socket connection
 */
Socket * TCPServerSocket::accept() {
    if (getDescriptor() < 0)
        return NULL;
    if (!isBound())
        return NULL;
    if (!isConnected())
        return NULL;

    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    int remoteDesc = ::accept(getDescriptor(), (struct sockaddr *) &address,
            &addressLength);
    if (remoteDesc < 0)
        return 0;

    Socket * socket = new TCPSocket(inet_ntoa(address.sin_addr), ntohs(
            address.sin_port), remoteDesc);
    socket->connect();
    return socket;
}

// UDPSocket
// --------------------------------------------------------------------------

UDPSocket::UDPSocket(String const & ip, unsigned int port) :
    Socket(ip, port), TCPSocket(ip, port, socket(AF_INET, SOCK_DGRAM, 0)) {
}

UDPSocket::UDPSocket(String const & ip, unsigned int port, int descriptor) :
    Socket(ip, port), TCPSocket(ip, port, descriptor) {
}

UDPSocket::UDPSocket(unsigned short ip1, unsigned short ip2,
        unsigned short ip3, unsigned short ip4, unsigned int port) :
    Socket(String::format("%d.%d.%d.%d", ip1, ip2, ip3, ip4), port), TCPSocket(
            String::format("%d.%d.%d.%d", ip1, ip2, ip3, ip4), port, socket(
                    AF_INET, SOCK_DGRAM, 0)) {
}

/* 
 * Sends a string to the socket 
 */
bool UDPSocket::writeString(String const & string)
        throw (ConnectionClosedException) {
    if (isConnected())
        return TCPSocket::writeString(string);

    if (!isBound() && !bind()) {
        WARN("Could not bind local socket for client");
        return false;
    }

    return sendToRemote(getDescriptor(), string.c_str(), string.length(),
            getIP(), getPort());
}

/* 
 * Writes a character to the socket 
 */
bool UDPSocket::writeChar(char character) throw (ConnectionClosedException) {
    if (isConnected())
        return TCPSocket::writeChar(character);

    if (!isBound() && !bind()) {
        WARN("Could not bind local socket for client");
        return false;
    }

    return sendToRemote(getDescriptor(), &character, sizeof(char), getIP(),
            getPort());
}

/* 
 * Writes a buffer into the socket 
 */
bool UDPSocket::write(char const * buffer, unsigned int length)
        throw (ConnectionClosedException) {
    if (isConnected())
        return TCPSocket::write(buffer, length);

    if (!isBound() && !bind()) {
        WARN("Could not bind local socket for client");
        return false;
    }

    return sendToRemote(getDescriptor(), buffer, length, getIP(), getPort());
}

/* 
 * Reads a character from the socket 
 */
char UDPSocket::readChar() throw (ConnectionClosedException) {
    if (isConnected())
        return TCPSocket::readChar();

    if (!isBound() && !bind()) {
        WARN("Could not bind local socket for client");
        return false;
    }

    {
        char character;
        return receiveFromRemote(getDescriptor(), &character,
                sizeof(character), getIP(), getPort()) == sizeof(character);
    }
}

/* 
 * Reads a number of characters into a buffer 
 */
bool UDPSocket::read(char * buffer, unsigned int length)
        throw (ConnectionClosedException) {
    if (isConnected())
        return TCPSocket::read(buffer, length);

    if (!isBound() && !bind()) {
        WARN("Could not bind local socket for client");
        return false;
    }

    return receiveFromRemote(getDescriptor(), buffer, length, getIP(),
            getPort()) == (int) length;
}

/* 
 * Reads data from the socket 
 */
String UDPSocket::readStringFromSocket() throw (ConnectionClosedException) {
    if (isConnected())
        return TCPSocket::readStringFromSocket();

    if (!isBound() && !bind()) {
        WARN("Could not bind local socket for client");
        return "";
    }

    {
        char buf[STRING_BUFFER_LENGTH] = { 0 };
        memset(buf, STRING_BUFFER_LENGTH, 0);

        std::stringstream result;

        int received = STRING_BUFFER_LENGTH;
        while (received == STRING_BUFFER_LENGTH) {
            received = receiveFromRemote(getDescriptor(), buf,
                    STRING_BUFFER_LENGTH, getIP(), getPort());
            if (received > 0) {
                result << std::string(buf, received);
            }
            if (received == -1) {
                // TODO: This might not mean that the connection is closed
                setConnected(false);
                throw ConnectionClosedException(
                        "Connection was closed while reading data");
            }
        }

        return result.str();
    }
}

// UDPServerSocket
// --------------------------------------------------------------------------

UDPServerSocket::UDPServerSocket(unsigned int port) :
    Socket("", port), ServerSocket(port) {
    descriptor = socket(AF_INET, SOCK_DGRAM, 0);
}

/* 
 * Starts listening on the socket 
 */
bool UDPServerSocket::listen() {
    return bind();
}

/* 
 * Binds the local address to the socket
 */
bool UDPServerSocket::bind() {
    if (descriptor < 0)
        return false;
    struct sockaddr_in address;

    // Fill in address struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(getPort());

    // Bind the server port
    if (::bind(descriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
        WARN("Cannot bind port " << getPort() << " for UDP server");
        return false;
    }

    // Start listening
    setConnected(true);
    return isConnected();
}

/* 
 * Reads a sent packet, returning it as a string 
 */
String UDPServerSocket::readString() throw (ConnectionClosedException) {
    String result = buffer;
    buffer = "";
    return result + readStringFromSocket();
}

/* 
 * Reads all data up to a string delimiter. 
 * The returned data does not contain the delimiter. 
 */
String UDPServerSocket::readString(String const & delimiter)
        throw (ConnectionClosedException) {
    // TODO: This must be very slow...
    String::size_type pos = buffer.find(delimiter);
    while (pos == String::npos) {
        if (!isConnected())
            return "";
        // Try to get more data
        buffer += readStringFromSocket();
        pos = buffer.find(delimiter);
    }
    String result = buffer.substr(0, pos);
    buffer = buffer.substr(pos + delimiter.length());
    return result;
}

/* 
 * Reads a character from the socket 
 */
char UDPServerSocket::readChar() throw (ConnectionClosedException) {
    char character = 0;
    int recvd = ::recvfrom(descriptor, &character, 1, 0, NULL, NULL);
    if (recvd != 1) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while reading a character");
    }
    return character;
}

/* 
 * Reads a number of characters into a buffer 
 */
bool UDPServerSocket::read(char * buffer, unsigned int length)
        throw (ConnectionClosedException) {
    int recvd = ::recvfrom(descriptor, buffer, length, 0, NULL, NULL);
    if (recvd < 0 || recvd != (int) length) {
        setConnected(false);
        throw ConnectionClosedException(
                "Connection was closed while reading data");
    }
    return true;
}

/*
 * The writing operations will fail as UDP is connectionless.
 */
bool UDPServerSocket::writeString(String const & string)
        throw (ConnectionClosedException) {
    ERROR("Cannot write to UDP server socket");
    return false;
}

bool UDPServerSocket::writeChar(char character)
        throw (ConnectionClosedException) {
    ERROR("Cannot write to UDP server socket");
    return false;
}

bool UDPServerSocket::write(const char * source, unsigned int size)
        throw (ConnectionClosedException) {
    ERROR("Cannot write to UDP server socket");
    return false;
}

/* 
 * Reads data from the socket 
 */
String UDPServerSocket::readStringFromSocket()
        throw (ConnectionClosedException) {
    if (!isConnected())
        return "";
    char buf[STRING_BUFFER_LENGTH] = { 0 };
    memset(buf, STRING_BUFFER_LENGTH, 0);

    std::stringstream result;

    int received = STRING_BUFFER_LENGTH;
    while (received == STRING_BUFFER_LENGTH) {
        received = recvfrom(descriptor, buf, STRING_BUFFER_LENGTH, 0, NULL,
                NULL);
        if (received > 0) {
            result << std::string(buf, received);
        }
        if (received == -1) {
            // TODO: This might not mean that the connection is closed
            setConnected(false);
            throw ConnectionClosedException(
                    "Connection was closed while reading data");
        }
    }

    return result.str();
}
