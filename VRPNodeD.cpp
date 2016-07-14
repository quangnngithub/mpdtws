#include "PDS.h"
#include "VRPNodeD.h" //Quangnn++
using namespace std;

//int PDSneighborNodeSize;

VRPNodeD::VRPNodeD()
{
    ///
    /// Default constructor for the VRPNode class.
    /// Allocates an array of MAX_NEIGHBORLIST_SIZE VRPNeighborElements
    /// for the node.
    ///
    int i;
	int 

    x = 0;
    y = 0;
    
    for(i = 0;i < MAX_NEIGHBORNODE_SIZE_D; i++)
    {
        this->neighborNode[i].id=-1;
		this->neighborNode[i].distance=0;
    }
    neighborSize=0;
	beNeighborSize=0;
    neighborSizeMAX=0;
	beNeighborSizeMAX=0;

   // this->arrival_time=0;
    this->duration=0;
    this->sTW=-PDS_INFINITY;
    this->eTW=PDS_INFINITY;
}

VRPNodeD::~VRPNodeD()
{
    ///
    /// VRPNodeD destructor.
    ///
}
