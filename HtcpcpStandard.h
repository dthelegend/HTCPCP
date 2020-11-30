#ifndef _HTCPCP_STANDARD_H
#define _HTCPCP_STANDARD_H

#include <stdlib.h>

typedef struct
{
    char * key;
    char * value;
} HtcpcpHeader;
/* A HTCPCP Response Object */

typedef struct
{
    float version;
    unsigned int status;
    char * status_message;
    char * response;
    HtcpcpHeader * headers;
    size_t header_count;
} HtcpcpResponseObject;
/* A HTCPCP Response Object */

typedef struct
{
    float version;
    char * method;
    char * uri;
    char * request;
    HtcpcpHeader * headers;
    size_t header_count;
} HtcpcpRequestObject;
/* A HTCPCP Request Object */

HtcpcpRequestObject * decodeRequest(int);
/* Decodes and incoming HTCPCP request */

void encodeRequest(int, HtcpcpRequestObject *);
/* Encodes an outgoinging HTCPCP response */

void freeRequest(HtcpcpRequestObject *);
/* Frees a request object */

HtcpcpResponseObject * decodeResponse(int);
/*  */

void encodeResponse(int, HtcpcpResponseObject *);
/* Encodes an outgoinging HTCPCP response */

void freeResponse(HtcpcpResponseObject *);
/* Frees a request object */

#endif