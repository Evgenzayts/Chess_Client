#ifndef CLIENT_H
#define CLIENT_H

#include "client.h"
#include <iostream>
#include <QObject>

using namespace std;

class Client_network
{
private:
    Client *my_client;

public:
    Client_network();

    bool send_message();

    bool recieve_message();

    bool first_connection();

    bool init_server();

    void set_new_send_msg(size_t new_message);

    void finish();

    size_t get_recv_msg();

    ~Client_network() {
        delete my_client;
    }
};

#endif // CLIENT_H
