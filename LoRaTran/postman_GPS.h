

#ifndef NO_CAR_MODE
int getGPSLocation(double &sLon,double &sLat){
	while (1) {
        /* wait for 2 seconds to receive data */
        if (gps_waiting (&myGPS_Data, 2000000)) {
            /* read data */
            if ((rc = gps_read(&myGPS_Data)) == -1) {
                printf("error occured reading gps data. code: %d, reason: %s\n", rc, gps_errstr(rc));
            } else {
                /* Display data from the GPS receiver. */
                if ((myGPS_Data.status == STATUS_FIX) && 
                    (myGPS_Data.fix.mode == MODE_2D || myGPS_Data.fix.mode == MODE_3D) &&
                    !isnan(myGPS_Data.fix.latitude) && 
                    !isnan(myGPS_Data.fix.longitude)) {
                        //gettimeofday(&tv, NULL); EDIT: tv.tv_sec isn't actually the timestamp!
                        printf("latitude: %f, longitude: %f, speed: %f, timestamp: %ld\n", myGPS_Data.fix.latitude, myGPS_Data.fix.longitude, myGPS_Data.fix.speed, myGPS_Data.fix.time); //EDIT: Replaced tv.tv_sec with gps_data.fix.time
						break;
               } else {
                    printf("no GPS data available\n");
                }
            }
        }
		else{
			printf("wait for 2 seconds to receive data again\n");
		}

        unistd::sleep(3);
    }
	
	sLon = myGPS_Data.fix.longitude;
	sLat = myGPS_Data.fix.latitude;
	
	return CAR_OK;
}
#endif