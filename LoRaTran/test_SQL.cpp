//
//  main.cpp
//  test_SQL_cmd
//
//  Created by Keng on 15/01/2017.
//  Copyright © 2017 Keng. All rights reserved.
//

#include <iostream>
#include <mysql.h>
#include <string.h>

using namespace std;
#define DATABASE_USERNAME "root"
#define DATABASE_PASSWORD "jizzbear"
#define DATABASE_NAME "postman"
#define IP "192.168.0.116"
#define PORT 3306

int main(int argc, const char * argv[]) {
    
    MYSQL *mysql1;
    mysql1=mysql_init(NULL);
    int test_case;
    string state;
    string command;
    string data[2];
    string start_point[2];
    string des_point[2];
    string send_str;
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
        cout<<"It is connected to Mysql Server now !"<<endl;
        
        if(mysql_query(mysql1, "Use postman;")){cout<<mysql_errno(mysql1);}
        while(1){
            cout<<endl;
            cout<<"1. select state "<<endl;
            cout<<"2. modifify state"<<endl;
            cout<<"3. Get start_point,des_point"<<endl;
            cout<<"4. send_String";
            cin>>test_case;
            switch (test_case) {
                case 1:
                    if (mysql_query(mysql1, "SELECT state FROM transport")){cout<<mysql_errno(mysql1);}
                    else{
                        MYSQL_RES *result = mysql_store_result(mysql1);
                        if(result==NULL){cout<<mysql_errno(mysql1);}
                        else{
                            int num_fields = mysql_num_fields(result);
                            MYSQL_ROW row;
                            while ((row = mysql_fetch_row(result)))
                            {
                                for(int i = 0; i < num_fields; i++)
                                {
                                    state=row[i];
                                    cout<<state<<endl;
                                }
                            }
                        }
                    }

                    break;
                case 2:
                    
                    cin>>state;
                    command="UPDATE transport SET state = "+state+" where _id=2;";
                    if (mysql_query(mysql1, command.c_str()))
                    {
                        cout<<mysql_errno(mysql1);
                    }else{
                        cout<<"update database to 2"<<endl;
                    }

                    break;
                case 3:
                    if (mysql_query(mysql1, "SELECT start_id,des_id FROM transport")){cout<<mysql_errno(mysql1);}
                    else{
                        MYSQL_RES *result = mysql_store_result(mysql1);
                        if(result==NULL){cout<<mysql_errno(mysql1);}
                        else{
                            int num_fields = mysql_num_fields(result);
                            MYSQL_ROW row;
                            while ((row = mysql_fetch_row(result)))
                            {
                                for(int i = 0; i < num_fields; i++)
                                {
                                    data[i]=row[i];
                                }
                            }
                        }
                    }
                    for(int n=0;n<=1;n++){
                        string command="SELECT longitude,latitude from location where _id="+data[n];
                        
                        if (mysql_query(mysql1,command.c_str())){cout<<mysql_errno(mysql1);}
                        else{
                            MYSQL_RES *result = mysql_store_result(mysql1);
                            if(result==NULL){cout<<mysql_errno(mysql1);
}
                            else{
                                int num_fields = mysql_num_fields(result);
                                MYSQL_ROW row;
                                while ((row = mysql_fetch_row(result)))
                                {
                                    for(int i = 0; i < num_fields; i++)
                                    {
                                        if(n==1){
                                            start_point[i]=row[i];
                                        }else{
                                            des_point[i]=row[i];
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                    case 4:
                    send_str=state+" "+start_point[0]+","+start_point[1]+" "+des_point[0]+","+des_point[1];
                    cout<<send_str<<endl;
                    break;
                default:
                    break;
            }
            
            
        }
    }
    return 0;
}
