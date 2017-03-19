// Server.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "Socket.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <thread>
#include <mutex>
#include <string>

#define HOSTNAME "tcp://127.0.0.1:3306"
#define USERNAME "root"
#define PASSWARD "raspberry"
#define SCHEMA "postman"
#define BUFFSIZE 1000

using namespace std;

sql::Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res;

mutex sqlMutex;

/* Initialize and connect to MySQL database */
void initializeMySQL();
/* Identify connection is gateway or cellphone */
void handleNewConnection(SOCKET clientSocket);
void handleCellphoneTask(SOCKET clientSocket);
void handleGatewayTask(SOCKET clientSocket);

#include <locale>
#include <windows.h>
#include <cstdlib>

void testFunction() {

	stmt = con->createStatement();
	/*res = stmt->executeQuery("SELECT * FROM transport WHERE requireTime >= CURDATE()");
	res->next();
	sql::SQLString ss = res->getString("requireTime");
	double i = res->getDouble("requireTime");
	char cc[] = "圖a書a館a門a口";
	cout << cc << endl;
	cout << ss << endl;
	cout << i << endl;*/
	
	string tempStr = "123";
	string a = " ";
	a[0] = 100;
	tempStr.append("123\n97" + a);
	cout << tempStr << endl;

	string stmtStr;
	string dateStr;
	string dataStr;
	string buffStr;
	int tempInt, tempInt1, tempInt2;
	int index;
	int id = 4;
	tempStr = "20170318,";
	stmtStr = "SELECT * FROM transport WHERE sender=" + to_string(id) + " AND requireTime>=";

	tempInt = tempStr.find(",");
	dateStr = tempStr.substr(0, tempInt);
	cout << dateStr << endl;

	stmtStr.append(dateStr + " AND requireTime<ADDDATE(" + dateStr + ",1) ORDER BY requireTime");

	res = stmt->executeQuery(stmtStr);

	index = 0;
	tempStr = "";
	while (res->next()) {
		tempStr.append(res->getString("sender") + ",");
		tempStr.append(res->getString("receiver") + ",");
		tempStr.append(res->getString("requireTime") + ",");
		tempStr.append(res->getString("arriveTime") + ",");
		tempStr.append(res->getString("start_id") + ","); 
		tempStr.append(res->getString("des_id") + ",");
		tempStr.append(res->getString("state") + ",");
		tempStr.append(res->getString("key") + ",");

	}
	cout << tempStr << endl;

	// get sender
	tempInt2 = tempStr.find(",");
	dataStr = tempStr.substr(0, tempInt2);
	stmtStr = "SELECT name FROM user WHERE _id=" + dataStr;
	res = stmt->executeQuery(stmtStr);
	res->next();
	buffStr.append(res->getString("name") + ",");
	delete res;
	// get receiver
	tempInt1 = tempStr.find(",", tempInt2 + 1);
	dataStr = tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2 - 1);
	stmtStr = "SELECT name FROM user WHERE _id=" + dataStr;
	res = stmt->executeQuery(stmtStr);
	res->next();
	buffStr.append(res->getString("name") + ",");
	delete res;
	//get requireTime
	tempInt2 = tempStr.find(",", tempInt1 + 1);
	buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
	// get arriveTime
	tempInt1 = tempStr.find(",", tempInt2 + 1);
	buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
	// get startLocation
	tempInt2 = tempStr.find(",", tempInt1 + 1);
	dataStr = tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1 - 1);
	stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
	res = stmt->executeQuery(stmtStr);
	res->next();
	buffStr.append(res->getString("name") + ",");
	delete res;
	// get DesLocation
	tempInt1 = tempStr.find(",", tempInt2 + 1);
	dataStr = tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2 - 1);
	stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
	res = stmt->executeQuery(stmtStr);
	res->next();
	buffStr.append(res->getString("name") + ",");
	delete res;
	// get state
	tempInt2 = tempStr.find(",", tempInt1 + 1);
	buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
	// get key
	tempInt1 = tempStr.find(",", tempInt2 + 1);
	buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));

	cout << buffStr;
}

int main(void) {

	SOCKET listenSocket, clientSocket;

	try {

		initializeMySQL();
		cout << "Initialize database." << endl;

		testFunction();

		// Initialize Socket Listener
		listenSocket = getInitializeListenSocket();
		cout << "Initialize listenSocket" << endl;

		// Handle connections
		while (true) {
			// Accept socket connection
			clientSocket = getAcceptSocket(listenSocket);
			cout << "Accept a new client: " << clientSocket << endl;

			new thread(handleNewConnection, clientSocket);
			
		}

		delete stmt;
		delete con;

	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cout << endl;

	return EXIT_SUCCESS;
}

void initializeMySQL() {
	/* Create a MySQL connection */
	driver = get_driver_instance();
	con = driver->connect(HOSTNAME, USERNAME, PASSWARD);
	/* Connect to the MySQL postman database */
	con->setSchema(SCHEMA);
	stmt = con->createStatement();
}

void handleNewConnection(SOCKET clientSocket) {
	char buff;
	int errorCode;

	// Get identifier
	errorCode = recv(clientSocket, &buff, sizeof(char), 0);
	if (errorCode == -1 || errorCode == 0) {
		cout << "Socket: " << clientSocket << " connection terminate" << endl;
		return;
	}
	if (buff == 1) {
		// client is cellphone
		handleCellphoneTask(clientSocket);
	}else if (buff == 2) {
		// client is gateway
		handleGatewayTask(clientSocket);
	}
}

bool login(char* buff, string &account, int &id) {
	string password;
	string tempStr;
	int tempInt1, tempInt2;
	
	tempStr = buff;
	
	// get account
	tempInt1 = tempStr.find(",");
	account = tempStr.substr(0, tempInt1);
	// get password
	tempInt2 = tempStr.find(",", tempInt1 + 1);
	password = tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1 - 1);
	res = stmt->executeQuery("SELECT _id FROM user WHERE \"" + account + "\"=account AND \"" + password + "\"=password");

	if (res->next()) {
		id = res->getInt("_id");
		delete res;
		return true;
	}
	delete res;
	return false;
}

int createAccount(char* buff) {
	string account, password, name, email;
	string tempStr;
	int tempInt1, tempInt2;
	bool accountBool = true, emailBool = true;

	tempStr = buff;
	
	// get account
	tempInt1 = tempStr.find(",");
	account = tempStr.substr(0, tempInt1);
	//get password
	tempInt2 = tempStr.find(",", tempInt1 + 1);
	password = tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1 - 1);
	//get name
	tempInt1 = tempStr.find(",", tempInt2 + 1);
	name = tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2 - 1);
	// get email
	tempInt2 = tempStr.find(",", tempInt1 + 1);
	email = tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1 - 1);

	res = stmt->executeQuery("SELECT * FROM user WHERE account='" + account + "'");
	if (res->next()) {
		accountBool = false;
	}
	delete res;
	
	res = stmt->executeQuery("SELECT * FROM user WHERE mail='" + email + "'");
	if (res->next()) {
		emailBool = false;
	}
	delete res;

	if (accountBool) {
		if (emailBool) {
			stmt->execute("INSERT INTO user(account, password, name, mail, createTime) VALUES('" + account + "', '" + password + "', '" + name + "', '" + email + "', CURRENT_TIME)");
			return 1;
		}else {
			return 3;
		}
	}else {
		if (emailBool) {
			return 2;
		}else {
			return 4;
		}
	}
}

bool askTransportSend(int id, char* buff, string &buffStr) {
	try {
		string tempStr;
		string stmtStr;
		string dataStr;
		int tempInt1, tempInt2;
		int index;

		tempStr = buff;
		stmtStr = "SELECT * FROM transport WHERE sender=" + to_string(id) + " AND requireTime>=";

		tempInt1 = tempStr.find(",");
		dataStr = tempStr.substr(0, tempInt1);
		cout << dataStr << endl;

		stmtStr.append(dataStr + " AND requireTime<ADDDATE(" + dataStr + ",1) ORDER BY requireTime");

		res = stmt->executeQuery(stmtStr);

		index = 0;
		tempStr = "";
		while (res->next()) {
			tempStr.append(res->getString("receiver") + ",");
			tempStr.append(res->getString("requireTime") + ",");
			tempStr.append(res->getString("arriveTime") + ",");
			tempStr.append(res->getString("start_id") + ",");
			tempStr.append(res->getString("des_id") + ",");
			tempStr.append(res->getString("state") + ",");
			tempStr.append(res->getString("key") + ",");
			index++;
		}
		buffStr[1] = index;
		delete res;
		for (int i = 0; i < index; i++) {
			// get receiver
			tempInt1 = tempStr.find(",");
			dataStr = tempStr.substr(0, tempInt1);
			stmtStr = "SELECT name FROM user WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + ",");
			delete res;
			//get requireTime
			tempInt2 = tempStr.find(",", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get arriveTime
			tempInt1 = tempStr.find(",", tempInt2 + 1);
			buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
			// get startLocation
			tempInt2 = tempStr.find(",", tempInt1 + 1);
			dataStr = tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + ",");
			delete res;
			// get DesLocation
			tempInt1 = tempStr.find(",", tempInt2 + 1);
			dataStr = tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + ",");
			delete res;
			// get state
			tempInt2 = tempStr.find(",", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get key
			tempInt1 = tempStr.find(",", tempInt2 + 1);
			buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
		}
		return true;
	}catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		return false;
	}
}

bool askTransportRec(int id, char* buff, string &buffStr) {
	try {
		string tempStr;
		string stmtStr;
		string dataStr;
		int tempInt1, tempInt2;
		int index;

		tempStr = buff;
		stmtStr = "SELECT * FROM transport WHERE receiver=" + to_string(id) + " AND requireTime>=";

		tempInt1 = tempStr.find(",");
		dataStr = tempStr.substr(0, tempInt1);
		cout << dataStr << endl;

		stmtStr.append(dataStr + " AND requireTime<ADDDATE(" + dataStr + ",1) ORDER BY requireTime");

		res = stmt->executeQuery(stmtStr);

		index = 0;
		tempStr = "";
		while (res->next()) {
			tempStr.append(res->getString("sender") + ",");
			tempStr.append(res->getString("requireTime") + ",");
			tempStr.append(res->getString("arriveTime") + ",");
			tempStr.append(res->getString("start_id") + ",");
			tempStr.append(res->getString("des_id") + ",");
			tempStr.append(res->getString("state") + ",");
			tempStr.append(res->getString("key") + ",");
			index++;
		}
		buffStr[1] = index;
		delete res;
		for (int i = 0; i < index; i++) {
			// get receiver
			tempInt1 = tempStr.find(",");
			dataStr = tempStr.substr(0, tempInt1);
			stmtStr = "SELECT name FROM user WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + ",");
			delete res;
			//get requireTime
			tempInt2 = tempStr.find(",", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get arriveTime
			tempInt1 = tempStr.find(",", tempInt2 + 1);
			buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
			// get startLocation
			tempInt2 = tempStr.find(",", tempInt1 + 1);
			dataStr = tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + ",");
			delete res;
			// get DesLocation
			tempInt1 = tempStr.find(",", tempInt2 + 1);
			dataStr = tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + ",");
			delete res;
			// get state
			tempInt2 = tempStr.find(",", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get key
			tempInt1 = tempStr.find(",", tempInt2 + 1);
			buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
		}
		return true;
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		return false;
	}
}

bool askTransportSendRec(int id, char* buff, string &buffStr) {
	try {
		string tempStr;
		string stmtStr;
		string dataStr;
		int tempInt1, tempInt2;
		int index;

		tempStr = buff;
		stmtStr = "SELECT * FROM transport WHERE requireTime>=";

		tempInt1 = tempStr.find(",");
		dataStr = tempStr.substr(0, tempInt1);
		cout << dataStr << endl;

		stmtStr.append(dataStr + " AND requireTime<ADDDATE(" + dataStr + ",1) ORDER BY requireTime");

		res = stmt->executeQuery(stmtStr);

		index = 0;
		tempStr = "";
		while (res->next()) {
			tempStr.append(res->getString("sender") + ",");
			tempStr.append(res->getString("receiver") + ",");
			tempStr.append(res->getString("requireTime") + ",");
			tempStr.append(res->getString("arriveTime") + ",");
			tempStr.append(res->getString("start_id") + ",");
			tempStr.append(res->getString("des_id") + ",");
			tempStr.append(res->getString("state") + ",");
			tempStr.append(res->getString("key") + ",");
			index++;
		}
		buffStr[1] = index;
		delete res;
		for (int i = 0; i < index; i++) {
			// get sender
			tempInt2 = tempStr.find(",");
			dataStr = tempStr.substr(0, tempInt2);
			stmtStr = "SELECT name FROM user WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + ",");
			delete res;
			// get receiver
			tempInt1 = tempStr.find(",", tempInt2 + 1);
			dataStr = tempStr.substr(tempInt2 + 1, tempInt1 -tempInt2 - 1);
			stmtStr = "SELECT name FROM user WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + ",");
			delete res;
			//get requireTime
			tempInt2 = tempStr.find(",", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get arriveTime
			tempInt1 = tempStr.find(",", tempInt2 + 1);
			buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
			// get startLocation
			tempInt2 = tempStr.find(",", tempInt1 + 1);
			dataStr = tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + ",");
			delete res;
			// get DesLocation
			tempInt1 = tempStr.find(",", tempInt2 + 1);
			dataStr = tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + ",");
			delete res;
			// get state
			tempInt2 = tempStr.find(",", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get key
			tempInt1 = tempStr.find(",", tempInt2 + 1);
			buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
		}
		return true;
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		return false;
	}
}

void handleCellphoneTask(SOCKET clientSocket) {
	cout << "Start cellphone task" << endl;
	char buff[BUFFSIZE] = {'\0'};
	string buffStr;
	int errorCode;

	// User Info
	bool isLogin = false;
	string account;
	int id;

	while (true) {
		errorCode = recv(clientSocket, buff, BUFFSIZE, 0);

		lock_guard<mutex> mLock(sqlMutex);
		if (errorCode == -1 || errorCode == 0) {
			cout << "Socket: " << clientSocket << " connection terminate" << endl;
			closesocket(clientSocket);
			return;
		}
		cout << "Socket: " << clientSocket << " recv size: " << errorCode << endl << "data: " << (int)buff[0] << buff << endl;

		// error message
		if (buff[0] == 1) {
			cout << "error info: "<< &buff[1] << endl;
			return;
		}

		buffStr;
		if (!isLogin) {
			// buff for send

			switch (buff[0]) {
			case 2:
				buffStr = "  \n";
				buffStr[0] = 1;
				// create acount
				switch (createAccount(&buff[1])) {
				case 1:
					cout << "account: " << account << " create success" << endl;
					buffStr[1] = 1;
					send(clientSocket, &buffStr[0], buffStr.size(), 0);
					break;
				case 2:
					cout << "account: " << account << " create fail" << endl;
					buffStr[1] = 2;
					send(clientSocket, &buffStr[0], buffStr.size(), 0);
					break;
				case 3:
					cout << "account: " << account << " create fail" << endl;
					buffStr[1] = 3;
					send(clientSocket, &buffStr[0], buffStr.size(), 0);
					break;
				case 4:
					cout << "account: " << account << " create fail" << endl;
					buffStr[1] = 4;
					send(clientSocket, &buffStr[0], buffStr.size(), 0);
					break;
				default:
					break;
				}
				break;
			case 3:
				// log in
				isLogin = login(&buff[1], account, id);
				buffStr = "  \n";
				buffStr[0] = 3;
				if (isLogin) {
					cout << "account: " << account << " login success, id = " << id << endl;
					buffStr[1] = 1;
					send(clientSocket, &buffStr[0], buffStr.size(), 0);
				}else {
					cout << "account: " << account << " login fail" << endl;
					buffStr[1] = 0;
					send(clientSocket, &buffStr[0], buffStr.size(), 0);
				}
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				buffStr = " you need to login first\n";
				buffStr[0] = 1;
				send(clientSocket, &buffStr[0], buffStr.size(), 0);
				break;
			default:
				buffStr = " wrong state\n";
				buffStr[0] = 1;
				send(clientSocket, &buffStr[0], buffStr.size(), 0);
				break;
			}
		}
		else {
			switch (buff[0]) {
			case 2:
			case 3:
				buffStr = " you already login\n";
				buffStr[0] = 1;
				send(clientSocket, &buffStr[0], buffStr.size(), 0);
				break;
			/*case 4:
				break;
			case 5:
				break;
			case 6:
				break;*/
			case 7:
				buffStr = "  ";
				buffStr[0] = 7;
				askTransportSend(id, &buff[1], buffStr);
				send(clientSocket, &buffStr[0], buffStr.size(), 0);
				break;
			case 8:
				buffStr = "  ";
				buffStr[0] = 8;
				askTransportRec(id, &buff[1], buffStr);
				send(clientSocket, &buffStr[0], buffStr.size(), 0);
				break;
			case 9:
				buffStr = "  ";
				buffStr[0] = 9;
				break;
			case 10:
				break;
			}
		}
		
	}
}

void handleGatewayTask(SOCKET clientSocket) {
	cout << "Start gateway task" << endl;
	while (true) {

	}
}