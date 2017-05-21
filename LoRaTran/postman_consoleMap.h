#include <stdio.h>
#include <iostream>

using namespace std;

#define mapclear() printf("\033[H\033[J")
#define mapgotoxy(x,y) printf("\033[%d;%dH", (x), (y))

double Wscale = (double)135/28;
double  Hscale = (double)40/13;

void mapGoTo(int x,int y);

void mapPrint()
{
    int width = 30;
    int height = 15;
    int road = 1;



    mapclear();

    for(int j = 0; j<height;j++){
        for(int i = 0; i < width; i++){
            if(j != 0 && j != (height - 1)){
                if(i!=0 && i!= (width - 1)){
                    cout << "  ";
                }
                else{ cout << "- ";}
            }
            else{
                cout << "- ";
            }
        }
        cout << endl;
    }



    for(int j = 0; j < 11;j++){
       mapgotoxy(1 + road +1 + j , 2 + road*2 + 1);
        for(int i = 0; i < 12; i++ ){
             if(j != 0 && j != 10){
                if(i!=0 && i!= 11){
                    cout << "  ";
                }
                else{ cout << "- ";}
            }
            else{
                cout << "- ";
            }
        }
    }

    for(int j = 0; j < 11;j++){
        mapgotoxy(1 + road +1 + j , 2 + road*2 + 1 + 26);
        for(int i = 0; i < 13; i++ ){
             if(j != 0 && j != 10){
                if(i!=0 && i!= 12){
                    cout << "  ";
                }
                else{ cout << "- ";}
            }
            else{
                cout << "- ";
            }
        }

    }
    return;
}

void mapGoTo(int x,int y){
    mapgotoxy( 15 - (int)(2 + x/Hscale) + 1,3 + (int)(y/Wscale)*2);
}