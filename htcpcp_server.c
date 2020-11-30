#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "HtcpcpStandard.h"

int main(int argc, char * argv[]) {
    int SockFD, ultraSock;
    unsigned int alen;
    struct sockaddr_in sad, client;
    HtcpcpResponseObject res;
    HtcpcpRequestObject * req;

    sad.sin_family = AF_INET;
    sad.sin_port = htons(8000);
    sad.sin_addr.s_addr = INADDR_ANY;
    SockFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(bind(SockFD, (struct sockaddr *) &sad, sizeof(sad)) < 0) 
    {
        printf("Error: Failed to Bind Socket\n");
        return 1;
    }
    printf("Bound\n");

    listen(SockFD, 15);
    printf("Listening!\n\n");

    alen = sizeof(client);
    while((ultraSock = accept(SockFD, (struct sockaddr *) &client, &alen)))
    {
        printf("Connected to client %d.%d.%d.%d on port %d!\n", (client.sin_addr.s_addr >> 3) & 15, (client.sin_addr.s_addr >> 2) & 15, (client.sin_addr.s_addr >> 1) & 15, (client.sin_addr.s_addr) & 15, ntohs(client.sin_port));
        req = decodeRequest(ultraSock);
        encodeRequest(STDOUT_FILENO, req);
        printf("Decoding Request Complete!\n\n");

        res.headers = req->headers;
        res.header_count = req->header_count;
        res.response = req->request;
        res.status = 200;
        res.status_message = "OK";
        res.version = 1.0;

        encodeResponse(ultraSock, &res);
        encodeResponse(STDOUT_FILENO, &res);
        printf("\n");
    }

    if(ultraSock < 0)
    {
        printf("Error: Failed to Accept\n");
        return 1;
    }

    return 0;
}