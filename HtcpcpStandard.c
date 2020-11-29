#include <stdio.h>
#include <ctype.h>

#include "HtcpcpStandard.h"

void freeRequest(HtcpcpRequestObject * request)
{
    int i;
    free(request->method);
    free(request->uri);
    for(i = 0; i < request->header_count; ++i) 
    {
        free((request->headers + i)->key);
        free((request->headers + i)->value);
    }
    free(request->headers);
    free(request->request);
    free(request);
    return;
}

void freeResponse(HtcpcpResponseObject * response)
{
    int i;
    free(response->status_message);
    for(i = 0; i < response->header_count; ++i) 
    {
        free((response->headers + i)->key);
        free((response->headers + i)->value);
    }
    free(response->headers);
    free(response->response);
    free(response);
    return;
}

HtcpcpRequestObject * decodeRequest(FILE * message)
{
    char * request_uri_version = calloc(2048, sizeof(char));
    HtcpcpRequestObject * decoded = NULL;

    decoded = calloc(sizeof(HtcpcpRequestObject), 1);
    decoded->method = calloc(sizeof(char), 9);
    decoded->uri = calloc(sizeof(char), 2048);

    fgets(request_uri_version, 2047, message);

    sscanf(request_uri_version, "%4s %2047s HTCPCP/%f", decoded->method, decoded->uri, &decoded->version);
    decoded->method[4] = '\0';
    decoded->uri[2047] = '\0';
    
    free(request_uri_version);

    decoded->headers = calloc(64, sizeof(HtcpcpHeader));
    for(decoded->header_count = 0; decoded->header_count < 64; ++decoded->header_count)
    {
        HtcpcpHeader new_head;
        char * key_value = calloc(2048, sizeof(char));
        int pos, pos1;

        fgets(key_value, 2047, message);
        if(key_value[0] == '\n'){
            free(key_value);
            break;
        }
        new_head.key = calloc(2048, sizeof(char));
        new_head.value = calloc(2048, sizeof(char));
        for(pos = 0; *(key_value + pos); ++pos)
        {
            if(*(key_value + pos) == ':') break;
            *(new_head.key + pos) = tolower(*(key_value + pos));
        }
        while(*(key_value + ++pos) == ' ');
        for(pos1 = 0; *(key_value + pos + pos1); ++pos1) *(new_head.value + pos1) = tolower(*(key_value + pos + pos1));
        *(decoded->headers + decoded->header_count) = new_head;
        free(key_value);
    }
    decoded->request = calloc(8192, sizeof(char));
    fread(decoded->request, 8191, sizeof(char), message);
    decoded->request[8191] = '\0';
    return decoded;
}

HtcpcpResponseObject * decodeResponse(FILE * message)
{
    char * request_uri_version = calloc(2048, sizeof(char));
    HtcpcpResponseObject * decoded = NULL;

    decoded = calloc(sizeof(HtcpcpResponseObject), 1);
    decoded->status_message = calloc(sizeof(char), 2048);

    fgets(request_uri_version, 2047, message);

    sscanf(request_uri_version, "HTCPCP/%f %u %s", &decoded->version, &decoded->status, decoded->status_message);
    decoded->status_message[2047] = '\0';
    
    free(request_uri_version);

    decoded->headers = calloc(64, sizeof(HtcpcpHeader));
    for(decoded->header_count = 0; decoded->header_count < 64; ++decoded->header_count)
    {
        HtcpcpHeader new_head;
        char * key_value = calloc(2048, sizeof(char));
        int pos, pos1;

        fgets(key_value, 2047, message);
        if(key_value[0] == '\n'){
            free(key_value);
            break;
        }
        new_head.key = calloc(2048, sizeof(char));
        new_head.value = calloc(2048, sizeof(char));
        for(pos = 0; *(key_value + pos); ++pos)
        {
            if(*(key_value + pos) == ':') break;
            *(new_head.key + pos) = tolower(*(key_value + pos));
        }
        while(*(key_value + ++pos) == ' ');
        for(pos1 = 0; *(key_value + pos + pos1); ++pos1) *(new_head.value + pos1) = tolower(*(key_value + pos + pos1));
        *(decoded->headers + decoded->header_count) = new_head;
        free(key_value);
    }
    decoded->response = calloc(8192, sizeof(char));
    fread(decoded->response, 8191, sizeof(char), message);
    decoded->response[8191] = '\0';
    return decoded;
}

void encodeResponse(FILE * message, HtcpcpResponseObject * response)
{
    int i;
    fprintf(message, "HTCPCP/%0.1f %d %s\n", response->version, response->status, response->status_message);
    for(i = 0; i < response->header_count; ++i) fprintf(message, "%s: %s", response->headers[i].key, response->headers[i].value);
    fprintf(message, "\n%s", response->response);
}

void encodeRequest(FILE * message, HtcpcpRequestObject * request)
{
    int i;
    fprintf(message, "%s %s HTCPCP/%0.1f\n", request->method, request->uri, request->version);
    for(i = 0; i < request->header_count; ++i) fprintf(message, "%s: %s", request->headers[i].key, request->headers[i].value);
    fprintf(message, "\n%s", request->request);
    return;
}