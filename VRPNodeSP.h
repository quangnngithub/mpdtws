#ifndef _VRP_NODESP_H
#define _VRP_NODESP_H

class VRPNodeSP 
{
public:
    
    int id;
	double x,y;
    int capacity;
    double sTW1, eTW1;// Time windows for unload or BOTH
	double sTW2, eTW2;// Time windows for load
    double unload; // represents the time required at the node
	double load;//represents for loading time at sp ???need another VRPNodeSP, so don't waste memory since pickup customers and delivery customers don't need duration1
    
    // Constructor
    VRPNodeSP();
    // Destructor
    ~VRPNodeSP();

    // Duplication
    void duplicate(VRPNodeSP *N);
    void show();
};

#endif
