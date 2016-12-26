//
//  main.cpp
//  test
//
//  Created by Keng on 07/12/2016.
//  Copyright © 2016 Keng. All rights reserved.
//

#include <iostream>
#include<my_global.h>
#include <mysql.h>
#include <string.h>

using namespace std;
#define DATABASE_USERNAME "root"
#define DATABASE_PASSWORD "s410385007"
#define DATABASE_NAME "bonjour"
#define IP "192.168.100.8"
#define PORT 3306

//mysql_real_connect(你選擇的資料庫, IP, DATABASE_USERNAME, DATABASE_PASSWORD, DATABASE_NAME, PORT,socket ,Client_flag)==NULL
//mysql_query ->傳遞指令 mysql_query(你選擇的資料庫,“SQL指令”);
//mysql_errno ->驗證錯誤碼 mysql_errno(你選擇的資料庫)，得到錯誤指令碼後，上網查錯誤訊息

void send_Command(MYSQL *db,char *command){
    if(mysql_query(db,command)==0){
        cout<<"ok"<<endl;
    }
    else{
        int e = mysql_errno(db);
        cout<<"error: "<<e<<endl;
    }

    
}
int main(int argc, const char * argv[]) {
    // insert code here...
    MYSQL *mysql1;
    mysql1=mysql_init(NULL);
    
    if(mysql1 == NULL)
    {
        fprintf(stderr, "ABB : %s\n", mysql_error(mysql1));
        return 0; 
    }
    cout<<"check"<<endl;
    if(mysql_real_connect(mysql1, IP, DATABASE_USERNAME, DATABASE_PASSWORD, NULL, PORT,NULL ,0)==NULL){
        int e = mysql_errno(mysql1);
        cout<<"error: "<<e<<endl;
    }
    else{
        
    // send_Command(mysql1, "create database bonjour");
    // send_Command(mysql1, "use bonjour");
    // send_Command(mysql1, "create table ntpu(id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,department VARCHAR(50),people INT)");
    // send_Command(mysql1, "insert into ntpu(department,people) values(\"foreign language\",946)");
        send_Command(mysql1, "use bonjour");
        MYSQL_RES *res=mysql_list_fields(mysql1, "ntpu", "%");
        unsigned int field_cnt = mysql_num_fields(res);
        printf("Number of columns: %d\n", field_cnt);
        
        for (int i=0; i < field_cnt; ++i)
        {
            /* col describes i-th column of the table */
            MYSQL_FIELD *col = mysql_fetch_field_direct(res, i);
            printf ("Column %d: %s\n", i, col->name);
        }
        return 0;

        }
    return 0;

}
