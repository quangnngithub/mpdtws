#ifndef _VRP_NODE_H
#define _VRP_NODE_H

//Value of these 2 const have to be greater than #cusP
#define PDS_MAX_NUM_CUSP    2000 
#define MAX_ASSIGNSP_OF_P 10 //each pickuped customer is assigned to maximum 10 sp 
#define MAX_NUM_CUSP_ASSIGNSP 500

//#define MAX_NODE 2000 //maximum number of node P/D; for eg. #PCus = 300; #DCusMAX of all sp = 340 --> MAX_NODE = 340
extern int PDSneighborNodeSize;



class VRPNode 
{
public:
    
    int id;									// NodeId
	double x,y;								//
    int capacity;							// DEMANDS
    double sTW;								// Time windows- READY TIME
	double eTW;								// Time windows- DUE TIME
    double duration;						// represents the time required at the node - SERVICE TIMES
	double depart;							// Departure time = eTW + duration - the lastest time vehicle can be at the node

	double startS_depot, depart_depot;

	int delID;//this variable is used for C2CP: id of delivery place
	int pickID;//this variable is used for C2CD: id of pickup place

	//double duration1;//represents for loading time at sp ???need another VRPNodeSP, so don't waste memory since pickup customers and delivery customers don't need duration1
    int zone; //id of supply point to which this customers is assigned 
	
	//FOR PICKUP CUSTOMERS:
	int numSP; //id of supply point to which this customers is assigned to  
	int sp[MAX_ASSIGNSP_OF_P]; //list of sp that customer is assign to 

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
    VRPNode();
    // Destructor
    ~VRPNode();

    // Duplication
    void duplicate(VRPNode *N);
    void show();
    
};

#endif
