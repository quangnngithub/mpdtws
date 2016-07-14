#ifndef _VRP_SEGMENT_H
#define _VRP_SEGMENT_H


class VRPSegment 
{
public:
    int numTrips;
	VRPTrip Trip[50]; //class VRPTrip *Trip;
	double vioTWC, vioTWSP;
	int vioCAP;
	bool feasible;
	double fitness;
	double cost;

	// Constructor
    VRPSegment();
    // Destructor
    ~VRPSegment();
		
	VRPSegment(VRPSegment &cSeg);//copy constructor
    VRPSegment &operator = (VRPSegment &cSeg);  //assignment; not copy constructor

};

#endif
