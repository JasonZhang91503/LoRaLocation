#ifndef CARMAPSYSTEM_H
#define CARMAPSYSTEM_H

#include"CarMapNode.h"
#include<queue>
#include <list>
#include<iostream>
#include <algorithm>
#include<stdlib.h>
#include<math.h>
using namespace std;

#define PI 3.14159265

#define MAX_FLOAT 3.40282347e+38

#define ROOT_2 1.41421

#define ROOT_3 1.73205



struct cmp
{
    bool operator() (const CarMapNode* lhs, const CarMapNode* rhs) { return lhs->GetFvalue() > rhs->GetFvalue(); }
};

typedef vector<CarMapNode*> vec_CMnode;
typedef priority_queue<CarMapNode*, vec_CMnode, cmp> pri_queue;

class myPriQueue: public pri_queue{
public:
    vec_CMnode::iterator begin(){
        return pri_queue::c.begin();
    }
    vec_CMnode::iterator end(){
        return pri_queue::c.end();
    }
};

struct Coor{
    double x;
    double y;
};

struct Stronghold{
    int value;
    int x;
    int y;

    bool operator==(const Stronghold &rhs) const {
        return (rhs.value == value);
    }
};

class CarMapSystem
{
    public:
        CarMapSystem(int width,int height){
            maxHeight = height;
            maxWidth = width;

            openHeap = new myPriQueue;

            openList = new bool*[maxWidth];
            closeList = new bool*[maxWidth];
            carMapNode = new CarMapNode*[maxWidth];
            for(int i = 0; i < maxWidth;i++){
                openList[i] = new bool[maxHeight];
                closeList[i] = new bool[maxHeight];
                carMapNode[i] = new CarMapNode[maxHeight];
                for(int j = 0; j < maxHeight;j++){
                    openList[i][j] = false;
                    closeList[i][j] = false;
                    carMapNode[i][j].SetCoordinate(i,j);
                }
            }
        }

        unsigned int GetmaxHeight() { return maxHeight; }
        void SetmaxHeight(unsigned int val) { maxHeight = val; }
        unsigned int GetmaxWidth() { return maxWidth; }
        void SetmaxWidth(unsigned int val) { maxWidth = val; }

        void printCarNodeInfo(){
            for(int i = 0; i < maxWidth;i++){
                for(int j = 0; j < maxHeight;j++){
                    cout << "x:" << carMapNode[i][j].GetCor_x()
                        << ",y:"<< carMapNode[i][j].GetCor_y()
                        << ",wall=" << carMapNode[i][j].GetisWlakable()<<endl;
                }
            }
        }
        void printMap();

        void setRectangleWall(int s_x,int s_y,int e_x,int e_y){
            for(int i = s_x;i <= e_x;i++){
                for(int j = s_y;j <= e_y;j++){
                    carMapNode[i][j].SetisWlakable(false);
                }
            }
        }
        void setWall(int x,int y){
            carMapNode[x][y].SetisWlakable(false);
        }
        void clearWall(){
            for(int i = 0; i < maxWidth;i++){
                for(int j = 0; j < maxHeight;j++){
                    carMapNode[i][j].SetisWlakable(true);
                }
            }
        }

        vec_CMnode findPath(int s_x,int s_y,int e_x,int e_y){
            bool hasPath = run_A_star(s_x,s_y,e_x,e_y);

            vec_CMnode emptyVec;
            if(hasPath){
                return tracePath(e_x,e_y);
            }
            return emptyVec;
        }
        vec_CMnode tracePath(int e_x,int e_y){
            vec_CMnode traceVec;

            traceVec.push_back(&carMapNode[e_x][e_y]);

            int x = e_x;
            int y = e_y;

            CarMapNode* nextNode = carMapNode[x][y].GetfatherNode();

            while(nextNode){
                x = nextNode->GetCor_x();
                y = nextNode->GetCor_y();
                traceVec.push_back(&carMapNode[x][y]);

                nextNode = nextNode->GetfatherNode();
            }

            reverse(traceVec.begin(),traceVec.end());


            return traceVec;
        }

        bool isInsideMap(int x,int y){
            return  0 <= x && x < maxWidth && 0 <= y && y < maxHeight;
        }

CarMapNode** carMapNode;
    protected:
        //CarMapNode** carMapNode;
        unsigned int maxHeight;
        unsigned int maxWidth;
        bool run_A_star(int s_x,int s_y,int e_x,int e_y){
            bool findNode = false;

            //���l��
            for(int i = 0; i < maxWidth; i++){
                for(int j = 0; j < maxHeight; j++){
                    closeList[i][j] = false;
                    openList[i][j] = false;
                    while(!openHeap->empty()){
                        openHeap->pop();
                    }
                    carMapNode[i][j].SetGvalue(0);
                    carMapNode[i][j].SetHvalue(0);
                    carMapNode[i][j].SetfatherNode(NULL);
                }
            }

            addToOpenList(s_x,s_y);
            carMapNode[s_x][s_y].SetGvalue(0);
            //carMapNode[s_x][s_y].SetHvalue(0);

            while(!openHeap->empty()){
                CarMapNode* currentNode = openHeap->top();
                int c_x = currentNode->GetCor_x();
                int c_y = currentNode->GetCor_y();
                removeFromOpenList();

                if(c_x == e_x && c_y == e_y){
                    findNode = true;
                    break;
                }

                for(int i = c_x - 1; i < c_x+2;i++){
                    for(int j = c_y -1; j < c_y+2;j++){
                        //�W�X�d��
                        if(i < 0 || j < 0 || maxWidth <= i || maxHeight <= j){
                            continue;
                        }
                        //���������P�Oupdate
                        if(i == c_x && j == c_y){
                            continue;
                        }
                        //�w�[�JcloseList�����P�Oupdate
                        if(closeList[i][j]){
                            continue;
                        }
                        //�Y�O�����h���ਫ
                        if(!carMapNode[i][j].GetisWlakable()){
                            continue;
                        }

                        if(openList[i][j]){
                            for(vec_CMnode::iterator it=openHeap->begin();
                                it!=openHeap->end();
                                it++)
                            {
                                if((*it)->GetCor_x() == i && (*it)->GetCor_y() == j){
                                    int rG = (*it)->GetGvalue();
                                    int cG = currentNode->GetGvalue();
                                    int dist = isStraightWalk(i,j,c_x,c_y)
                                                ?straightWeight:obliqueWeight;

                                    if(cG + dist< rG){
                                        updateFromOpenList(i,j,cG + dist);
                                        carMapNode[i][j].SetfatherNode(&carMapNode[c_x][c_y]);
                                    }
                                }

                            }

                        }
                        else{
                            addNodeGvalue(i,j,c_x,c_y);
                            setNodeHvalue(i,j,e_x,e_y);
                            carMapNode[i][j].SetfatherNode(&carMapNode[c_x][c_y]);
                            addToOpenList(i,j);

                        }

                    }
                }

            }
            return findNode;
        }
    private:

        //a star �t���k����

        void addToOpenList(int i,int j){
            //List.push_back(&carMapNode[i][j]);
            openHeap->push(&carMapNode[i][j]);
            openList[i][j] = true;
        }
        void removeFromOpenList(){
            //List.erase(&carMapNode[i][j]);
            CarMapNode* currentNode = openHeap->top();
            int c_x = currentNode->GetCor_x();
            int c_y = currentNode->GetCor_y();
            openHeap->pop();
            openList[c_x][c_y] = false;
            closeList[c_x][c_y] = true;
        }
        void updateFromOpenList(int x,int y,int update){
            //priority_queue<CarMapNode*, vector<CarMapNode*>, cmp> *tmpQueue;
            //tmpQueue = new priority_queue<CarMapNode*, vector<CarMapNode*>, cmp>;

            myPriQueue *tmpQueue;
            tmpQueue = new myPriQueue;

            int tmpX,tmpY;
            CarMapNode *tmpNode;

            tmpNode = openHeap->top();
            tmpX = tmpNode->GetCor_x();
            tmpY = tmpNode->GetCor_y();

            //���qopenHeap���F����tmpQueue
            while(!(tmpX == x && tmpY == y)){
                tmpQueue->push(openHeap->top());
                openHeap->pop();

                tmpNode = openHeap->top();
                tmpX = tmpNode->GetCor_x();
                tmpY = tmpNode->GetCor_y();
            }
            //����tmpQueue�̪��F���F pop��
            openHeap->pop();

            //�������Ӯe���j�p  �j�����sopenList
            if(tmpQueue->size() > openHeap-> size()){
                //priority_queue<CarMapNode*, vector<CarMapNode*>, cmp> *swapTmpQueue;
                myPriQueue *swapTmpQueue;
                swapTmpQueue = tmpQueue;
                tmpQueue = openHeap;
                openHeap = swapTmpQueue;
            }

            //��tmpQueue��������openList
            while(!tmpQueue->empty()){
                openHeap->push(tmpQueue->top());
                tmpQueue->pop();
            }
            //�N���wnode�i��update
            carMapNode[x][y].SetGvalue(update);

            //�̫��N���s��node���JopenList
            openHeap->push(&carMapNode[x][y]);
            delete tmpQueue;
        }
        void addNodeGvalue(int x,int y,int c_x,int c_y){
            int c_gv = carMapNode[c_x][c_y].GetGvalue();
            //�����ξ
            if(isStraightWalk(x,y,c_x,c_y)){
                carMapNode[x][y].SetGvalue(c_gv + straightWeight);
            }
            //�ר�
            else{
                carMapNode[x][y].SetGvalue(c_gv + obliqueWeight);
            }
        }
        void setNodeHvalue(int x,int y,int e_x,int e_y){
            int ManhattanCount = abs(e_x - x) + abs(e_y - y);//�n�[������

            carMapNode[x][y].SetHvalue(ManhattanCount * ManhattanValue);
        }
        bool isStraightWalk(int x,int y,int c_x,int c_y){
            //�����ξ
            if(x == c_x || y == c_y){
                return true;
            }
            //�ר�
            else{
                return false;
            }
        }
        list<CarMapNode*> List;
        //priority_queue<CarMapNode*, vector<CarMapNode*>, cmp> *openHeap;
        myPriQueue *openHeap;
        bool** openList;
        bool** closeList;
        unsigned int straightWeight = 10;
        unsigned int obliqueWeight = 14;
        unsigned int ManhattanValue = 10;
};

class CarGpsMapSystem : public CarMapSystem{
public:

    static CarGpsMapSystem* getInstance(int width,int height,Coor init,Coor xMax,Coor yMax){
        if(!instance){
            instance = new CarGpsMapSystem(width,height,init,xMax,yMax);
        }
        return instance;
    }

    CarGpsMapSystem(int width,int height,Coor init,Coor xMax,Coor yMax)
        :CarMapSystem(width,height),origin(init),xMaxCoor(xMax),yMaxCoor(yMax){


            double xDiff = xMaxCoor.x - origin.x;
            double yDiff = xMaxCoor.y - origin.y;
            xTan = yDiff/xDiff;
            yTan = xDiff/(-yDiff);

            xRadin = atan(xTan);
            
            double angle = xRadin * 180 / PI;

            hypotenuse = hypot(xDiff,yDiff);

            Scale = hypotenuse/width;

            for(int i = 0; i < maxWidth;i++){
                for(int j = 0; j < maxHeight;j++){
                    carMapNode[i][j].SetStronghold(-1);
                }
            }
        }

    void addStronghold(Stronghold sh){
        shList.push_back(sh);
    }

    bool findStronghold(int value){
        Stronghold sh;
        sh.value = value;
        list<Stronghold>::iterator findSh = find(shList.begin(), shList.end(), sh);
        if(findSh == shList.end()){
            return false;
        }
        else{
            return true;
        }
    }

    bool removeStronghold(int value){
        Stronghold sh;
        sh.value = value;
        list<Stronghold>::iterator findSh = find(shList.begin(), shList.end(), sh);
        if(findSh == shList.end()){
            return false;
        }
        else{
            shList.erase(findSh);
            return true;
        }
    }

    int getStrongholdNum(){
        return shList.size();
    }


    void fillNodeStronghold(){
        bool loop = true;
        int spread = 1;
        int shSize = shList.size();
        bool* arrCount = new bool[shSize + 1];

        clearStronghold();


        for(int i = 1; i <= shSize; i++){
            arrCount[i] = false;
        }

        for(list<Stronghold>::iterator s = shList.begin();s != shList.end();s++){
            carMapNode[s->x][s->y].SetStronghold(s->value);
        }


        while(loop){
            loop = false;
            for(list<Stronghold>::iterator s = shList.begin();s != shList.end();s++){
                bool flag = false;

                if(arrCount[s->value]){
                    continue;
                }

                for(int i = s->x - spread; i <= s->x + spread; i++){
                    if(i < 0 || maxWidth <= i) continue;

                    //�Y�O�̤U�Ʃγ̤W��
                    if((i != s->x - spread) || (i != s->x + spread)){
                        for(int j = s->y - spread; j <= s->y + spread; j++){
                            if(j < 0 || maxHeight <= j) continue;

                            if(!carMapNode[i][j].GetisWlakable())continue;

                            //if(carMapNode[i][j].GetStronghold() != s->value)continue;

                            bool around = false;
                            for(int m = i-1; m <= i+1;m++){
                                if(m < 0 || maxWidth <= m) continue;
                                for(int n = j - 1; n <= j+1; n++){
                                    if(n < 0 || maxHeight <= n) continue;
                                    if(carMapNode[m][n].GetStronghold() == s->value){
                                        around = true;
                                        break;
                                    }
                                }
                                if(around) break;
                            }
                            if(!around){continue;}

                            if(carMapNode[i][j].GetStronghold() == -1){
                                carMapNode[i][j].SetStronghold(s->value);
                                flag = true;
                                loop = true;
                            }
                        }
                    }
                    else{
                        int spreadDiff[2]={s->y - spread,s->y + spread};
                        for(int k = 0; k < 2; k++){
                            if((spreadDiff[k]) < 0 || maxHeight <= (spreadDiff[k])) continue;

                            if(!carMapNode[i][spreadDiff[k]].GetisWlakable())continue;

                            //if(carMapNode[s->x][s->y].GetStronghold() != s->value)continue;


                            bool around = false;
                            for(int m = i-1; m <= i+1;m++){
                                if(m < 0 || maxWidth <= m) continue;
                                for(int n = k - 1; n <= k+1; n++){
                                    if(n < 0 || maxHeight <= n) continue;
                                    if(carMapNode[m][n].GetStronghold() == s->value){
                                        around = true;
                                        break;
                                    }
                                }
                                if(around) break;
                            }
                            if(!around){continue;}



                            if(carMapNode[i][spreadDiff[k]].GetStronghold() == -1){
                                carMapNode[i][spreadDiff[k]].SetStronghold(s->value);
                                flag = true;
                                loop = true;
                            }
                        }
                    }

                }

                if(!flag){
                    arrCount[s->value] = true;
                }

            }
            spread++;
        }
    }

    void printNodeStronghold(){
        char c;
        for(int j = GetmaxHeight() - 1; j >= 0 ;j--){
            for(int i = 0 ; i < GetmaxWidth();i++){
                int s = carMapNode[i][j].GetStronghold();
                if(s == -1){
                    c = '0';
                }
                else if(1 <= s && s <= 9){
                    c = s + '0';
                }
                else if(10 <= s && s <= 35){
                    c = s - 10 + 'a';
                }
                else if(36 <= s && s <= 61){
                    c = s - 36 + 'A';
                }

                cout << c;

            }
            cout << endl;
        }
    }

    void clearStronghold(){
        for(int i = 0; i < maxWidth;i++){
            for(int j = 0; j < maxHeight;j++){
                carMapNode[i][j].SetStronghold(-1);
            }
        }
    }

    //�̭��]A*�t���k�h�M
    vec_CMnode findPath(Coor startGPS, Coor endGPS,float** adj){
        vec_CMnode traceVec;

        if(!isInsideMap(startGPS.x,startGPS.y) || !isInsideMap(endGPS.x,endGPS.y)){
            printf("cgms : gps not in map\n");
            return traceVec;
        }

        //�NstartNode�PendNode���������y��
        Coor startCoor = gpsToCoordinate(startGPS);
        Coor endCoor = gpsToCoordinate(endGPS);

        //���X��startNode�PendNode�̪���Stronghold
        int s = carMapNode[(int)startCoor.x][(int)startCoor.y].GetStronghold();
        int d = carMapNode[(int)endCoor.x][(int)endCoor.y].GetStronghold();

        cout << "cgms : findPath : s = " << s <<",d = "<< d <<endl;

        if(s <= 0 || 15 < s || d <= 0 || 15 < d){
            printf("cgms : strongHold not in exist\n");
            return traceVec;
        }

        //���X�_�IStronghold�����IStronghold��Stronghold���|
        vector<int> traceStrongholdVec = findStrongholdPath(s,d,adj);

        for(vector<int>::iterator it = traceStrongholdVec.begin(); it != traceStrongholdVec.end();it++){
            cout << *it << " ";
        }
        cout << endl;

        list<Stronghold>::iterator it = shList.begin();
        advance(it, carMapNode[(int)startCoor.x][(int)startCoor.y].GetStronghold() - 1);

        bool hasPath = run_A_star(startCoor.x,startCoor.y,it->x,it->y);

        if(hasPath){
            traceVec = tracePath(it->x,it->y);
            
            cout << "traceStrongholdVec x -> 0";
            for(vec_CMnode::iterator it = traceVec.begin(); it != traceVec.end();it++){
                cout << "x:" << (*it)->GetCor_x()
                        << ",y:"<< (*it)->GetCor_y()
                        <<endl;
            }

        }

        for(int i = 0; i < traceStrongholdVec.size() - 1; i++){
            list<Stronghold>::iterator itS = shList.begin();
            list<Stronghold>::iterator itE = shList.begin();
            advance(itS, traceStrongholdVec[i] - 1);
            advance(itE, traceStrongholdVec[i+1] - 1);

            hasPath = run_A_star(itS->x,itS->y,itE->x,itE->y);
            if(hasPath){
                vec_CMnode tempVec;
                tempVec = tracePath(itE->x,itE->y);

                cout << "traceStrongholdVec "<< traceStrongholdVec[i] -1 << " -> " <<traceStrongholdVec[i+1] -1;
                for(vec_CMnode::iterator it = traceVec.begin(); it != traceVec.end();it++){
                    cout << "x:" << (*it)->GetCor_x()
                            << ",y:"<< (*it)->GetCor_y()
                            <<endl;
                }

                traceVec.insert(traceVec.end(),tempVec.begin(),tempVec.end());
            }
        }

        //�ثe�u�ɯ����̫��@�Ӥ��~�I
        //�γ\�i�H�����ɯ������I�T�����m


        return traceVec;
    }

    vector<int> findStrongholdPath(int s,int d,float** adj){
        float* key = NULL;
        int *parent;

        //�Q�θ�Stronghold���Xall shortest path
        Dijkstra_algorithm(adj, getStrongholdNum(), s, &key, &parent);

        vector<int> traceVec;

        traceVec.push_back(d);
        int stepBack = d;
        if(parent[d] == 0){
            return traceVec;
        }
        do{
            for(vector<int>::iterator it = traceVec.begin(); it != traceVec.end();it++){
                cout << *it << " ";
            }
            cout << endl;

            traceVec.push_back(parent[d]);
            d = parent[d];
        }while(parent[d]);

        reverse(traceVec.begin(),traceVec.end());

        return traceVec;
    }

    void Dijkstra_algorithm(float** adj, int n, int s, float** key, int ** parent) {
        *key = new float[n + 1];	//�Ҧ��I��key��
        bool* isUpdate = new bool[n + 1];	//�����C���I�O�_�Qupdate
        *parent = new int[n + 1];

        for (int i = 1; i <= n; i++) {
            (*key)[i] = MAX_FLOAT;
        }
        for (int i = 1; i <= n; i++) {
            isUpdate[i] = false;
        }

        (*key)[s] = 0;
        (*parent)[s] = 0;

        for (int i = 1; i < n; i++) {
            float min = MAX_FLOAT;
            int u;
            //�M���̤p��key
            for (int j = 1; j <= n; j++) {
                if (isUpdate[j]) {
                    continue;
                }
                if (min >(*key)[j]) {
                    min = (*key)[j];
                    u = j;
                }
            }
            isUpdate[u] = true;

            //���Pkey�۳s���I���s
            for (int j = 1; j <= n; j++) {
                if (!isUpdate[j] && (*key)[u] + adj[u][j] < (*key)[j]) {
                    (*key)[j] = (*key)[u] + adj[u][j];
                    (*parent)[j] = u;
                }
            }
        }
    }

    Coor gpsToCoordinate(Coor gps){
        Coor rotateCoorXMax = rotationGPS(xMaxCoor,origin,-xRadin);
        Coor rotateCoorYMax = rotationGPS(yMaxCoor,origin,-xRadin);
        Coor rotateGPS = rotationGPS(gps,origin,-xRadin);

        long double xRatio = fabs((long double)rotateGPS.x)/(long double)rotateCoorXMax.x;
        long double yRatio = fabs((long double)rotateGPS.y)/(long double)rotateCoorYMax.y;

        Coor coordinate;
        coordinate.x = (long double)maxWidth * xRatio;
        coordinate.y = (long double)maxHeight * yRatio;
        return coordinate;
    }

    Coor coordinateToGps(Coor coor){
        Coor rotateCoorXMax = rotationGPS(xMaxCoor,origin,-xRadin);
        Coor rotateCoorYMax = rotationGPS(yMaxCoor,origin,-xRadin);

        Coor initNode;
        initNode.x = 0;
        initNode.y = 0;

        long double xRatio = (coor.x / (long double)maxWidth);
        long double yRatio = (coor.y / (long double)maxHeight);

        Coor rotatedGPS;
        rotatedGPS.x = xRatio * (long double)rotateCoorXMax.x;
        rotatedGPS.y = yRatio * (long double)rotateCoorYMax.y;

        Coor initGPS;
        initGPS.x = (rotatedGPS.x) * cos(xRadin) - (rotatedGPS.y) * sin(xRadin) + origin.x;
        initGPS.y = (rotatedGPS.x) * sin(xRadin) + (rotatedGPS.y) * cos(xRadin) + origin.y;

        return initGPS;
/*
        Coor initNode;
        initNode.x = 0;
        initNode.y = 0;
        Coor rotateNode = rotationGPS(coor,initNode,xRadin);

        Coor rotateGps;
        rotateGps.x = origin.x + rotateNode.x * ( (xMaxCoor.x - origin.x) / (double)maxWidth);
        rotateGps.y = origin.y + rotateNode.y * ( (yMaxCoor.y - origin.y) / (double)maxHeight);
        return rotateGps;
        */
    }

    Coor rotationGPS(Coor rot,Coor ori,double radin){
        Coor rotateCoor;

        rotateCoor.x = (rot.x - ori.x) * cos(radin) - (rot.y - ori.y) * sin(radin);
        rotateCoor.y = (rot.x - ori.x) * sin(radin) + (rot.y - ori.y) * cos(radin);

        return rotateCoor;
    }
    bool isInsideMap(double gpsX, double gpsY){
        Coor gps = {gpsX,gpsY};
        Coor mapNode = gpsToCoordinate(gps);
        return CarMapSystem::isInsideMap(mapNode.x,mapNode.y);
    }

    double getAngle(){
        return xRadin * 180 / PI;
    }


protected:
private:
    static CarGpsMapSystem* instance;
    double xTan;
    double yTan;
    double hypotenuse;
    double xRadin;
    Coor origin;
    Coor xMaxCoor;
    Coor yMaxCoor;
    double Scale;
    list<Stronghold> shList;
};


CarGpsMapSystem* CarGpsMapSystem::instance = 0;


#endif // CARMAPSYSTEM_H
