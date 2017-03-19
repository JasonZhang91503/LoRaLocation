#include<queue>
#include<list>
#include<iostream>
using namespace std;

struct UserRequest{
	int packetNum;
    int state;
    double src_lon;
	double src_lat;
	double dest_lon;
	double dest_lat;
	string packetKey;

	bool operator==( const UserRequest &other ) const  {
		if(this->packetNum != other.packetNum){return false;}
		return true;
	}
};

class RequestListener{
public:
	virtual void RequestAdded(UserRequest *req) = 0;
	virtual UserRequest* RequestPop() = 0;
};

class RequestObserver : public RequestListener{
public:

	RequestObserver(int id){
		this->id = id;
	}

	void RequestAdded(UserRequest *req){
		cout << "Observer(" << id << ") add request :" << req->packetNum << endl;
		reqList.push_front(req);
	}
	UserRequest* RequestPop(){
		cout << "Observer(" << id << ") remove request :" << reqList.back()->packetNum << endl;
		UserRequest *popedRequest = reqList.back();
		reqList.pop_back();
		return popedRequest;
	}

	bool HasRequest(){
		return !reqList.empty();
	}

	bool operator==( const RequestObserver &other ) const  {
		return this->id == other.id;
	}

private:
	int id;
	list<UserRequest*> reqList;
};

class RequestManager{
public:
	void addReqestListener(RequestObserver *listener){
		reqListenList.push_back(listener);
	}
	void removeReqestListener(RequestObserver *listener) {
        reqListenList.remove(listener);
    }
	
	void notifyAdded(UserRequest *req) {
        for(RequestListener *listener : reqListenList) {
            listener->RequestAdded(req);
        }
    }
	void add(UserRequest *req) {
        notifyAdded(req);
    }

//private:
	int curNum;
	list<RequestObserver*> reqListenList;
};