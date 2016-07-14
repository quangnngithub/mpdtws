#include "PDS.h"
using namespace std;

VRPSegment::VRPSegment()
{
	numTrips=0;
	//Trip = new VRPTrip[PDS_MAX_NUM_TRIPSVEH];
	vioCAP=0;
	vioTWC=0; vioTWSP=0;
	feasible=1;
	fitness=0;
	cost=0;
}


VRPSegment::~VRPSegment()
{
	//cout<<"Delete trip"<<endl;
	//delete [] Trip;
	//cout<<"FINI VERPSEGMENT DESTROY"<<endl;
}

VRPSegment::VRPSegment(VRPSegment &cSeg) //copy constructor
{
	//cout<<"Copy constructor Segment: "<<endl;
	numTrips = cSeg.numTrips;
	//Trip = new VRPTrip[PDS_MAX_NUM_TRIPSVEH];
    for (int i=0; i<numTrips;i++) Trip[i] = cSeg.Trip[i];

	vioCAP = cSeg.vioCAP;
	vioTWC=cSeg.vioTWC; vioTWSP=cSeg.vioTWSP;
	feasible = cSeg.feasible;
	fitness = cSeg.fitness;
    cost = cSeg.cost;
}

VRPSegment & VRPSegment::operator = (VRPSegment &cSeg)  //assignment; not copy constructor
{
	if (this == &cSeg) return *this;
	(*this).numTrips = cSeg.numTrips;
    for (int i=0; i<numTrips;i++) (*this).Trip[i] = cSeg.Trip[i];
	//(*this).vio_TW = cSeg.vio_TW;
    (*this).vioCAP = cSeg.vioCAP;
	(*this).vioTWC = cSeg.vioTWC;
	(*this).vioTWSP = cSeg.vioTWSP;
	(*this).feasible = cSeg.feasible;
	(*this).fitness = cSeg.fitness;
	(*this).cost = cSeg.cost;
	return *this; 

}

