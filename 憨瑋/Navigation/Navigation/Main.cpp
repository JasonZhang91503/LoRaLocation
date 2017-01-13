#include "Navigation.h"
#include <iostream>

using namespace std;

int main() {
	double directionInfo, distanceInfo, reachDistance;
	double sLon, sLat, dLon, dLat;
	bool isCarReach;

	do {
		// test data
		// 0.001 24.943783 121.369794 24.943905 121.370041
		cin >> reachDistance >> sLon >> sLat >> dLon >> dLat;
		isCarReach = isCarReachDestination(directionInfo, distanceInfo, reachDistance, sLon +10, sLat + 10, dLon + 10, dLat+ 10);
		
		if (isCarReach) {
			cout << "Finish!" << endl;
		}else {
			cout << "Keep going!" << endl;
		}
		
		cout << "You should still go toward "<< directionInfo << " degree for " << distanceInfo << " kilometer." << endl;
	} while ( -180 < sLon && sLon < 180);


	system("PAUSE");
	return 0;
}