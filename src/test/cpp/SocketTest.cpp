#include "util/Socket.hpp"
#include "util/Log.hpp"

#ifdef SSL
#include "util/SSLSocket.hpp"
#endif // SSL
#include <iostream>

using namespace util;
using namespace std;

inline void testTCP() {
    cout << endl;
    Socket * sock = new TCPSocket("127.0.0.1", 12742);
    ASSERT(sock->connect());
    ASSERT(sock->isConnected());

    ASSERT(sock->writeString("Morjensta" + Socket::linefeed));

    String read = sock->readLine();
    cout << "READ " << read.length() << ": \"" << read << "\"" << endl;
    read = sock->readLine();
    cout << "READ " << read.length() << ": \"" << read << "\"" << endl;
    read = sock->readLine();
    cout << "READ " << read.length() << ": \"" << read << "\"" << endl;
    read = sock->readLine();
    cout << "READ " << read.length() << ": \"" << read << "\"" << endl;

    ASSERT(sock->isConnected());

    cout << "..................... ";
}

void testWeb() {
    String host = "www.tietokilta.fi";
    Socket * sock = new TCPSocket(host, 80);
    ASSERT(sock->connect());
    ASSERT(sock->isConnected());

    ASSERT(sock->writeString("GET / HTTP/1.1" + Socket::linefeed));
    ASSERT(sock->writeString("Host: " + host + Socket::linefeed));
    ASSERT(sock->writeString("Connection: close" + Socket::linefeed));
    ASSERT(sock->writeString(Socket::linefeed));

    cout << sock->readString();
    delete sock;
}

#ifdef SSL
void testSSL()
{
    String host = "intra.mmd.net";
    Socket * sock = new SSLTCPSocket(host, 443);
    ASSERT(sock->connect());
    ASSERT(sock->isConnected());

    ASSERT(sock->writeString("GET / HTTP/1.1" + Socket::linefeed));
    ASSERT(sock->writeString("Host: " + host + Socket::linefeed));
    ASSERT(sock->writeString("Connection: close" + Socket::linefeed));
    ASSERT(sock->writeString(Socket::linefeed));

    cout << sock->readString();
    delete sock;
}
#endif // SSL

int main(int argc, char ** argv) {
    cout << "Testing Sockets.........." << endl;
    cout << "-------------------------" << endl;

    cout << "TCP Sockets ......... ";
    //testTCP();
    cout << " ok" << std::endl;

    cout << "Connecting to web ... ";
    testWeb();
    cout << " ok" << std::endl;

#ifdef SSL
    cout << "Connecting to SSL web ... ";
    testSSL();
    cout << " ok" << std::endl;
#endif // SSL 
    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}
