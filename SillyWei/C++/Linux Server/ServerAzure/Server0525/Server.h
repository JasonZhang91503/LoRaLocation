#ifndef _SERVER_H
#define _SERVER_H

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <thread>
#include <mutex>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <chrono>
#include <iomanip>

#define HOSTNAME "tcp://127.0.0.1:3306"
#define USERNAME "root"
#define PASSWORD "Jizzbear2017"
#define SCHEMA "postman"
#define BUFFSIZE 5000

using namespace std;

extern sql::Driver *driver;
extern sql::Connection *con;
extern sql::Statement *stmt;
extern sql::ResultSet *res;

extern mutex sqlMutex;
extern mutex gatewayMutex;

void initializeMySQL();
void handleNewConnection(int intSocketClient);
void cleanCharArrayBuffer(char charArray[], int intSize);
string makeKey(int intSize);

#endif
