#ifndef _CELLPHONE_H
#define _CELLPHONE_H

#include "Server.h"

// handle cellphone app
void handleCellphoneFront(int intSocketClient);
int createAccount(string strTemp);
bool login(string strTemp, int &intId, string &strAccount, string &strName, string &strMail);
bool createTransport(string strTemp);
void carAvailableTime(string strTemp, string &strSendBuff);
void askTransportSend(string strTemp, string &strSendBuff, int intId);
void askTransportRecv(string strTemp, string &strSendBuff, int intId);
void askTransportSendRecv(string strTemp, string &strSendBuff, int intId);
void getAccountName(string &strSendBuff, int intId);
void getLocationName(string &strSendBuff);
bool changeAccountData(string strTemp, int intId, string &strMail);

// handle cellphone service
void handleCellphoneBack(int intSocketClient);
bool cellphoneBackLogin(char* charPtr, int &intId);
bool getArriveTransport(string &strSendBuff, int intId);
void phoneSleep();

#endif
