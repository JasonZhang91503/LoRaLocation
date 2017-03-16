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
	res = stmt->executeQuery("SELECT name FROM location WHERE _id = 1");
	res->next();
	sql::SQLString ss = res->getString("name");
	char cc[] = "圖a書a館a門a口";
	cout << cc << endl;
	cout << ss << endl;
}

int main(void) {

	SOCKET listenSocket, clientSocket;

	try {

		initializeMySQL();
		cout << "Initialize database." << endl;

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

void handleCellphoneTask(SOCKET clientSocket) {
	cout << "Start cellphone task" << endl;
	char buff[BUFFSIZE] = {'\0'};
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

		if (!isLogin) {
			// buff for send
			char c[2];

			switch (buff[0]) {
			case 2:
				// create acount
				c[0] = 2;
				switch (createAccount(&buff[1])) {
				case 1:
					cout << "account: " << account << " create success" << endl;
					c[1] = 1;
					send(clientSocket, c, 2, 0);
					break;
				case 2:
					cout << "account: " << account << " create fail" << endl;
					c[1] = 2;
					send(clientSocket, c, 2, 0);
					break;
				case 3:
					cout << "account: " << account << " create fail" << endl;
					c[1] = 3;
					send(clientSocket, c, 2, 0);
					break;
				case 4:
					cout << "account: " << account << " create fail" << endl;
					c[1] = 4;
					send(clientSocket, c, 2, 0);
					break;
				default:
					break;
				}
				break;
			case 3:
				// log in
				isLogin = login(&buff[1], account, id);
				c[0] = 3;
				if (isLogin) {
					cout << "account: " << account << " login success, id = " << id << endl;
					c[1] = 1;
					send(clientSocket, c, 2, 0);
				}else {
					cout << "account: " << account << " login fail" << endl;
					c[1] = 0;
					send(clientSocket, c, 2, 0);
				}
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			{
				char message1[] = "you need to login first";
				send(clientSocket, message1, 24, 0);
				break;
			}
			default:
				char message2[] = "wrong state";
				send(clientSocket, message2, 12, 0);
				break;
			}
		}
		else {
			switch (buff[0]) {
			case 2:
			case 3:
				char message1[] = "you already login";
				send(clientSocket, message1, 18, 0);
				break;
			/*case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			case 9:
				break;
			case 10:
				break;*/
			}
		}
		
	}
}

void handleGatewayTask(SOCKET clientSocket) {
	cout << "Start gateway task" << endl;
	while (true) {

	}
}