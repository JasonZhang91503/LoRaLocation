#include<iostream>
#include"postman_request.h"

int main(){
    RequestManager manger;
    RequestObserver observer1(1);
    RequestObserver observer2(2);

    manger.addReqestListener(observer1);
    manger.addReqestListener(observer2);
    manger.removeReqestListener(observer2);

    cout << manger.reqListenList.size() << endl;

    UserRequest a,b,c;
    a.packetNum = 0;
    b.packetNum = 1;
    c.packetNum = 2;

    manger.add(a);
    manger.add(b);
    manger.add(c);

    cin.get();

    return 0;
}