#include <stdio.h>
#include "HtcpcpStandard.h"

int main(int argc, char const *argv[])
{
    FILE * fp;
    HtcpcpRequestObject * req;
    HtcpcpResponseObject * res;

    printf("------------------------------------------------------------\n");

    fp = fopen("htcpcp_request.txt", "r");
    req = decodeRequest(fp);
    encodeRequest(stdout, req);
    freeRequest(req);
    fclose(fp);

    printf("\n------------------------------------------------------------\n");

    fp = fopen("htcpcp_response.txt", "r");
    res = decodeResponse(fp);
    encodeResponse(stdout, res);
    freeResponse(res);
    fclose(fp);

    printf("\n------------------------------------------------------------\n");

    return 0;
}