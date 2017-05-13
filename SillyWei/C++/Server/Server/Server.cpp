// Server.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "Socket.h"
#include <string.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <thread>
#include <mutex>
#include <string>
#include <vector>

#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>

#pragma warning(disable:4996)

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
mutex gatewayMutex;

/* Initialize and connect to MySQL database */
void initializeMySQL();
/* Identify connection is gateway or cellphone */
void handleNewConnection(SOCKET clientSocket);
void handleCellphoneTask(SOCKET clientSocket);
void handleCellphoneBack(SOCKET clientSocket);
void handleGatewayTask(SOCKET clientSocket);
void gatewaySend();

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

		//testFunction();

		// Initialize Socket Listener
		listenSocket = getInitializeListenSocket();
		cout << "Initialize listenSocket" << endl;

		// Create background thread for send event check
		new thread(gatewaySend);

		// Handle connections
		while (true) {
			// Accept socket connection
			clientSocket = getAcceptSocket(listenSocket);
			cout << endl << "Accept a new client: " << clientSocket << endl;

			// Handle connection
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
	if (buff == '1') {
		// client is cellphone
		handleCellphoneTask(clientSocket);
	}else if (buff == '2') {
		// client is gateway
		handleGatewayTask(clientSocket);
	}else if (buff == '3') {
		//client is cellphone back
		handleCellphoneBack(clientSocket);
	}else {
		cout << "Device error socket: " << clientSocket << endl;
	}
}

bool login(char* buff, string &account, int &id, string &name, string &email) {
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

	cout << account << " " << password << endl;
	
	res = stmt->executeQuery("SELECT _id, name, mail FROM user WHERE \"" + account + "\"=account AND \"" + password + "\"=password");

	if (res->next()) {
		id = res->getInt("_id");
		name = res->getString("name");
		email = res->getString("mail");
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

bool createTransport(int intId, char* charPtrBuff, string strSend) {
	string strSender;
	string strReceiver;
	string strRequireTime;
	string strStartLocation;
	string strDestLocation;
	string strRemark;


	string strTemp;
	int intTemp1;
	int intTemp2;

	strTemp = charPtrBuff;

	// get sender
	intTemp1 = strTemp.find(",");
	strSender = strTemp.substr(0, intTemp1);
	res = stmt->executeQuery("SELECT _id FROM user WHERE account=\"" + strSender + "\"");
	if (res->next()) {
		strSender = res->getString("_id");
		delete res;
	}else {
		delete res;
		return false;
	}
	// get receiver
	intTemp2 = strTemp.find(",", intTemp1 + 1);
	strReceiver = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);
	res = stmt->executeQuery("SELECT _id FROM user WHERE account=\"" + strReceiver + "\"");
	if (res->next()) {
		strReceiver = res->getString("_id");
		delete res;
	}else {
		delete res;
		return false;
	}
	// get requireTime
	intTemp1 = strTemp.find(",", intTemp2 + 1);
	strRequireTime = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 - 1);
	// get startLocation
	intTemp2 = strTemp.find(",", intTemp1 + 1);
	strStartLocation = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);
	res = stmt->executeQuery("SELECT _id FROM location WHERE name=\"" + strStartLocation + "\"");
	if (res->next()) {
		strStartLocation = res->getString("_id");
		delete res;
	}else {
		delete res;
		return false;
	}
	// get desLocation
	intTemp1 = strTemp.find(",", intTemp2 + 1);
	strDestLocation = strTemp.substr(intTemp2 + 1, intTemp1 - intTemp2 - 1);
	res = stmt->executeQuery("SELECT _id FROM location WHERE name=\"" + strDestLocation + "\"");
	if (res->next()) {
		strDestLocation = res->getString("_id");
		delete res;
	}else {
		delete res;
		return false;
	}
	// get remark
	intTemp2 = strTemp.find(",", intTemp1 + 1);
	strRemark = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);
	stmt->execute("INSERT INTO transport VALUES (default,\""+ strRequireTime + "\",\"0-0-0 0:0:0\"," +
		strSender + "," + strReceiver + "," + strStartLocation + "," + strDestLocation + ",1,0,5496,\"" + strRemark + "\")");
	
	return true;
}

void carAvailableTime(int id, char* buff, string &strSend) {
	string strDate;
	string strTransportTime;

	int intTemp;
	int intHour, intMinute;
	int intIndex;
	char* charPtrEnd;
	string strTemp;

	cout << "CarAvailableTime Fumction" << endl;

	strTemp = buff;

	// store date into a string
	intTemp = strTemp.find(",");
	strDate = strTemp.substr(0, intTemp);
	cout << strDate << endl;

	// get the transport in that date
	res = stmt->executeQuery("SELECT requireTime FROM transport WHERE requireTime>=\"" + strDate + 
		"\"AND requireTime<ADDDATE(\"" + strDate + "\",1)ORDER BY requireTime");

	// add available time to strSend
	intTemp = strSend.size();
	strSend.append("111111111111111111\n");

	while (res->next()) {
		strTransportTime = res->getString("requireTime");
		strTransportTime = strTransportTime.substr(11, 5);
		intHour = strtol(&strTransportTime.substr(0, 2)[0], &charPtrEnd, 10);
		intMinute = strtol(&strTransportTime.substr(3, 2)[0], &charPtrEnd, 10);
		intIndex = intTemp + intMinute / 30 + (intHour - 9) * 2;
		strSend[intIndex] = '0';
	}
	delete res;
}

bool askTransportSend(int id, char* buff, string &buffStr) {
	try {
		string tempStr;
		string stmtStr;
		string dataStr;
		int tempInt1, tempInt2;
		int index;

		tempStr = buff;
		stmtStr = "SELECT * FROM transport WHERE sender=" + to_string(id) + " AND requireTime>\"";

		tempInt1 = tempStr.find(",");
		dataStr = tempStr.substr(0, tempInt1);
		cout << dataStr << endl;

		stmtStr.append(dataStr + "\" AND requireTime<ADDDATE(\"" + dataStr + "\",1) ORDER BY requireTime");

		res = stmt->executeQuery(stmtStr);

		index = 0;
		tempStr = "";
		while (res->next()) {
			tempStr.append(res->getString("receiver") + "~");
			tempStr.append(res->getString("requireTime") + "^");
			tempStr.append(res->getString("arriveTime") + ";");
			tempStr.append(res->getString("start_id") + "/");
			tempStr.append(res->getString("des_id") + "!");
			tempStr.append(res->getString("state") + "#");
			tempStr.append(res->getString("key") + "$");
			tempStr.append(res->getString("remark") + "*");
			index++;
		}
		delete res;
		buffStr.append(to_string(index));

		tempInt2 = -1;
		for (int i = 0; i < index; i++) {
			// get receiver
			tempInt1 = tempStr.find("~", tempInt2 + 1);
			dataStr = tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2 - 1);
			stmtStr = "SELECT name FROM user WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + "~");
			delete res;
			//get requireTime
			tempInt2 = tempStr.find("^", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get arriveTime
			tempInt1 = tempStr.find(";", tempInt2 + 1);
			buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
			// get startLocation
			tempInt2 = tempStr.find("/", tempInt1 + 1);
			dataStr = tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + "/");
			delete res;
			// get DesLocation
			tempInt1 = tempStr.find("!", tempInt2 + 1);
			dataStr = tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + "!");
			delete res;
			// get state and key
			tempInt2 = tempStr.find("*", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get 
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
		stmtStr = "SELECT * FROM transport WHERE receiver=" + to_string(id) + " AND requireTime>\"";

		tempInt1 = tempStr.find(",");
		dataStr = tempStr.substr(0, tempInt1);
		cout << dataStr << endl;

		stmtStr.append(dataStr + "\" AND requireTime<ADDDATE(\"" + dataStr + "\",1) ORDER BY requireTime");

		res = stmt->executeQuery(stmtStr);

		index = 0;
		tempStr = "";
		while (res->next()) {
			tempStr.append(res->getString("sender") + "~");
			tempStr.append(res->getString("requireTime") + "^");
			tempStr.append(res->getString("arriveTime") + ";");
			tempStr.append(res->getString("start_id") + "/");
			tempStr.append(res->getString("des_id") + "!");
			tempStr.append(res->getString("state") + "#");
			tempStr.append(res->getString("key") + "$");
			tempStr.append(res->getString("remark") + "*");
			index++;
		}
		delete res;
		buffStr.append(to_string(index));
		tempInt2 = -1;
		for (int i = 0; i < index; i++) {
			// get receiver
			tempInt1 = tempStr.find("~", tempInt2 - 1);
			dataStr = tempStr.substr(tempInt2 + 1, tempInt1- tempInt2 - 1);
			stmtStr = "SELECT name FROM user WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + "~");
			delete res;
			//get requireTime
			tempInt2 = tempStr.find("^", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get arriveTime
			tempInt1 = tempStr.find(";", tempInt2 + 1);
			buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
			// get startLocation
			tempInt2 = tempStr.find("/", tempInt1 + 1);
			dataStr = tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + "/");
			delete res;
			// get DesLocation
			tempInt1 = tempStr.find("!", tempInt2 + 1);
			dataStr = tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + "!");
			delete res;
			// get state and key
			tempInt2 = tempStr.find("*", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
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
		stmtStr = "SELECT * FROM transport WHERE (receiver=" + to_string(id) + " Or sender=" + to_string(id) + ") AND requireTime>\"";

		tempInt1 = tempStr.find(",");
		dataStr = tempStr.substr(0, tempInt1);
		cout << dataStr << endl;

		stmtStr.append(dataStr + "\" AND requireTime<ADDDATE(\"" + dataStr + "\",1) ORDER BY requireTime");
		cout << stmtStr << endl;
		res = stmt->executeQuery(stmtStr);

		index = 0;
		tempStr = "";
		while (res->next()) {
			tempStr.append(res->getString("sender") + "~");
			tempStr.append(res->getString("receiver") + "^");
			tempStr.append(res->getString("requireTime") + ";");
			tempStr.append(res->getString("arriveTime") + "/");
			tempStr.append(res->getString("start_id") + "!");
			tempStr.append(res->getString("des_id") + "#");
			tempStr.append(res->getString("state") + "$");
			tempStr.append(res->getString("key") + "%");
			tempStr.append(res->getString("remark") + "*");
			index++;
		}
		delete res;
		buffStr.append(to_string(index));






		tempInt1 = -1;
		// same item send;
		for (int i = 0; i < index; i++) {
			// get sender
			tempInt2 = tempStr.find("~", tempInt1 + 1);
			dataStr = tempStr.substr(0, tempInt2 - tempInt1 - 1);
			stmtStr = "SELECT name FROM user WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + "~");
			delete res;
			// get receiver
			tempInt1 = tempStr.find("^", tempInt2 + 1);
			dataStr = tempStr.substr(tempInt2 + 1, tempInt1 -tempInt2 - 1);
			stmtStr = "SELECT name FROM user WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + "^");
			delete res;
			//get requireTime
			tempInt2 = tempStr.find(";", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get arriveTime
			tempInt1 = tempStr.find("/", tempInt2 + 1);
			buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
			// get startLocation
			tempInt2 = tempStr.find("!", tempInt1 + 1);
			dataStr = tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + "!");
			delete res;
			// get DesLocation
			tempInt1 = tempStr.find("#", tempInt2 + 1);
			dataStr = tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2 - 1);
			stmtStr = "SELECT name FROM location WHERE _id=" + dataStr;
			res = stmt->executeQuery(stmtStr);
			res->next();
			buffStr.append(res->getString("name") + "#");
			delete res;
			// get state
			tempInt2 = tempStr.find("$", tempInt1 + 1);
			buffStr.append(tempStr.substr(tempInt1 + 1, tempInt2 - tempInt1));
			// get key
			tempInt1 = tempStr.find("*", tempInt2 + 1);
			buffStr.append(tempStr.substr(tempInt2 + 1, tempInt1 - tempInt2));
		}
		cout << buffStr << endl;
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

void getAccountName(int intId, string &buffStr) {

	int index = 0;

	string tempStr;

	res = stmt->executeQuery("SELECT * FROM user WHERE _id!=" + to_string(intId));

	while (res->next()) {
		tempStr.append(res->getString("account") + "*");
		index++;
	}
	buffStr.append(to_string(index) + "^" + tempStr + "\n");
}

void getLocationName(string &buffStr) {
	
	int index = 0;
	
	string tempStr;
		
	res = stmt->executeQuery("SELECT * FROM location");

	while (res->next()) {
		tempStr.append(res->getString("name") + "*");
		index++;
	}
	buffStr.append(to_string(index) + "^" + tempStr + "\n");
}

bool changeAccountData(int id , char* buff) {
	string strPassword;
	string strMail;

	string strTemp;
	int intTemp1;
	int intTemp2;

	strTemp = buff;
	intTemp1 = strTemp.find(",");
	intTemp2 = strTemp.find(",", intTemp1 + 1);
	strPassword = strTemp.substr(0, intTemp1);
	strMail = strTemp.substr(intTemp1 + 1, intTemp2 - intTemp1 - 1);

	res = stmt->executeQuery("SELECT * FROM user WHERE _id!=" + to_string(id) + " AND mail=\"" + strMail + "\"");
	if (res->next()) {
		delete res;
		return false;
	}
	delete res;

	stmt->execute("UPDATE user SET password=\"" + strPassword + "\",mail=\"" + strMail + "\"WHERE _id=" + to_string(id));

	return true;
}

void handleCellphoneTask(SOCKET clientSocket) {
	cout << "Start cellphone task" << endl;
	string buffStr;
	int errorCode;
	int recognizeNumber;

	// User Info
	bool isLogin = false;
	string account, name, email;
	int id;

	while (true) {
		char buff[BUFFSIZE] = { '\0' }; 
		errorCode = recv(clientSocket, buff, BUFFSIZE, 0);

		lock_guard<mutex> mLock(sqlMutex);
		if (errorCode == -1 || errorCode == 0) {
			cout << endl << "Cellphone task socket: " << clientSocket << " connection terminate" << endl;
			closesocket(clientSocket);
			return;
		}
		cout << endl << "Cellphone task Socket: " << clientSocket << " recv size: " << errorCode << endl << "receive data: " << buff << endl;

		char* endP;
		recognizeNumber = (int)strtol(buff, &endP, 10);

		// error message
		if (recognizeNumber == 20) {
			cout << "error info: " << &buff[1] << endl;
			return;
		}else if (!isLogin) {
			// buff for send

			switch (recognizeNumber) {
			case 2:
				// create acount
				cout << "Task create account" << endl;
				buffStr = "2, \n";
				switch (createAccount(&buff[2])) {
				case 1:
					cout << "account: " << account << " create success" << endl;
					buffStr[2] = '1';
					break;
				case 2:
					cout << "account: " << account << " create fail" << endl;
					buffStr[2] = '2';
					break;
				case 3:
					cout << "account: " << account << " create fail" << endl;
					buffStr[2] = '3';
					break;
				case 4:
					cout << "account: " << account << " create fail" << endl;
					buffStr[2] = '4';
					break;
				default:
					break;
				}
				break;
			case 3:
				// log in
				cout << "Task login" << endl;
				isLogin = login(&buff[2], account, id, name, email);
				buffStr = "3, ";
				if (isLogin) {
					cout << "account: " << account << " login success, id = " << id << endl;
					buffStr[2] = '1';
					buffStr.append(name + "^" + email + "\n");
				}else {
					cout << "account: " << account << " login fail" << endl;
					buffStr[2] = '0';
					buffStr.append("\n");
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
				buffStr = "20,you need to login first\n";
				break;
			default:
				buffStr = "20,wrong state\n";
				send(clientSocket, &buffStr[0], buffStr.size(), 0);
				break;
			}
		}else {
			switch (recognizeNumber) {
			case 2:
			case 3:
				buffStr = "20,you already login\n";
				break;
			case 4:
				// create transport
				cout << "Task create transport" << endl;
				buffStr = "4,1\n";
				if (!createTransport(id, &buff[2], buffStr)) {
					buffStr[2] = '0';
				}
				break;
			case 5:
				// ask car available time
				cout << "Task ask for car available time" << endl;
				buffStr = "5,";
				carAvailableTime(id, &buff[2], buffStr);
				break;
			case 6:
				// ask user data
				cout << "Task ask for user data" << endl;
				buffStr = "6,";
				cout << "account: " << account << ", id = " << id << endl;
				buffStr.append(name + "^" + email + "\n");
				break;
			case 7:
				// ask for send transport
				cout << "Task ask for transport send data" << endl;
				buffStr = "7,";
				if (askTransportSend(id, &buff[2], buffStr)) {
					buffStr.append("\n");
				}else {
					buffStr.append("error\n");
				}
				break;
			case 8:
				// ask for receive transport
				cout << "Task ask for transport receive data" << endl;
				buffStr = "8,";
				if (askTransportRec(id, &buff[2], buffStr)) {
					buffStr.append("\n");
				}else {
					buffStr.append("error\n");
				}
				break;
			case 9:
				// ask for transport
				cout << "Task ask for transport data" << endl;
				buffStr = "9,";
				if (askTransportSendRec(id, &buff[2], buffStr)) {
					buffStr.append("\n");
				}else {
					buffStr.append("error\n");
				}
				break;
			case 10:
				// ask all user
				cout << "Task ask for all user" << endl;
				buffStr = "10,";
				getAccountName(id, buffStr);
				break;
			case 11:
				// ask all location name
				cout << "Task ask for all location name" << endl;
				buffStr = "11,";
				getLocationName(buffStr);
				break;
			case 12:
				// update user date
				cout << "Task ask for user data" << endl;
				if (changeAccountData(id, &buff[3])) {
					buffStr = "12,1\n";
				}else {
					buffStr = "12,0\n";
				}
				break;
			}
		}
		// Send data
		send(clientSocket, &buffStr[0], buffStr.size(), 0);
		if (errorCode == -1 || errorCode == 0) {
			cout << endl << "Cellphone task socket: " << clientSocket << " connection terminate" << endl;
			closesocket(clientSocket);
			return;
		}
		cout << "Cellphone task Socket: " << clientSocket << " send size: " << errorCode << endl << "send data: " << buffStr << endl;
	}
}

bool getArriveTransport(int id, string &strSend) {
	lock_guard<mutex> mLock(sqlMutex);

	using std::chrono::system_clock;
	time_t tt = system_clock::to_time_t(system_clock::now());
	struct tm* ptm = localtime(&tt);

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
	res = stmt->executeQuery("SELECT * FROM transport WHERE state=1 AND requireTime=\"" +
		strTime + "\"AND sender=" + to_string(id));
	if (res->next()) {
		strSend[2] = 1;
		delete res;
		return true;
	}
	delete res;

	// check receive
	res = stmt->executeQuery("SELECT * FROM transport WHERE state=3 AND requireTime=\"" +
		strTime + "\"AND receiver=" + to_string(id));
	if (res->next()) {
		strSend[2] = 0;
		delete res;
		return true;
	}
	delete res;
	return false;
}

void phoneWakeTime() {
	this_thread::sleep_for(chrono::seconds(30));
}

void handleCellphoneBack(SOCKET socketClient) {
	cout << "Start handleCellphoneBack" << endl;

	int id;

	int intErrorCode;
	char charArrayBuff[BUFFSIZE];
	string strTemp;

	intErrorCode = recv(socketClient, charArrayBuff, BUFFSIZE, 0);
	if (intErrorCode == -1 || intErrorCode == 0) {
		cout << "Socket: " << socketClient << " connection terminate" << endl;
		closesocket(socketClient);
		return;
	}
	{
		lock_guard<mutex> mLock(sqlMutex);
		if (!login(charArrayBuff, strTemp, id, strTemp, strTemp)) {
			cout << "Socket: " << socketClient << " connection terminate" << endl;
			send(socketClient, "1,0\n", 4, 0);
			closesocket(socketClient);
			return;
		}
		send(socketClient, "1,1\n", 4, 0);
	}

	while (true) {
		string strSend;
		strSend = "2, \n";
		if (getArriveTransport(id, strSend)) {
			send(socketClient, &strSend[0], strSend.size(), 0);
		}
		phoneWakeTime();
	}
}


class CarInfo {
public:
	int id;
	int packageNum;
	int intMessageSize;
	char buff[BUFFSIZE];
	char message[BUFFSIZE];

	void changePackageNum() {
		if (packageNum == 1) {
			packageNum = 2;
		}else if (packageNum == 2) {
			packageNum = 1;
		}
	}
};

vector<SOCKET> socketGateway;
vector<CarInfo> carInfoSend;
vector<CarInfo> carInfoReceive;

// Find carInfo with id, if exist return the index, else return -1
int findCarInfo(int id, vector<CarInfo> carInfo) {
	for (int i = 0; i < carInfo.size(); i++) {
		if (id == carInfo[i].id) {
			return i;
		}
	}
	return -1;
}

int findSocket(SOCKET clientSocket) {
	for (int i = 0; i < socketGateway.size(); i++) {
		if (clientSocket == socketGateway[i]) {
			return i;
		}
	}
	return -1;
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

void handleGatewayTask(SOCKET clientSocket) {
	int errorCode;
	int index;
	cout << "Start gateway task" << endl;

	socketGateway.push_back(clientSocket);

	while (true) {
		// Receive data
		char buff[BUFFSIZE] = { '\0' };
		errorCode = recv(clientSocket, buff, BUFFSIZE, 0);
		lock_guard<mutex> mLock(gatewayMutex);
		if (errorCode == -1 || errorCode == 0) {
			cout << "Socket: " << clientSocket << " connection terminate" << endl;
			closesocket(clientSocket);

			index = findSocket(clientSocket);
			socketGateway.erase(socketGateway.begin() + index);

			return;
		}
		cout << "Socket: " << clientSocket << " recv size: " << errorCode << endl;
		cout << "data: " << (int)buff[0] << (int)buff[1] << (int)buff[2] << (int)buff[3] << &buff[4] << endl;
		//

		
		// Package check
		// if index[0] == 1, server handle it
		if (buff[0] == 1) {
			// if index[3] == 1, buff is ack
			if (buff[3] == 1) {
				cout << "ack" << endl;
				// get index of buff in carInfoSend
				index = findCarInfo(buff[1], carInfoSend);
				// update package number
				if (carInfoSend[index].packageNum == buff[2]) {
					carInfoSend[index].changePackageNum();
				}

			// buff is instruction from car
			}else {
				// get index of buff in carInfoSend
				index = findCarInfo(buff[1], carInfoReceive);
				// car isn't in the carInfoRecv
				if (index == -1) {
					// add, update and send
					
					switch (buff[3]) {
					case 2:
						int id, state;
						char* endP;
						id = (int)strtol(&buff[4], &endP, 10);
						state = (int)strtol(endP + 1, &endP, 10);
						stateUpdate(id, state);

						// make ack package
						CarInfo carInfo;
						carInfo.id = buff[1];
						carInfo.packageNum = buff[2];
						carInfo.intMessageSize = 4;
						for (int i = 0; i < BUFFSIZE; i++) {
							carInfo.buff[i] = buff[i];
						}
						carInfo.message[0] = 2;
						carInfo.message[1] = buff[1];
						carInfo.message[2] = buff[2];
						carInfo.message[3] = 1;

						carInfoReceive.push_back(carInfo);
						cout << (int)carInfo.message[0] << (int)carInfo.message[1] << (int)carInfo.message[2] << (int)carInfo.message[3];
						send(clientSocket, carInfo.message, carInfo.intMessageSize, 0);
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
					s1 = carInfoReceive[index].buff;
					s2 = buff;
					if (carInfoReceive[index].packageNum == buff[2] && s1 == s2) {
						// resend
						cout << "resend ack" << endl;
						send(clientSocket, carInfoReceive[index].message, carInfoReceive[index].intMessageSize, 0);
					}else {
						// update and send

						switch (buff[3]) {
						case 2:
							int id, state;
							char* endP;
							id = (int)strtol(&buff[4], &endP, 10);
							state = (int)strtol(endP + 1, &endP, 10);
							stateUpdate(id, state);
							
							for (int i = 0; i < BUFFSIZE; i++) {
								carInfoReceive[index].buff[i] = buff[i];
							}
							
							carInfoReceive[index].packageNum = carInfoReceive[index].message[2] = buff[2];
							cout << (int)carInfoReceive[index].message[0] << (int)carInfoReceive[index].message[1] << (int)carInfoReceive[index].message[2] << (int)carInfoReceive[index].message[3];
							send(clientSocket, carInfoReceive[index].message, carInfoReceive[index].intMessageSize, 0);

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
		//
	}
}

void wakeTime() {
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

void getNewTransport(vector<string> &message) {
	cout << "getNewTransport start" << endl;
	lock_guard<mutex> mLock(sqlMutex);

	using std::chrono::system_clock;
	time_t tt = system_clock::to_time_t(system_clock::now());

	struct tm * ptm = std::localtime(&tt);
	
	if (ptm->tm_min >= 50) {
		ptm->tm_min = 0;
		ptm->tm_hour++;
	}else if (ptm->tm_min >= 20 && ptm->tm_min < 30) {
		ptm->tm_min = 30;
	}else {
		return;
	}

	ptm->tm_sec = 0;
	char buff[80] = {'\0'};
	strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", ptm);
	string time(buff), header, stringEmpty;
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
	index = findCarInfo(message[1], carInfoSend);
	if (index == -1) {
		lock_guard<mutex> mLock(sqlMutex);

		index = carInfoSend.size();

		// make ack package
		CarInfo carInfo;
		carInfo.id = message[1];
		carInfo.packageNum = 1;
		carInfo.intMessageSize = message.size();
		for (int i = 0; i < message.size(); i++) {
			carInfo.message[i] = message[i];
		}
		carInfoSend.push_back(carInfo);
		message[2] = 1;
	}
	else {
		lock_guard<mutex> mLock(sqlMutex);
		message[2] = carInfoSend[index].packageNum;
		for (int i = 0; i < message.size(); i++) {
			carInfoSend[index].message[i] = message[i];
		}
	}

	while (carInfoSend[index].packageNum == message[2]) {
		gatewayMutex.lock();
		cout << "gateway background send message" << endl;
		cout << "data: " << (int)message[0] << (int)message[1] << (int)message[2] << (int)message[3] << endl;
		cout << &message[4] << endl;
		for (int i = 0; i < socketGateway.size(); i++) {
			send(socketGateway[i], &message[0], message.size(), 0);
		}
		gatewayMutex.unlock();
		this_thread::sleep_for(chrono::seconds(10));
	}
	cout << "gateway background send end" << endl;
}

void gatewaySend() {

	while (true) {
		system("PAUSE");
		vector<string> message;
		getNewTransport(message);

		for (int i = 0; i < message.size(); i++) {
			cout << "gateway Send have message: " << message[i] << endl;
			new thread(gatewayBackgroundSend, message[i]);
		}
		wakeTime();
	}
}