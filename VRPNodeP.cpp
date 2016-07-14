#include "PDS.h"
#include "VRPNodeP.h" //Quangnn++

using namespace std;



VRPNodeP::VRPNodeP()
{
    ///
    /// Default constructor for the VRPNode class.
    /// Allocates an array of MAX_NEIGHBORLIST_SIZE VRPNeighborElements
    /// for the node.
    ///

    x = 0;
    y = 0;
    
    neighborSize=0;
	beNeighborSize=0;

   // this->arrival_time=0;
    this->duration=0;
    this->sTW=-PDS_INFINITY;
    this->eTW=PDS_INFINITY;
}

VRPNodeP::~VRPNodeP()
{
    ///
    /// VRPNodeP destructor.
    ///
}
