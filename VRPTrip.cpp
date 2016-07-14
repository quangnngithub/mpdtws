#include "PDS.h"
#include "randvals.h"    // included for hashing
using namespace std; 



VRPTrip::VRPTrip()
{
    assSP = 0; nextSP = 0;
	Type = 1; //=0 pickup trip, =1: delivery trip
	coordinate = 0;
	waitingS = -1; waitingS1 = -1; //waitingS1 used incase needed for connecting sp-sp' in a trip: p - sp - sp' - d
	firstCus = -1; lastCus = -1; numCus = 0; capacity = 0;
	leaveSP = 0; endS= 0; duration = 0; arriveSP= 0; //??may don't need startS, endS if in VRPNode have startS, endS
	//for delivery trip: leaveSP is the time at which the vehicle leaves sp;  arriveSP: is the time the vehicle arrive at SP (show up at sp)
	//for pickup trip: leaveSP is the time at which the vehicle finish unloading
	vioTWC = 0; vioTWSP = 0;
	vioCAP = 0;
	cost = 0; fitness = 0; connectCost =0;
	feasible = 1;    
}

VRPTrip::VRPTrip(VRPTrip &cTrip) //copy constructor
{
	//cout<<"GOI COPY CONSTRUCTOR"<<endl;
	assSP=cTrip.assSP;
	nextSP=cTrip.nextSP;
	Type = cTrip.Type; coordinate = cTrip.coordinate;
	waitingS = cTrip.waitingS;
	waitingS1 = cTrip.waitingS1;
	firstCus= cTrip.firstCus;
	lastCus= cTrip.lastCus;
    numCus= cTrip.numCus;
	capacity=cTrip.capacity;
	leaveSP=cTrip.leaveSP;endS=cTrip.endS; duration = cTrip.duration;
	arriveSP = cTrip.arriveSP;
	vioTWC = cTrip.vioTWC; vioTWSP = cTrip.vioTWSP;
    vioCAP = cTrip.vioCAP;
	cost = cTrip.cost; fitness = cTrip.fitness; connectCost = cTrip.connectCost;
	feasible = cTrip.feasible;
	/*if (cTrip.x)
	{
		//cout<<"???????????????"<<endl;
		for (int i=1;i<=nCMEM;i++) 
		{
			x[i] = cTrip.x[i];
			y[i] = cTrip.y[i];
			ordering[i] = cTrip.ordering[i];
		}
	}*/
}

VRPTrip & VRPTrip::operator = (VRPTrip &cTrip)  //assignment; not copy constructor
{	//cout<<"GOI ASSIGNMENT"<<endl;
	if (this == &cTrip) return *this; 
	//cout<<"Assignment tripP: "<<endl;
	(*this).Type = cTrip.Type;
	(*this).coordinate = cTrip.coordinate;

	(*this).waitingS = cTrip.waitingS;
	(*this).waitingS1 = cTrip.waitingS1;
	(*this).firstCus= cTrip.firstCus;
	(*this).lastCus= cTrip.lastCus;
	(*this).numCus= cTrip.numCus;
    (*this).capacity= cTrip.capacity;
	(*this).leaveSP = cTrip.leaveSP;
	(*this).arriveSP = cTrip.arriveSP;
	(*this).endS = cTrip.endS;
	(*this).duration = cTrip.duration;
	//(*this).vio_TW = cLeg.vio_TW;
	(*this).vioTWC = cTrip.vioTWC;
	(*this).vioTWSP = cTrip.vioTWSP;
	(*this).vioCAP = cTrip.vioCAP;
	(*this).cost = cTrip.cost;
	(*this).fitness = cTrip.fitness;
	(*this).connectCost = cTrip.connectCost;
	(*this).feasible = cTrip.feasible;
	(*this).assSP=cTrip.assSP; 
	(*this).nextSP = cTrip.nextSP; 

	/*if (cTrip.x)
	{
		for (int i=1;i<=nCMEM;i++) 
		{
			(*this).x[i] = cTrip.x[i];
			(*this).y[i] = cTrip.y[i];
			(*this).ordering[i] = cTrip.ordering[i];
		}
	}*/
    return *this; 
}


VRPTrip::~VRPTrip()
{
}

