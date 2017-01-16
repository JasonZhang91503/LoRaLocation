#pragma once

// Return whether the car reaches the destination, return true if the source and the destination
// are closer than the ReachDistance attribute in kilometer.
// Attribute directionInfo will return the degrees of rotation in the unit circle,
// Attribute distance will return the distance between the source and the destination.
// North represents 0.0, East represents 90.0, and so on.
// You should fill Lons and Lats of the source and the destination.
// 這個function有個bug，他不能跨國際換日線，因為它會選擇走經過經度0。
bool isCarReachDestination(double &directionInfo, double &distanceInfo, double reachDistance, double sourceLon, double sourceLat, double destinationLon, double destinationLat);