#include "client.h"

void __send(Client *my_client)
{
    unsigned int size = sizeof(my_client->_serv_info);
    sendto(my_client->_socket,
           (char *) &my_client->_send_message,
           sizeof (uint32_t),
           0,
           (const struct sockaddr *) &my_client->_serv_info,
           size);
}

bool __recieve(Client *my_client)
{
    unsigned int size = sizeof(my_client->_serv_info);
    int len_message = recvfrom(my_client->_socket,
                               (char *) &my_client->_recv_message,
                               sizeof (uint32_t),
                               MSG_DONTWAIT,
                               (struct sockaddr *) &my_client->_serv_info,
                               &size);

    if (len_message < 0)
        return false;

    return true;
}

bool _first_connection(Client *my_client)
{
    uint16_t first_message = 1;

    _set_new_send_msg(my_client, first_message);

    return _send_message(my_client);
}

bool _init_server(Client *my_client)
{
    my_client->_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (!my_client->_socket)
        return false;

    memset(&my_client->_serv_info, 0, sizeof(my_client->_serv_info));

    my_client->_serv_info.sin_family = AF_INET;
    my_client->_serv_info.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    my_client->_serv_info.sin_port = htons(PORT);

    return true;
}

void _set_new_send_msg(Client *my_client, uint32_t new_message)
{
    my_client->_send_message = new_message | (KEY << 16);
}

bool _send_message(Client *my_client)
{
    __send(my_client);

    for (size_t i = 0; i < 100; i++) {
        if (__recieve_check_msg(my_client) && my_client->_check_message == KEY) {
            return true;
        }
        usleep(10000);
    }
    return false;
}

bool __recieve_check_msg(Client *my_client)
{
    unsigned int size = sizeof(my_client->_serv_info);
    int len_message = recvfrom(my_client->_socket,
                               (char *) &my_client->_check_message,
                               sizeof (uint16_t),
                               MSG_DONTWAIT,
                               (struct sockaddr *) &my_client->_serv_info,
                               &size);

    if (len_message <= 0)
        return false;

    return true;
}

bool _recieve_message(Client *my_client)
{
//    uint16_t key_from_recv = my_client->_recv_message >> 16;

    if (__recieve(my_client)/* && key_from_recv == KEY*/) {
        __send_check_msg(my_client);
        return true;
    }
    return false;
}

void __send_check_msg(Client *my_client)
{
    my_client->_check_message = KEY;

    unsigned int size = sizeof(my_client->_serv_info);
    sendto(my_client->_socket,
           (char *) &my_client->_check_message,
           sizeof (uint16_t),
           0,
           (const struct sockaddr *) &my_client->_serv_info,
           size);
}

void _finish(Client *my_client)
{
    close(my_client->_socket);
}
