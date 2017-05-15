#include "Gateway.h"
#include "Server.h"
#include "Socket.h"

vector<int> vectorIntSocketGateway;
vector<CarInfo> vectorCarInfoSend;
vector<CarInfo> vectorCarInfoReceive;

void handleGatewayFront(int intSocketClient) {
    cout << "Function handleGatewayFront start" << endl;

    int intErrorCode;
	int intIndex;
    char charArrayBuff[BUFFSIZE] = { '\0' };

	vectorIntSocketGateway.push_back(intSocketClient);

	while (true) {
		// Receive data
        cleanCharArrayBuffer(charArrayBuff, BUFFSIZE);
		intErrorCode = recv(intSocketClient, charArrayBuff, BUFFSIZE, 0);
		lock_guard<mutex> mLock(gatewayMutex);
		if (intErrorCode == -1 || intErrorCode == 0) {
			cout << endl << "GatewayFront socket: " << intSocketClient << ", connection terminate" << endl;
			close(intSocketClient);

			intIndex = findSocket(intSocketClient);
			vectorIntSocketGateway.erase(vectorIntSocketGateway.begin() + intIndex);

			return;
		}
		cout << endl << "GatewayFront Socket: " << intSocketClient << ", recv size: " << intErrorCode << endl
            << "receive data: " << (int)charArrayBuff[0] << (int)charArrayBuff[1] << (int)charArrayBuff[2] << (int)charArrayBuff[3]
            << &charArrayBuff[4] << endl;


		// Package check
		// if index[0] == 1, server handle it
		if (charArrayBuff[0] == 1) {
			// if index[3] == 1, buff is ack
			if (charArrayBuff[3] == 1) {
				cout << "ack" << endl;
				// get index of buff in carInfoSend
				intIndex = findCarInfo(charArrayBuff[1], vectorCarInfoSend);
				// update package number
				if (vectorCarInfoSend[intIndex].intPackageNum == charArrayBuff[2]) {
					vectorCarInfoSend[intIndex].changePackageNum();
				}

			// buff is instruction from car
			}else {
				// get index of buff in carInfoSend
				intIndex = findCarInfo(charArrayBuff[1], vectorCarInfoReceive);
				// car isn't in the carInfoRecv
				if (intIndex == -1) {
					// add, update and send

					switch (charArrayBuff[3]) {
					case 2:
						int id, state;
						char* endP;
						id = (int)strtol(&charArrayBuff[4], &endP, 10);
						state = (int)strtol(endP + 1, &endP, 10);
						stateUpdate(id, state);

						// make ack package
						CarInfo carInfo;
						carInfo.intId = charArrayBuff[1];
						carInfo.intPackageNum = charArrayBuff[2];
						carInfo.intMessageSize = 4;
						for (int i = 0; i < BUFFSIZE; i++) {
							carInfo.charArrayBuff[i] = charArrayBuff[i];
						}
						carInfo.charArrayMessage[0] = 2;
						carInfo.charArrayMessage[1] = charArrayBuff[1];
						carInfo.charArrayMessage[2] = charArrayBuff[2];
						carInfo.charArrayMessage[3] = 1;

						vectorCarInfoReceive.push_back(carInfo);
						cout << (int)carInfo.charArrayMessage[0] << (int)carInfo.charArrayMessage[1] << (int)carInfo.charArrayMessage[2] << (int)carInfo.charArrayMessage[3];
						send(intSocketClient, carInfo.charArrayMessage, carInfo.intMessageSize, 0);
						break;
					case 3:
						cout << "car is broken" << endl;
						break;
					default:
						break;
					}

				// car is in the carInfoRecv
				}else {
					string s1, s2;
					s1 = vectorCarInfoReceive[intIndex].charArrayBuff;
					s2 = charArrayBuff;
					if (vectorCarInfoReceive[intIndex].intPackageNum == charArrayBuff[2] && s1 == s2) {
						// resend
						cout << "resend ack" << endl;
						send(intSocketClient, vectorCarInfoReceive[intIndex].charArrayMessage, vectorCarInfoReceive[intIndex].intMessageSize, 0);
					}else {
						// update and send

						switch (charArrayBuff[3]) {
						case 2:
							int id, state;
							char* endP;
							id = (int)strtol(&charArrayBuff[4], &endP, 10);
							state = (int)strtol(endP + 1, &endP, 10);
							stateUpdate(id, state);

							for (int i = 0; i < BUFFSIZE; i++) {
								vectorCarInfoReceive[intIndex].charArrayBuff[i] = charArrayBuff[i];
							}

							vectorCarInfoReceive[intIndex].intPackageNum = vectorCarInfoReceive[intIndex].charArrayMessage[2] = charArrayBuff[2];
							cout << (int)vectorCarInfoReceive[intIndex].charArrayMessage[0] << (int)vectorCarInfoReceive[intIndex].charArrayMessage[1] << (int)vectorCarInfoReceive[intIndex].charArrayMessage[2] << (int)vectorCarInfoReceive[intIndex].charArrayMessage[3];
							send(intSocketClient, vectorCarInfoReceive[intIndex].charArrayMessage, vectorCarInfoReceive[intIndex].intMessageSize, 0);

							break;
						case 3:
							cout << "car is broken" << endl;
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}

void stateUpdate(int id, int state) {
	cout << "StateUpdate start" << endl;
	lock_guard<mutex> mLock(sqlMutex);

	string idString, stateString;
	idString = "      ";
	stateString = " ";
	sprintf(&idString[0], "%d", id);
	sprintf(&stateString[0], "%d", state);
	stmt->execute("UPDATE transport SET state = " + stateString + " WHERE _id = " + idString);
}

void handleGatewayBack() {
    cout << "Function handleGatewayBack start" << endl;
    // test
    int i;
    while (true) {
        // test
        cin >> i;
        vector<string> message;
        getNewTransport(message);

        for (int i = 0; i < message.size(); i++) {
            cout << "gateway send hane message: " << message[i] << endl;
            new thread(gatewayBackgroundSend, message[i]);
        }
        //gatewaySleep();
    }
}

// Find carInfo with id, if exist return the index, else return -1
int findCarInfo(int intId, vector<CarInfo> vectorCarInfo) {
	for (int i = 0; i < vectorCarInfo.size(); i++) {
		if (intId == vectorCarInfo[i].intId) {
			return i;
		}
	}
	return -1;
}

int findSocket(int intSocketClient) {
	for (int i = 0; i < vectorIntSocketGateway.size(); i++) {
		if (intSocketClient == vectorIntSocketGateway[i]) {
			return i;
		}
	}
	return -1;
}

void getNewTransport(vector<string> &message) {
    cout << endl << "Function getNewTransport start" << endl;
    lock_guard<mutex> mLock(sqlMutex);

    using std::chrono::system_clock;
    time_t tt = system_clock::to_time_t(system_clock::now());

    struct tm* ptm = std::localtime(&tt);

    // test
    if (ptm->tm_min >= 30) {
        ptm->tm_min = 0;
        if (ptm->tm_hour == 23) {
            ptm->tm_hour = 0;
        }else {
            ptm->tm_hour++;
        }
    }else {
        ptm->tm_min = 30;
    }

    // test
    /*if (ptm->tm_min >= 50) {
        ptm->tm_min = 0;
        if (ptm->tm_hour == 23) {
            ptm->tm_hour = 0;
        }else {
            ptm->tm_hour++;
        }
    }else if(ptm->tm_min >= 20 && ptm->tm_min < 30) {
        ptm->tm_min = 30;
    }else {
        return;
    }*/

    ptm->tm_sec = 0;
    char charArrayBuff[80] = {'\0'};
    strftime(charArrayBuff, sizeof(charArrayBuff), "%Y-%m-%d %H:%M:%S", ptm);
    string time(charArrayBuff), header, stringEmpty;
    vector<int> startId, desId;

    header = "\2  \2";
    res = stmt->executeQuery("SELECT * FROM transport WHERE requireTime = \"" + time + "\"");
	for (int i = 0; res->next(); i++) {
		cout << "gateway background have message" << endl;
		header[1] = res->getInt("car_id");
		message.push_back(header);
		message[i].append(res->getString("_id") + ",");
		message[i].append(res->getString("sender") + ",");
		message[i].append(res->getString("receiver") + ",");
		message[i].append(res->getString("key") + ",");
		message[i].append(res->getString("requireTime") + ",");

		startId.push_back(res->getInt("start_id"));
		desId.push_back(res->getInt("des_id"));
	}
	delete res;
	for (int i = 0; i < startId.size(); i++) {
		res = stmt->executeQuery("SELECT * FROM location WHERE _id=" + to_string(startId[i]));
		res->next();
		message[i].append(res->getString("longitude") + ",");
		message[i].append(res->getString("latitude") + ",");
		delete res;
		res = stmt->executeQuery("SELECT * FROM location WHERE _id=" + to_string(desId[i]));
		res->next();
		message[i].append(res->getString("longitude") + ",");
		message[i].append(res->getString("latitude") + ",");
		delete res;
	}
}

void gatewayBackgroundSend(string message) {
	int index;
	index = findCarInfo(message[1], vectorCarInfoSend);
	if (index == -1) {
		lock_guard<mutex> mLock(sqlMutex);

		index = vectorCarInfoSend.size();

		// make ack package
		CarInfo carInfo;
		carInfo.intId = message[1];
		carInfo.intPackageNum = 1;
		carInfo.intMessageSize = message.size();
		for (int i = 0; i < message.size(); i++) {
			carInfo.charArrayMessage[i] = message[i];
		}
		vectorCarInfoSend.push_back(carInfo);
		message[2] = 1;
	}
	else {
		lock_guard<mutex> mLock(sqlMutex);
		message[2] = vectorCarInfoSend[index].intPackageNum;
		for (int i = 0; i < message.size(); i++) {
			vectorCarInfoSend[index].charArrayMessage[i] = message[i];
		}
	}

	while (vectorCarInfoSend[index].intPackageNum == message[2]) {
		gatewayMutex.lock();
		cout << "gateway background send message" << endl;
		cout << "data: " << (int)message[0] << (int)message[1] << (int)message[2] << (int)message[3] << endl;
		cout << &message[4] << endl;
		for (int i = 0; i < vectorIntSocketGateway.size(); i++) {
			send(vectorIntSocketGateway[i], &message[0], message.size(), 0);
		}
		gatewayMutex.unlock();
		this_thread::sleep_for(chrono::seconds(10));
	}
	cout << "gateway background send end" << endl;
}

void gatewaySleep() {
	using std::chrono::system_clock;
	time_t tt = system_clock::to_time_t(system_clock::now());

	struct tm * ptm = std::localtime(&tt);
	cout << "Current time: " << put_time(ptm, "%X") << '\n';

	cout << "Waiting for the next time to fetch data...\n";
	if (ptm->tm_min >= 50) {
		ptm->tm_hour++;
		ptm->tm_min = 20;
	}else if (ptm->tm_min >= 20) {
		ptm->tm_min = 50;
	}else {
		ptm->tm_min = 20;
	}
	ptm->tm_sec = 0;
	this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));

	cout << std::put_time(ptm, "%X") << " reached!\n";
}
