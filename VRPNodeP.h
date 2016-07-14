#ifndef _VRP_NODEP_H
#define _VRP_NODEP_H


#define MAX_ASSIGNSP_OF_P 10
#define MAX_NUM_CUSP_ASSIGNSP 500

class VRPNodeP 
{
public:
    
    int id;
	double x,y;
    int capacity;
    double sTW, eTW;// Time windows
    double duration; // represents the time required at the node
	//double duration1;//represents for loading time at sp ???need another VRPNodeSP, so don't waste memory since pickup customers and delivery customers don't need duration1
    int numSP; //id of supply point to which this customers is assigned to  
	int sp[MAX_ASSIGNSP_OF_P]; //list of sp that customer is assign to #1:closest, #2:2nd closest....
    double r;        // For polar    
    double theta;    // coordinates
    double angle;
	int cluster;

    VRPNeighborNode *neighborNode;
	int neighborSize;
	VRPBeNeighborNode *beNeighbor;
	int *neighborOF; // node[i].neighborOF[j] = k node i is neighbor kth of node j; if k = -1 that means i is not neighbor of j
	int beNeighborSize;
           
    //double arrival_time;
    
    // Constructor
    VRPNodeP();
    // Destructor
    ~VRPNodeP();

    // Duplication
    void duplicate(VRPNodeP *N);
    void show();
    
};

#endif
