#include "PDS.h"
using namespace std;

int PDSneighborNodeSize;

VRPNode::VRPNode()
{
    ///
    /// Default constructor for the VRPNode class.
    /// Allocates an array of MAX_NEIGHBORLIST_SIZE VRPNeighborElements
    /// for the node.
    ///

    x = 0;
    y = 0;
	delID = 0; pickID = 0;

    neighborSize=0;
	beNeighborSize=0;

   // this->arrival_time=0;
    duration=0;
    sTW=-PDS_INFINITY;
    eTW=PDS_INFINITY;
}

VRPNode::~VRPNode()
{
    ///
    /// VRPNode destructor.
    ///
}
