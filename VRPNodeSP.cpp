#include "PDS.h"
using namespace std;

VRPNodeSP::VRPNodeSP()
{
    ///
    /// Default constructor for the VRPNodeSP class.
    ///

    x = 0;
    y = 0;
    
    this->unload=0; this->load = 0;
    this->sTW1=-PDS_INFINITY;
    this->eTW1=PDS_INFINITY;
}

VRPNodeSP::~VRPNodeSP()
{
    ///
    /// VRPNodeSP destructor.
    ///
}
