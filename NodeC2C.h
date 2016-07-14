#ifndef _NODE_C2C_H
#define _NODE_C2C_H

class NodeC2C 
{
public:
    
    VRPNeighborNode *neighborNodeExchange; 
	int neighborSizeExchange;
	VRPBeNeighborNode *beNeighborExchange;
	int *neighborOF; // nodeC2C[i].neighborOF[j] = k node i is neighbor kth of node j; if k = -1 that means i is not neighbor of j
	int beNeighborSizeExchange;
           
    // Constructor
    NodeC2C();
    // Destructor
    ~NodeC2C();
};

#endif
