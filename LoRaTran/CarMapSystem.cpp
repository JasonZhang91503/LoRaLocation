#include "CarMapSystem.h"
/*
CarMapSystem::CarMapSystem(int width,int height)
{
    maxHeight = height;
    maxWidth = width;

    //openHeap = new priority_queue<CarMapNode*, vector<CarMapNode*>, cmp>;
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

void CarMapSystem::printCarNodeInfo()
{
    for(int i = 0; i < maxWidth;i++){
        for(int j = 0; j < maxHeight;j++){
            cout << "x:" << carMapNode[i][j].GetCor_x()
                << ",y:"<< carMapNode[i][j].GetCor_y()
                << ",wall=" << carMapNode[i][j].GetisWlakable()<<endl;
        }
    }
}

void CarMapSystem::setRectangleWall(int s_x,int s_y,int e_x,int e_y)
{
    for(int i = s_x;i <= e_x;i++){
        for(int j = s_y;j <= e_y;j++){
            carMapNode[i][j].SetisWlakable(false);
        }
    }
}

void CarMapSystem::setWall(int x,int y)
{
    carMapNode[x][y].SetisWlakable(false);
}

void CarMapSystem::clearWall()
{
    for(int i = 0; i < maxWidth;i++){
        for(int j = 0; j < maxHeight;j++){
            carMapNode[i][j].SetisWlakable(true);
        }
    }
}

vec_CMnode CarMapSystem::findPath(int s_x,int s_y,int e_x,int e_y){
    bool hasPath = run_A_star(s_x,s_y,e_x,e_y);

    vec_CMnode emptyVec;
    if(hasPath){
        return tracePath(e_x,e_y);
    }
    return emptyVec;
}

vec_CMnode CarMapSystem::tracePath(int e_x,int e_y){
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


bool CarMapSystem::run_A_star(int s_x,int s_y,int e_x,int e_y)
{
    bool findNode = false;

    //初始化
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
                //超出範圍
                if(i < 0 || j < 0 || maxWidth <= i || maxHeight <= j){
                    continue;
                }
                //本身不須判別update
                if(i == c_x && j == c_y){
                    continue;
                }
                //已加入closeList不須判別update
                if(closeList[i][j]){
                    continue;
                }
                //若是牆壁則不能走
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
*/

CarGpsMapSystem* CarGpsMapSystem::instance = 0;

