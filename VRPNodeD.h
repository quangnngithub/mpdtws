#ifndef _VRP_NODED_H
#define _VRP_NODED_H

#define MAX_NEIGHBORNODE_SIZE_D		1000 //Quangnn++ for compiling

//Value of these 2 const have to be greater than #cusP
#define PDS_MAX_NUM_CUSP    2000 
//#define MAX_NODE 2000 //maximum number of node P/D; for eg. #PCus = 300; #DCusMAX of all sp = 340 --> MAX_NODE = 340
extern int PDSneighborNodeSize;

class VRPNodeD 
{
public:
    
    int id;
	double x,y;
    int capacity;
    double sTW, eTW;// Time windows
    double duration; // represents the time required at the node
	//double duration1;//represents for loading time at sp ???need another VRPNodeSP, so don't waste memory since pickup customers and delivery customers don't need duration1
    int sp; //id of supply point to which this customers is assigned 
	//int spID;

    double r;        // For polar    
    double theta;    // coordinates
    double angle;
	int cluster;

    VRPNeighborNode *neighborNode; 
	int neighborSize;
	VRPBeNeighborNode *beNeighbor;
	int *neighborOF; // node[i].neighborOF[j] = k node i is neighbor kth of node j; if k = -1 that means i is not neighbor of j
	int beNeighborSize;

	int neighborSizeMAX;		//Quangnn++ for compiling
	int beNeighborSizeMAX;		//Quangnn++ for compiling

           
    //double arrival_time;
    
    // Constructor
    VRPNodeD();
    // Destructor
    ~VRPNodeD();

    // Duplication
    void duplicate(VRPNodeD *N);
    void show();
    
};

#endif
