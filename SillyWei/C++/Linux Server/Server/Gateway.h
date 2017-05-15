#ifndef _GATEWAY_H
#define _GATEWAY_H

#include "Server.h"

class CarInfo{
public:
    int intId;
    int intPackageNum;
    int intMessageSize;
    char charArrayBuff[BUFFSIZE];
    char charArrayMessage[BUFFSIZE];

    void changePackageNum() {
        if (intPackageNum == 1) {
            intPackageNum = 2;
        }else if (intPackageNum == 2) {
            intPackageNum = 1;
        }
    }
};

void handleGatewayFront(int intSocketClient);
void stateUpdate(int id, int state);

void handleGatewayBack();
void getNewTransport(vector<string> &message);
void gatewayBackgroundSend(string message);
void gatewaySleep();
// Find carInfo with id, if exist return the index, else return -1
int findCarInfo(int intId, vector<CarInfo> vectorCarInfo);
int findSocket(int intSocketClient);

#endif
