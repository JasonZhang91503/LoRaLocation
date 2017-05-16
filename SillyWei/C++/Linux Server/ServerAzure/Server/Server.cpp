#include "Server.h"
#include "Socket.h"
#include "Cellphone.h"
#include "Gateway.h"

sql::Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res;

mutex sqlMutex;
mutex gatewayMutex;

void initializeMySQL() {
    // create a MySQL connection
    driver = get_driver_instance();
    con = driver->connect(HOSTNAME, USERNAME, PASSWORD);
    // connect to the MySQL postman database
    con->setSchema(SCHEMA);
    stmt = con->createStatement();
}

void handleNewConnection(int intSocketClient) {
    cout << "Function handleNewConnection start" << endl;

    int intErrorCode;
    int intRecognizeNumber;
    char charBuff;
    char* charPtr;

    // get identifier
    intErrorCode = recv(intSocketClient, &charBuff, sizeof(char), 0);
    if(intErrorCode == -1 || intErrorCode == 0) {
        cout << "HandleNewConnection socket: " << intSocketClient << ", connection terminate" << endl;
        close(intSocketClient);
        return;
    }

    // choice a device
    intRecognizeNumber = (int)strtol(&charBuff, &charPtr, 10);
    switch (intRecognizeNumber) {
    case 1:
        new thread(handleCellphoneFront, intSocketClient);
        break;
    case 2:
        new thread(handleGatewayFront, intSocketClient);
        break;
    case 3:
        new thread(handleCellphoneBack, intSocketClient);
        break;
    default:
        cout << "Device error" << endl;
        close(intSocketClient);
        break;
    }
}

void cleanCharArrayBuffer(char charArray[], int intSize) {
    for (int i = 0; i < intSize; i++) {
        charArray[i] = '\0';
    }
}

string makeKey(int intSize) {
    string strTemp;
    for (int i = 0; i < intSize; i++) {
        strTemp.append(to_string(rand() % 10));
    }
    return strTemp;
}
