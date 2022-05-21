#include "Class_client.h"

Client_network::Client_network()
    : my_client(new Client)
{
    if (!init_server())
        exit (1);
}

bool Client_network::send_message()
{
    return _send_message(my_client);
}

bool Client_network::recieve_message()
{
    return _recieve_message(my_client);
}

bool Client_network::first_connection()
{
    return _first_connection(my_client);
}

bool Client_network::init_server()
{
    return _init_server(my_client);
}

void Client_network::set_new_send_msg(size_t new_message)
{
    _set_new_send_msg(my_client, new_message);
}

void Client_network::finish()
{
    _finish(my_client);
}

size_t Client_network::get_recv_msg()
{
    return my_client->_recv_message;
}
