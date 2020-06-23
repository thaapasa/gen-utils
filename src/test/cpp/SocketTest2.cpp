#include "util/Socket.hpp"
#include "util/Log.hpp"
#include "util/Thread.hpp"
#include "util/Deletor.hpp"

#define LINE1 "This is my packet\r\n"
#define LINE2 "And this is the other packet\r\n"

#include <iostream>

using namespace util;
using namespace std;

void testTCPServer() {
    ServerSocket * sock = new TCPServerSocket(12742);
    ASSERT(sock->listen());
    Socket * remote = sock->accept();
    ASSERT(remote != 0);
    ASSERT(remote->isConnected());

    ASSERT(remote->writeString(Socket::linefeed));
    ASSERT(remote->writeString("Kuta kuuluu" + Socket::linefeed));
    ASSERT(remote->writeString("Rivi 1" + Socket::linefeed + "Rivi2"
            + Socket::linefeed));

    delete remote;
    delete sock;

}

class ServerRunner: public Runnable {
    void stop() {
    }
    int run() {
        ServerSocket * sock = new UDPServerSocket(1234);
        sock->listen();
        ASSERT(sock->readString() == LINE1);
        ASSERT(sock->readString() == LINE2);

        delete sock;
        return Thread::ExitCodeOK;
    }
};

Thread * serverThread = 0;
Runnable * server = 0;

void testUDPClient() {
    Socket * sock = new UDPSocket("localhost", 1234);
    ASSERT(sock->connect());
    ASSERT(sock->writeString(LINE1));
    ASSERT(sock->writeString(LINE2));
    serverThread->waitForExit();
    delete sock;
    delete serverThread;
    delete server;
}

void testUDPServer() {
    server = new ServerRunner();
    serverThread = new Thread(server);
    serverThread->start();
}

int main(int argc, char ** argv) {
    cout << "Testing Sockets.........." << endl;
    cout << "-------------------------" << endl;

    //cout << "TCP server socket ... ";
    //testTCPServer();
    //cout << " ok" << std::endl;

    cout << "UDP client server ... ";
    testUDPServer();
    cout << " ok" << std::endl;

    cout << "UDP client socket ... ";
    testUDPClient();
    cout << " ok" << std::endl;

    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;
    return 0;
}

