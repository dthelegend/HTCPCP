#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

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

HtcpcpRequestObject * decodeRequest(int message)
{
    char * request_uri_version = calloc(2048, sizeof(char));
    HtcpcpRequestObject * decoded = calloc(sizeof(HtcpcpRequestObject), 1);
    char * buff = calloc(8192, sizeof(char));
    int buff_head, internal_buff_head;

    decoded->method = calloc(sizeof(char), 9);
    decoded->uri = calloc(sizeof(char), 2048);
    decoded->headers = calloc(64, sizeof(HtcpcpHeader));
    decoded->request = calloc(8192, sizeof(char));

    buff[read(message, buff, 8191) + 1] = '\0';

    for(buff_head = 0; buff[buff_head] != '\n'; ++buff_head) request_uri_version[buff_head] = buff[buff_head];
    sscanf(request_uri_version, "%8s %2047s HTCPCP/%f", decoded->method, decoded->uri, &decoded->version);
    free(request_uri_version);

    for(decoded->header_count = 0; decoded->header_count < 64; ++decoded->header_count)
    {
        HtcpcpHeader new_head;
        if(buff[++buff_head] == '\n') break;
        new_head.key = calloc(2048, sizeof(char));
        new_head.value = calloc(2048, sizeof(char));
        for(internal_buff_head = 0; buff[buff_head + internal_buff_head] != ':'; ++internal_buff_head)
        {
            new_head.key[internal_buff_head] = tolower(buff[buff_head + internal_buff_head]);
        }
        buff_head += internal_buff_head;
        while(buff[++buff_head] == ' ');
        for(internal_buff_head = 0; buff[buff_head + internal_buff_head] != '\n'; ++internal_buff_head) new_head.value[internal_buff_head] = tolower(buff[buff_head + internal_buff_head]);
        decoded->headers[decoded->header_count] = new_head;
        buff_head += internal_buff_head;
    }

    ++buff_head;

    for(internal_buff_head = 0; buff[buff_head + internal_buff_head]; ++internal_buff_head) decoded->request[internal_buff_head] = buff[buff_head + internal_buff_head];
    decoded->request[8191] = '\0';

    free(buff);

    return decoded;
}

HtcpcpResponseObject * decodeResponse(int message)
{
    char * version_status_mess = calloc(2048, sizeof(char));
    HtcpcpResponseObject * decoded = calloc(sizeof(HtcpcpRequestObject), 1);
    char * buff = calloc(8192, sizeof(char));
    int buff_head, internal_buff_head;

    decoded->status_message = calloc(2048, sizeof(char));
    decoded->response = calloc(8192, sizeof(char));
    decoded->headers = calloc(64, sizeof(HtcpcpHeader));

    buff[read(message, buff, 8191) + 1] = '\0';

    for(buff_head = 0; buff[buff_head] != '\n'; ++buff_head) version_status_mess[buff_head] = buff[buff_head];
    sscanf(version_status_mess, "HTCPCP/%f %u %s", &decoded->version, &decoded->status, decoded->status_message);
    free(version_status_mess);

    for(decoded->header_count = 0; decoded->header_count < 64; ++decoded->header_count)
    {
        HtcpcpHeader new_head;
        if(buff[++buff_head] == '\n') break;
        new_head.key = calloc(2048, sizeof(char));
        new_head.value = calloc(2048, sizeof(char));
        for(internal_buff_head = 0; buff[buff_head + internal_buff_head] != ':'; ++internal_buff_head)
        {
            new_head.key[internal_buff_head] = tolower(buff[buff_head + internal_buff_head]);
        }
        buff_head += internal_buff_head;
        while(buff[++buff_head] == ' ');
        for(internal_buff_head = 0; buff[buff_head + internal_buff_head] != '\n'; ++internal_buff_head) new_head.value[internal_buff_head] = tolower(buff[buff_head + internal_buff_head]);
        decoded->headers[decoded->header_count] = new_head;
        buff_head += internal_buff_head;
    }

    ++buff_head;

    for(internal_buff_head = 0; buff[buff_head + internal_buff_head]; ++internal_buff_head) decoded->response[internal_buff_head] = buff[buff_head + internal_buff_head];
    decoded->response[8191] = '\0';

    free(buff);

    return decoded;
}

void encodeResponse(int message, HtcpcpResponseObject * response)
{
    int i;
    char * out = calloc(8192, sizeof(char));
    sprintf(out + strlen(out), "HTCPCP/%0.1f %u %s\n", response->version, response->status, response->status_message);
    for(i = 0; i < response->header_count; ++i) sprintf(out + strlen(out), "%s: %s\n", response->headers[i].key, response->headers[i].value);
    sprintf(out + strlen(out), "\n%s", response->response);
    write(message, out, strlen(out));
    free(out);
    return;
}

void encodeRequest(int message, HtcpcpRequestObject * request)
{
    int i;
    char * out = calloc(8192, sizeof(char));
    sprintf(out, "%s %s HTCPCP/%0.1f\n", request->method, request->uri, request->version);
    for(i = 0; i < request->header_count; ++i) sprintf(out + strlen(out), "%s: %s\n", request->headers[i].key, request->headers[i].value);
    sprintf(out + strlen(out), "\n%s", request->request);
    write(message, out, strlen(out));
    free(out);
    return;
}