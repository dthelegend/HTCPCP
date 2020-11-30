#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "HtcpcpStandard.h"

int main(int argc, char * argv[]) {
    int SockFD;
    struct sockaddr_in sad;
    HtcpcpResponseObject * res;
    HtcpcpRequestObject * req = calloc(1, sizeof(HtcpcpRequestObject));

    sad.sin_family = AF_INET;
    sad.sin_port = htons(8000);
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");

    SockFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    req->method = "BREW";
    req->header_count = 4;
    req->headers = calloc(sizeof(HtcpcpHeader), req->header_count);
    req->headers[0].key = "addition-type";
    req->headers[0].value = "milk-type";
    req->headers[1].key = "host";
    req->headers[1].value = "127.0.0.1";
    req->headers[2].key = "User-Agent";
    req->headers[2].value = "HTCPCP-Client";
    req->headers[3].key = "accept";
    req->headers[3].value = "*/*";
    req->request = "Hello Coffee World!";
    req->uri = "/";
    req->version = 1.0;

    if(connect(SockFD, (struct sockaddr *) &sad, sizeof(sad)) < 0)
    {
        printf("Error: Failed to Connect Socket\n");
        return 1;
    }
    printf("Connected!\n");

    encodeRequest(SockFD, req);
    printf("Encoding Request Complete!\n");
    free(req->headers);
    free(req);

    res = decodeResponse(SockFD);
    printf("Decoding Response Complete!\n");
    encodeResponse(STDOUT_FILENO, res);
    printf("\n");
    
    freeResponse(res);
    close(SockFD);

    return 0;
}