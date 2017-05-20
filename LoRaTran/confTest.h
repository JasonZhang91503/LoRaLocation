#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_PATH "Request.conf"
#define SIZE         256

/*config vars*/


int request_receivePeriod;

bool readRequestConfig(UserRequest* req){
    PacketManager *PacManager = PacketManager::getInstance(request_receivePeriod);

    char name[SIZE];
    char value[SIZE];

    FILE *fp = fopen(CONFIG_PATH, "r");
    if (fp == NULL) { return false; }
    while(!feof(fp)){
        memset(name,0,SIZE); memset(value,0,SIZE);

        /*Read Data*/
        fscanf(fp,"%s = %s\n", name, value);

        if (!strcmp(name, "packetNum")){
            req->packetNum = atoi(value);

        }else if (!strcmp(name, "senderID")){
            req->senderID = atoi(value);

        }else if (!strcmp(name, "receiverID")){
            req->receiverID = atoi(value);
        }
        else if (!strcmp(name, "packetKey")){
            req->packetKey.assign(value);
        }
        else if (!strcmp(name, "sendTime")){
            req->sendTime.assign(value);
        }
        else if (!strcmp(name, "sLon")){
            req->src_lon = atof(value);
        }
        else if (!strcmp(name, "sLat")){
            req->src_lat = atof(value);
        }
        else if (!strcmp(name, "dLon")){
            req->dest_lon = atof(value);
        }
        else if (!strcmp(name, "dLat")){
            req->dest_lat = atof(value);
        }
        req->state = 0;
    }
    fclose(fp);

    return true;
}