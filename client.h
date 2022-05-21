#ifndef _CLIENT_H
#define _CLIENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "stdbool.h"

#define PORT 9898
#define KEY 0xF5E6

struct _Client {
    int _socket;
    uint32_t _send_message;
    uint32_t _recv_message;
    uint16_t _check_message;
    struct sockaddr_in _serv_info;
};

typedef struct _Client Client;

bool _send_message(Client *my_client);

bool _recieve_message(Client *my_client);

void __send(Client *my_client);

void __send_check_msg(Client *my_client);

bool __recieve_check_msg(Client *my_client);

bool __recieve(Client *my_client);

bool _first_connection(Client *my_client);

bool _init_server(Client *my_client);

void _set_new_send_msg(Client *my_client, uint32_t new_message);

void _finish(Client *my_client);

#ifdef __cplusplus
}
#endif

#endif // _CLIENT_H
