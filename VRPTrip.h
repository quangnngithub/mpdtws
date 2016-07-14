#ifndef _VRP_TRIP_H
#define _VRP_TRIP_H

#define NO_WS    -1 //trip doesnot have waiting station
#define EMPTY_TRIP -1 //empty trip: having startNode = EMPTY_TRIP


class VRPTrip 
{
public:
	/*
	 * startNodeIdx, endNodeIdx can be differerent from type of node in the trip
	 * - For PICKUP trip:
	 *		startNodeIdx can be DEPOT/SUPPLY POINT/last DELIVERY/last C2C-DELIVERY
	 *		endNodeIdx can be SUPPLY POINT/WAITING STATION
	 * - For DELIVERY trip:
	 *		startNodeIdx only can be SUPPLY POINT
	 *		endNodeIdx can be DEPOT/SUPPLY POINT/WAITING STATION/first PICKUP/first C2C-Pickup
	 * - For C2C trip:
	 *		startNodeIdx can be DEPOT/SUPPLYPOINT/last DELIVERY
	 *		endNodeIdx can be DEPOT/SUPPLY POINT/WAITING STATION/first PICKUP
	 */
	int startNodeIdx, endNodeIdx;

    int assSP, nextSP; 

	/*
	 * 0: pickup trip
	 * 1: delivery trip
	 * 2: C2C
	 */
	int Type;

	double dbUtilization; // for leg worst utilization destroy

	int coordinate;
	
	int waitingS, waitingS1; //waitingS1 used incase needed for connecting sp-sp' in a trip: p - sp - sp' - d
	
	
	int firstCus, lastCus, numCus, capacity;
	
	/*
	 * leaveSP: departure time at startNodeIdx
	 * endS: finished time at endNodeIdx
	 * [leaveSP, endS] covers whole proceesing of trip
	 * - For PICKUP trip:
	 *		endS: include unloading time at endNodeIdx
	 * - For DELIVERY trip:
	 *		leaveSP: has to add loading time at startNodeIdx
	 */
	double leaveSP, endS;

	double duration, arriveSP; //??may don't need startS, endS if in VRPNode have startS, endS
	//for delivery trip: leaveSP is the time at which the vehicle leaves sp;  arriveSP: is the time the vehicle arrive at SP (show up at sp)
	//for pickup trip: leaveSP is the time at which the vehicle finish unloading
	
	double vioTWC, vioTWSP;
	int vioCAP;
	double cost,fitness, connectCost;
	bool feasible;

	/*int nCMEM;

	int hash_val;
	int hash_val2;
	double *x, *y;
	int *ordering;*/


	// Constructor
    VRPTrip();

    // Destructor
    ~VRPTrip();

	VRPTrip(VRPTrip &cTrip);//copy constructor
    VRPTrip &operator = (VRPTrip &cTrip);  //assignment; not copy constructor

};


#endif
