//
//  Lora_Positioning.h
//  RSSI_to_distance+positioning
//
//  Created by Keng on 18/10/2016.
//  Copyright © 2016 Keng. All rights reserved.
//
#include <math.h>
#ifndef Lora_Positioning_h
#define Lora_Positioning_h
/*-----------------------
 BsPoint 用以紀錄 基地台位置
 double x:x-axis point
 double y:y-axis point
 ------------------------*/
struct Point{
    double x;
    double y;
    char BsPointID;
};
typedef struct Point Point;
//set up (Base Station & FinalPoint)point in x-axis & y-axis ,and the name;
Point set_Bspoint(double x,double y,char name){
    Point point;
    point.x=x;
    point.y=y;
    point.BsPointID=name;
    
    return point;
    
}

/*-----------------------
 dir_vector 用以紀錄方向向量
 double x:x-dir
 double y:y-dir
 ------------------------*/
struct dir_Vector{
    double x,y;
};
typedef struct dir_Vector dir_Vector;


float norm (dir_Vector p) // get the norm of a vector
{
    return pow(pow(p.x,2)+pow(p.y,2),.5);
}
Point trilateration(Point point1, Point point2, Point point3, float r1, float r2, float r3) {
    Point resultPose;
    //unit vector in a direction from point1 to point 2
    double p2p1Distance = pow(pow(point2.x-point1.x,2) + pow(point2.y-point1.y,2),0.5);
    dir_Vector ex = {(point2.x-point1.x)/p2p1Distance, (point2.y-point1.y)/p2p1Distance};
    dir_Vector aux = {point3.x-point1.x,point3.y-point1.y};
    //signed magnitude of the x component
    double i = ex.x * aux.x + ex.y * aux.y;
    //the unit vector in the y direction.
    dir_Vector aux2 = { point3.x-point1.x-i*ex.x, point3.y-point1.y-i*ex.y};
    dir_Vector ey = { aux2.x / norm (aux2), aux2.y / norm (aux2) };
    //the signed magnitude of the y component
    double j = ey.x * aux.x + ey.y * aux.y;
    //coordinates
    double x = (pow(r1,2) - pow(r2,2) + pow(p2p1Distance,2))/ (2 * p2p1Distance);
    double y = (pow(r1,2) - pow(r3,2) + pow(i,2) + pow(j,2))/(2*j) - i*x/j;
    //result coordinates
    double finalX = point1.x+ x*ex.x + y*ey.x;
    double finalY = point1.y+ x*ex.y + y*ey.y;
    resultPose.x = finalX;
    resultPose.y = finalY;
    return resultPose;
}



#endif /* Lora_Positioning_h */
