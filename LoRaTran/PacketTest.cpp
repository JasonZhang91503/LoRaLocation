#include<iostream>
using namespace std;

#include"postman_packet.h"
#include"CarMapSystem.h"

//#define NO_CAR_MODE
Coor init,xMax,yMax;

int main(){
    init.x = 121.370889;
    init.y = 24.943544;
    xMax.x = 121.373223;
    xMax.y = 24.944915;
    yMax.x = 121.370456;
    yMax.y = 24.944184;


    CarGpsMapSystem* cgms = CarGpsMapSystem::getInstance(1200,600,init,xMax,yMax);


    Coor ss1,ee1;
    
    ss1.x = 121.371941;
    ss1.y = 24.944261;
    ee1.x = 121.373262;
    ee1.y = 24.944943;


/*
    ss1.x = init.x;
    ss1.y = init.y;
    ee1.x = yMax.x;
    ee1.y = yMax.y;
    */
    Coor ss2 = cgms->gpsToCoordinate(ss1);
    Coor ee2 = cgms->gpsToCoordinate(ee1);

    Coor ss3 = cgms->coordinateToGps(ss2);
    Coor ee3 = cgms->coordinateToGps(ee2);


    printf("ss1.x = %lf,%lf\n",ss1.y,ss1.x);
    printf("ee1.x = %lf,%lf\n",ee1.y,ee1.x);
    printf("ss2.x = %lf,%lf\n",ss2.y,ss2.x);
    printf("ee2.x = %lf,%lf\n",ee2.y,ee2.x);
    printf("ss3.x = %lf,%lf\n",ss3.y,ss3.x);
    printf("ee3.x = %lf,%lf\n",ee3.y,ee3.x);


    cin.get();
}