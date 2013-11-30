#include <stdio.h>
#include <strings.h>
#include <netinet/in.h>

#include "protocol1.h"

#define PORT_NO     9999
#define BUFFER_SIZE 1024


int
main()
{
    int                 sd;
    struct sockaddr_in  addr;
    message_request_t   message_request;
    message_reply_t     message_reply;

    // Create client socket
    if( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("socket error");
        return -1;
    }

    bzero(&addr, sizeof(addr));

    addr.sin_family         = AF_INET;
    addr.sin_port           = htons(PORT_NO);
    addr.sin_addr.s_addr    =  htonl(INADDR_ANY);

    // Connect to server socket
    if(connect(sd, (struct sockaddr *)&addr, sizeof addr) < 0) {
        perror("Connect error");
        return -1;
    }

    message_request.value1 = 10;
    message_request.value2 = 20;

    send(sd, &message_request, sizeof(message_request), 0);

    // Receive message from the server
    recv(sd, &message_reply, sizeof(message_reply), 0);
    printf("reply: %d\n", message_reply.result);

    // while (strcmp(buffer, "q") != 0) {
    //     // Read input from user and send message to the server
    //     gets(buffer);
    //     send(sd, buffer, strlen(buffer), 0);

    //     // Receive message from the server
    //     recv(sd, buffer, BUFFER_SIZE, 0);
    //     printf("message: %s\n", buffer);
    // }

    return 0;
}
