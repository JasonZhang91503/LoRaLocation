#include "Cellphone.h"
#include "Socket.h"

void handleCellphoneFront(int intSocketClient) {
    cout << "Function handleCellphoneFront start" << endl;

    int intErrorCode;
    int intRecognizeNumber;
    char charArrayRecvBuff[BUFFSIZE];
    char* charPtr;
    string strSendBuff;

    // user info
    bool boolLogin = false;
    int intId;
    string strAccount;
    string strName;
    string strMail;

    // handle receive tasks
    while (true) {
        // clean the buffer
        cleanCharArrayBuffer(charArrayRecvBuff, BUFFSIZE);

        // receive task
        intErrorCode = recv(intSocketClient, charArrayRecvBuff, BUFFSIZE, 0);
        if(intErrorCode == -1 || intErrorCode == 0) {
            cout << endl << "CellphoneFront socket: " << intSocketClient << ", connection terminate" << endl;
            close(intSocketClient);
            return;
        }
        // print the receive data
        cout << endl << "CellphoneFront Socket: " << intSocketClient << ", recv size: " << intErrorCode << endl
            << "receive data: " << charArrayRecvBuff << endl;
        // get the recognize code
        intRecognizeNumber = (int)strtol(charArrayRecvBuff, &charPtr, 10);

        // lock sqlMutex
        lock_guard<mutex> mLock(sqlMutex);

        if (intRecognizeNumber == 20) {
            // error message
            cout << "error info: " << &charArrayRecvBuff[3] << endl;
            close(intSocketClient);
            cout << "CellphoneFront socket: " << intSocketClient << ", connection terminate" << endl;
            return;
        }else if(!boolLogin) {
            // not yet login
            switch (intRecognizeNumber) {
            case 2:
                // create account
                cout << "Task create account" << endl;
                strSendBuff = "2, \n";
                switch (createAccount(&charArrayRecvBuff[2])) {
                case 1:
                    cout << "Create success" << endl;
                    strSendBuff[2] = '1';
                    break;
                case 2:
                    cout << "Create fail, duplicated account" << endl;
                    strSendBuff[2] = '2';
                    break;
                case 3:
                    cout << "Create fail, duplicated mail" << endl;
                    strSendBuff[2] = '3';
                    break;
                case 4:
                    cout << "Create fail, duplicated account and mail" << endl;
                    strSendBuff[2] = '4';
                    break;
                case 5:
                    cout << "Create fail, known reason" << endl;
                    strSendBuff[2] = '5';
                    break;
                }
                break;
            case 3:
                // log in
                cout << "Task login" << endl;
                strSendBuff = "3, ";
                boolLogin = login(&charArrayRecvBuff[2], intId, strAccount, strName, strMail);
                if (boolLogin) {
                    cout << "Account: " << strAccount << ", login success, id = " << intId << endl;
                    strSendBuff[2] = '1';
                    strSendBuff.append(strName + "^" + strMail + "\n");
                }else {
                    cout << "Account: " << strAccount << ", login fail, id = " << intId << endl;
                    strSendBuff[2] = '0';
                    strSendBuff.append("\n");
                }
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
                strSendBuff = "20,You need to login first\n";
                break;
            default:
                strSendBuff = "20,Wrong state\n";
                break;
            }
        }else {
            // already login
            cout << "Account: " << strAccount << endl;
            switch (intRecognizeNumber) {

            /* test */
            case 2:
                break;
            case 3:
            // log in
                cout << "Task login" << endl;
                strSendBuff = "3, ";

                if (login(&charArrayRecvBuff[2], intId, strAccount, strName, strMail)) {
                    cout << "Account: " << strAccount << ", login success, id = " << intId << endl;
                    strSendBuff[2] = '1';
                    strSendBuff.append(strName + "^" + strMail + "\n");
                }else {
                    cout << "Account: " << strAccount << ", login fail, id = " << intId << endl;
                    strSendBuff[2] = '0';
                    strSendBuff.append("\n");
                }
                break;
            /* test */
            /*
            case 2:
            case 3:
                strSendBuff = "20,You already login\n";
                break;
            */
            case 4:
                // create transport
                cout << "Task create transport" << endl;
                strSendBuff = "4, \n";
                if (createTransport(&charArrayRecvBuff[2])) {
                    cout << "Create transport success" << endl;
                    strSendBuff[2] = '1';
                }else {
                    cout << "Create transport fail" << endl;
                    strSendBuff[2] = '0';
                }
                break;
            case 5:
                // ask car available time
                cout << "Task ask for car available time" << endl;
                strSendBuff = "5,";
                carAvailableTime(&charArrayRecvBuff[2], strSendBuff);
                strSendBuff.append("\n");
                break;
            case 6:
                // ask for user data
                cout << "Task ask for user data" << endl;
                strSendBuff = "6," + strAccount + "^" + strMail + "\n";
                break;
            case 7:
                // ask for transport send data
                cout << "Task ask for transport send data" << endl;
                strSendBuff = "7,";
                askTransportSend(&charArrayRecvBuff[2],strSendBuff, intId);
                strSendBuff.append("\n");
                break;
            case 8:
                // ask for transport receive data
                cout << "Task ask for transport receive data" << endl;
                strSendBuff = "8,";
                askTransportRecv(&charArrayRecvBuff[2],strSendBuff, intId);
                strSendBuff.append("\n");
                break;
            case 9:
                // ask for transport data
                cout << "Task ask for transport data" << endl;
                strSendBuff = "9,";
                askTransportSendRecv(&charArrayRecvBuff[2],strSendBuff, intId);
                strSendBuff.append("\n");
                break;
            case 10:
                // ask all user
                cout << "Task ask for all user" << endl;
                strSendBuff = "10,";
                getAccountName(strSendBuff, intId);
                strSendBuff.append("\n");
                break;
            case 11:
                // ask all location name
                cout << "Task ask for location name" << endl;
                strSendBuff = "11,";
                getLocationName(strSendBuff);
                strSendBuff.append("\n");
                break;
            case 12:
                // update user data
                cout << "Task for update user data" << endl;
                strSendBuff = "12, \n";
                if (changeAccountData(&charArrayRecvBuff[3], intId, strMail)) {
                    cout << "Update success" << endl;
                    strSendBuff[3] = '1';
                }else {
                    cout << "Update fail" << endl;
                    strSendBuff[3] = '0';
                }
                break;
            default:
                strSendBuff = "20,Wrong state\n";
                break;
            }
        }
        // sendback to task
        intErrorCode = send(intSocketClient, &strSendBuff[0], strSendBuff.size(), 0);
        if(intErrorCode == -1 || intErrorCode == 0) {
            cout << "CellphoneFront socket: " << intSocketClient << ", connection terminate" << endl;
            close(intSocketClient);
            return;
        }
        cout << "CellphoneFront Socket: " << intSocketClient << ", send size: " << intErrorCode << endl
            << "send data: " << strSendBuff << endl;
    }
}

int createAccount(string strTemp) {
    string strAccount;
    string strPassword;
    string strName;
    string strMail;

    int intTemp1;
    int intTemp2;
    bool boolAccount = true;
    bool boolMail = true;

    // get account
    intTemp1 = strTemp.find(",");
    strAccount = strTemp.substr(0, intTemp1);

    // get password
    intTemp2 = strTemp.find(",", intTemp1 + 1);
    strPassword = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);

    // get name
    intTemp1 = strTemp.find(",", intTemp2 + 1);
    strName = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 - 1);

    // get email
    intTemp2 = strTemp.find(",", intTemp1 + 1);
    strMail = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);

    // check account
    res = stmt->executeQuery("SELECT * FROM user WHERE account='" + strAccount + "'");
    if (res->next()) {
        boolAccount = false;
    }
    delete res;

    // check mail
    res = stmt->executeQuery("SELECT * FROM user WHERE mail='" + strMail + "'");
    if (res->next()) {
        boolMail = false;
    }
    delete res;

    if (boolAccount) {
        if (boolMail) {
            if (stmt->executeUpdate("INSERT INTO user(account,password,name,mail)VALUES('" +
                strAccount + "','" + strPassword + "','" + strName + "','" + strMail + "')")) {
                return 1;
            }
            return 5;
        }else {
            return 3;
        }
    }else {
        if (boolMail) {
            return 2;
        }else {
            return 4;
        }
    }
}

bool login(string strTemp, int &intId, string &strAccount, string &strName, string &strMail) {
    string strPassword;

    int intTemp1;
    int intTemp2;

    // get account
    intTemp1 = strTemp.find(",");
    strAccount = strTemp.substr(0, intTemp1);

    // get password
    intTemp2 = strTemp.find(",", intTemp1 + 1);
    strPassword = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);

    // check account
    res = stmt->executeQuery("SELECT _id,name,mail FROM user WHERE account='" + strAccount + "'AND password='" + strPassword + "'");
    if (res->next()) {
        intId = res->getInt("_id");
        strName = res->getString("name");
        strMail = res->getString("mail");
        delete res;
        return true;
    }
    delete res;
    return false;
}

bool createTransport(string strTemp) {
    string strSender;
    string strReceiver;
    string strRequireTime;
    string strStartLocation;
    string strDestLocation;
    string strRemark;

    int intTemp1;
    int intTemp2;

    // get sender
    intTemp1 = strTemp.find(",");
    strSender = strTemp.substr(0, intTemp1);
    res = stmt->executeQuery("SELECT _id FROM user WHERE account='" + strSender + "'");
    if (res->next()) {
        strSender = res->getString ("_id");
        delete res;
    }else {
        delete res;
        return false;
    }
    cout << strSender << endl;

    // get receiver
    intTemp2 = strTemp.find(",", intTemp1 + 1);
    strReceiver = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);
    res = stmt->executeQuery("SELECT _id FROM user WHERE account='" + strReceiver + "'");
    if (res->next()) {
        strReceiver = res->getString ("_id");
        delete res;
    }else {
        delete res;
        return false;
    }
    cout << strReceiver << endl;

    // get requireTime
    intTemp1 = strTemp.find(",", intTemp2 + 1);
    strRequireTime = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 - 1);
    cout << strRequireTime << endl;

    // get startLocation
    intTemp2 = strTemp.find(",", intTemp1 + 1);
    strStartLocation = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);
    res = stmt->executeQuery("SELECT _id FROM location WHERE name='" + strStartLocation + "'");
    if (res->next()) {
        strStartLocation = res->getString ("_id");
        delete res;
    }else {
        delete res;
        return false;
    }
    cout << strStartLocation << endl;

    // get destLocation
    intTemp1 = strTemp.find(",", intTemp2 + 1);
    strDestLocation = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 - 1);
    res = stmt->executeQuery("SELECT _id FROM location WHERE name='" + strDestLocation + "'");
    if (res->next()) {
        strDestLocation = res->getString ("_id");
        delete res;
    }else {
        delete res;
        return false;
    }
    cout << strDestLocation << endl;

    // get remark
    intTemp2 = strTemp.find(",", intTemp1 + 1);
    strRemark = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);
    cout << strRemark << endl;

    cout << "INSERT INTO transport VALUE(default,'" + strRequireTime + "','" +
        strRequireTime + "'+interval 15 MINUTE," + strSender +"," + strReceiver + "," +
        strStartLocation + "," + strDestLocation + ",1,0,'" + makeKey(4) + "','" + strRemark +"',true)" << endl;
    // create a transport
    if (stmt->executeUpdate("INSERT INTO transport VALUE(default,'" + strRequireTime + "','" +
        strRequireTime + "'+interval 15 MINUTE," + strSender +"," + strReceiver + "," +
        strStartLocation + "," + strDestLocation + ",1,0,'" + makeKey(4) + "','" + strRemark +"',true)")) {
        cout << "here" << endl;
        return true;
    }
    return false;
}

void carAvailableTime(string strTemp, string &strSendBuff) {
    string strDate;
    string strTransportTime;
    int intTemp;
    int intHour;
    int intMinute;
    int intIndex;
    char* charPtr;

    // get date
    intTemp = strTemp.find(",");
    strDate = strTemp.substr(0, intTemp);

    // get the transports in that date
    res = stmt->executeQuery("SELECT requireTime FROM transport WHERE requireTime>='" + strDate +
        "'AND requireTime<ADDDATE('" + strDate + "',1)ORDER BY requireTime");

    // add available time to strSendBuff
    intTemp = strSendBuff.size();
    strSendBuff.append("111111111111111111");

    while (res->next()) {
        strTransportTime = res->getString("requireTime");
        strTransportTime = strTransportTime.substr(11, 5);
        intHour = strtol(&strTransportTime.substr(0, 2)[0], &charPtr,10);
        intMinute = strtol(&strTransportTime.substr(3, 2)[0], &charPtr, 10);
        intIndex = intTemp + intMinute / 30 + (intHour - 9) * 2;
        strSendBuff[intIndex] = '0';
    }
    delete res;
}

void askTransportSend(string strTemp, string &strSendBuff, int intId) {
    string strData;
    int intTemp1;
    int intTemp2;
    int intIndex;

    // get transport send
    intTemp1 = strTemp.find(",");
    strData = strTemp.substr(0, intTemp1);
    res = stmt->executeQuery("SELECT * FROM transport WHERE sender=" + to_string(intId) + " AND requireTime>='" +
        strData + "' AND requireTime<ADDDATE('" + strData + "',1)ORDER BY requireTime");
    intIndex = 0;
    strTemp = "";
    while (res->next()) {
        strTemp.append(res->getString("receiver") + "~");
        strTemp.append(res->getString("requireTime") + "^");
        strTemp.append(res->getString("arriveTime") + ";");
        strTemp.append(res->getString("start_id") + "/");
        strTemp.append(res->getString("des_id") + "!");
        strTemp.append(res->getString("state") + "#");
        strTemp.append(res->getString("packetKey") + "$");
        strTemp.append(res->getString("remark") + "*");
        intIndex++;
    }
    delete res;
    strSendBuff.append(to_string(intIndex) + ",");

    intTemp2 = -1;
    for (int i = 0; i < intIndex; i++) {
        // get receiver
        intTemp1 = strTemp.find("~", intTemp2 + 1);
        strData = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 -1);
        res = stmt->executeQuery("SELECT name FROM user WHERE _id=" + strData);
        res->next();
        strSendBuff.append(res->getString("name") + "~");
        delete res;

        // get requireTime
        intTemp2 = strTemp.find("^", intTemp1 + 1);
        strSendBuff.append(strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1));
        // get arriveTime
        intTemp1 = strTemp.find(";", intTemp2 + 1);
        strSendBuff.append(strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2));
        // get startLocation
        intTemp2 = strTemp.find("/", intTemp1 + 1);
        strData = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);
        res = stmt->executeQuery("SELECT name FROM location WHERE _id=" + strData);
        res->next();
        strSendBuff.append(res->getString("name") + "/");
        delete res;
        // get destLocation
        intTemp1 = strTemp.find("!", intTemp2 + 1);
        strData = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 - 1);
        res = stmt->executeQuery("SELECT name FROM location WHERE _id=" + strData);
        res->next();
        strSendBuff.append(res->getString("name") + "!");
        delete res;
        // get state and key and remark
        intTemp2 = strTemp.find("*", intTemp1 + 1);
        strSendBuff.append(strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1));
    }
}

void askTransportRecv(string strTemp, string &strSendBuff, int intId) {
    string strData;
    int intTemp1;
    int intTemp2;
    int intIndex;

    // get transport send
    intTemp1 = strTemp.find(",");
    strData = strTemp.substr(0, intTemp1);
    res = stmt->executeQuery("SELECT * FROM transport WHERE receiver=" + to_string(intId) + " AND requireTime>='" +
        strData + "' AND requireTime<ADDDATE('" + strData + "',1)ORDER BY requireTime");
    intIndex = 0;
    strTemp = "";
    while (res->next()) {
        strTemp.append(res->getString("sender") + "~");
        strTemp.append(res->getString("requireTime") + "^");
        strTemp.append(res->getString("arriveTime") + ";");
        strTemp.append(res->getString("start_id") + "/");
        strTemp.append(res->getString("des_id") + "!");
        strTemp.append(res->getString("state") + "#");
        strTemp.append(res->getString("packetKey") + "$");
        strTemp.append(res->getString("remark") + "*");
        intIndex++;
    }
    delete res;
    strSendBuff.append(to_string(intIndex) + ",");

    intTemp2 = -1;
    for (int i = 0; i < intIndex; i++) {
        // get sender
        intTemp1 = strTemp.find("~", intTemp2 + 1);
        strData = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 -1);
        res = stmt->executeQuery("SELECT name FROM user WHERE _id=" + strData);
        res->next();
        strSendBuff.append(res->getString("name") + "~");
        delete res;
        // get requireTime
        intTemp2 = strTemp.find("^", intTemp1 + 1);
        strSendBuff.append(strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1));
        // get arriveTime
        intTemp1 = strTemp.find(";", intTemp2 + 1);
        strSendBuff.append(strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2));
        // get startLocation
        intTemp2 = strTemp.find("/", intTemp1 + 1);
        strData = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);
        res = stmt->executeQuery("SELECT name FROM location WHERE _id=" + strData);
        res->next();
        strSendBuff.append(res->getString("name") + "/");
        delete res;
        // get destLocation
        intTemp1 = strTemp.find("!", intTemp2 + 1);
        strData = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 - 1);
        res = stmt->executeQuery("SELECT name FROM location WHERE _id=" + strData);
        res->next();
        strSendBuff.append(res->getString("name") + "!");
        delete res;
        // get state and key and remark
        intTemp2 = strTemp.find("*", intTemp1 + 1);
        strSendBuff.append(strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1));
    }
}

void askTransportSendRecv(string strTemp, string &strSendBuff, int intId) {
    string strData;
    int intTemp1;
    int intTemp2;
    int intIndex;

    // get transport send
    intTemp1 = strTemp.find(",");
    strData = strTemp.substr(0, intTemp1);
    res = stmt->executeQuery("SELECT * FROM transport WHERE (sender=" + to_string(intId) +" OR receiver=" + to_string(intId) + ") AND requireTime>='" +
        strData + "' AND requireTime<ADDDATE('" + strData + "',1)ORDER BY requireTime");
    intIndex = 0;
    strTemp = "";
    while (res->next()) {
        strTemp.append(res->getString("sender") + "~");
        strTemp.append(res->getString("receiver") + "^");
        strTemp.append(res->getString("requireTime") + ";");
        strTemp.append(res->getString("arriveTime") + "/");
        strTemp.append(res->getString("start_id") + "!");
        strTemp.append(res->getString("des_id") + "#");
        strTemp.append(res->getString("state") + "$");
        strTemp.append(res->getString("packetKey") + "%");
        strTemp.append(res->getString("remark") + "*");
        intIndex++;
    }
    delete res;
    strSendBuff.append(to_string(intIndex) + ",");

    intTemp1 = -1;
    for (int i = 0; i < intIndex; i++) {
        // get sender
        intTemp2 = strTemp.find("~", intTemp1 + 1);
        strData = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 -1);
        res = stmt->executeQuery("SELECT name FROM user WHERE _id=" + strData);
        res->next();
        strSendBuff.append(res->getString("name") + "~");
        delete res;
        // get receiver
        intTemp1 = strTemp.find("^", intTemp2 + 1);
        strData = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 -1);
        res = stmt->executeQuery("SELECT name FROM user WHERE _id=" + strData);
        res->next();
        strSendBuff.append(res->getString("name") + "^");
        delete res;
        // get requireTime
        intTemp2 = strTemp.find(";", intTemp1 + 1);
        strSendBuff.append(strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1));
        // get arriveTime
        intTemp1 = strTemp.find("/", intTemp2 + 1);
        strSendBuff.append(strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2));
        // get startLocation
        intTemp2 = strTemp.find("!", intTemp1 + 1);
        strData = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);
        res = stmt->executeQuery("SELECT name FROM location WHERE _id=" + strData);
        res->next();
        strSendBuff.append(res->getString("name") + "!");
        delete res;
        // get destLocation
        intTemp1 = strTemp.find("#", intTemp2 + 1);
        strData = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 - 1);
        res = stmt->executeQuery("SELECT name FROM location WHERE _id=" + strData);
        res->next();
        strSendBuff.append(res->getString("name") + "#");
        delete res;
        // get state and key and remark
        intTemp2 = strTemp.find("*", intTemp1 + 1);
        strSendBuff.append(strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1));
        intTemp1 = intTemp2;
    }
}

void getAccountName(string &strSendBuff, int intId) {
    int intIndex = 0;
    string strTemp;

    res = stmt->executeQuery("SELECT * FROM user WHERE _id!=" + to_string(intId));
    while (res->next()) {
        strTemp.append(res->getString("account") + "~");
        strTemp.append(res->getString("name") + "*");
        intIndex++;
    }
    strSendBuff.append(to_string(intIndex) + "^" + strTemp);
}

void getLocationName(string &strSendBuff) {
    int intIndex = 0;
    string strTemp;

    res = stmt->executeQuery("SELECT * FROM location");
    while (res->next()) {
        strTemp.append(res->getString("name") + "*");
        intIndex++;
    }
    strSendBuff.append(to_string(intIndex) + "^" + strTemp);
}

bool changeAccountData(string strTemp, int intId, string &strMail) {
    string strPassword;

    int intTemp1;
    int intTemp2;
    intTemp1 = strTemp.find(",");
    intTemp2 = strTemp.find(",", intTemp1 + 1);
    strPassword = strTemp.substr(0, intTemp1);
    strMail = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);

    res = stmt->executeQuery("SELECT * FROM user WHERE _id!=" + to_string(intId) + " AND mail='" + strMail + "'");
    if (res->next()) {
        delete res;
        return false;
    }
    delete res;
    stmt->executeUpdate("UPDATE user SET password='" + strPassword + "',mail='" + strMail +"'WHERE _id=" + to_string(intId));

    return true;
}

void handleCellphoneBack(int intSocketClient) {
    cout << "Function handleCellphoneBack start" << endl;

    int intErrorCode;
    char charArrayBuff[BUFFSIZE];
    string strSendBuff;

    // user info
    int intId;
    string strTransportId;
    string strAccount;

    intErrorCode = recv(intSocketClient, charArrayBuff, BUFFSIZE, 0);
    if(intErrorCode == -1 || intErrorCode == 0) {
        cout << endl << "CellphoneBack socket: " << intSocketClient << ", connection terminate" << endl;
        close(intSocketClient);
        return;
    }

    // login
    if (cellphoneBackLogin(&charArrayBuff[0], intId)) {
        cout << "CellphoneBack socket: " << intSocketClient << ", login success" << endl;
        strSendBuff = "1,1\n";
        send(intSocketClient, &strSendBuff[0], strSendBuff.size(), 0);
    }else {
        cout << endl << "CellphoneBack socket: " << intSocketClient << ", connection terminate" << endl;
        strSendBuff = "1,0\n";
        send(intSocketClient, &strSendBuff[0], strSendBuff.size(), 0);
        close(intSocketClient);
        return;
    }

    //
    while (true) {

        phoneSleep();
        lock_guard<mutex> mLock(sqlMutex);

        strSendBuff = "2, \n";
        if (getArriveTransport(strSendBuff, intId, strTransportId)) {
            send(intSocketClient, &strSendBuff[0], strSendBuff.size(), 0);
            cout << endl << "CellphoneBack socket: " << intSocketClient << ", have a transport" << endl <<
                "data: " + strSendBuff + ", wait for the next search" << endl;
            if (intErrorCode == -1 || intErrorCode == 0) {
                cout << endl << "CellphoneBack socket: " << intSocketClient << ", connection terminate" << endl;
                close(intSocketClient);
                return;
            }
            recv(intSocketClient, charArrayBuff, BUFFSIZE, 0);
            if (charArrayBuff[0] == '2') {
                updateArriveTransport(strTransportId);
            }
        }else {
            cout << endl << "CellphoneBack socket: " << intSocketClient << ", have no transport wait for the next search" << endl;
        }
    }
}

bool cellphoneBackLogin(char* charPtr, int &intId) {
    string strTemp;
    lock_guard<mutex> mLock(sqlMutex);
    return login(charPtr, intId, strTemp,strTemp, strTemp);
}

bool getArriveTransport(string &strSendBuff, int intId, string &strTransportId) {
    using std::chrono::system_clock;
    time_t tt = system_clock::to_time_t(system_clock::now());
    struct tm* ptm = localtime(&tt);
    string strId;

    if (ptm->tm_min >= 30) {
        ptm->tm_min = 30;
    }else {
        ptm->tm_min = 0;
    }
    ptm->tm_sec = 0;
    char charArrayBuff[80];
    strftime(charArrayBuff, sizeof(charArrayBuff), "%Y-%m-%d %H:%M:%S", ptm);
    string strTime(charArrayBuff);

    // check send
    res = stmt->executeQuery("SELECT * FROM transport WHERE state=1 AND firstNotify=true AND sender=" + to_string(intId));
    if (res->next()) {
        strSendBuff[2] = '1';
        strTransportId = res->getString("_id");
        delete res;
        return true;
    }
    delete res;

    // check receive
    res = stmt->executeQuery("SELECT * FROM transport WHERE state=3 AND firstNotify=true AND receiver=" + to_string(intId));
    if (res->next()) {
        strSendBuff[2] = '0';
        strTransportId = res->getString("_id");
        delete res;
        return true;
    }
    delete res;
    return false;
}

void updateArriveTransport(string strTransportId) {
    stmt->executeUpdate("UPDATE transport SET firstNotify=false WHERE _id=" + strTransportId);
}

void phoneSleep() {
    this_thread::sleep_for(chrono::seconds(30));
}

