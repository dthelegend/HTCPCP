#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "HtcpcpStandard.h"

int main(int argc, char const *argv[])
{
    int fd;
    HtcpcpRequestObject * req;
    HtcpcpResponseObject * res;

    printf("------------------------------------------------------------\n");

    fd = open("htcpcp_request.txt", O_RDONLY);
    req = decodeRequest(fd);
    encodeRequest(STDOUT_FILENO, req);
    freeRequest(req);
    close(fd);

    printf("\n------------------------------------------------------------\n");

    fd = open("htcpcp_response.txt", O_RDONLY);
    res = decodeResponse(fd);
    encodeResponse(STDOUT_FILENO, res);
    freeResponse(res);
    close(fd);

    printf("\n------------------------------------------------------------\n");

    return 0;
}