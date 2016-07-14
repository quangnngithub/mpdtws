#ifndef _VRP_NODEWS_H
#define _VRP_NODEWS_H


class VRPNodeWS 
{
public:
    
    int id;
	double x,y;
    
    // Constructor
    VRPNodeWS();
    // Destructor
    ~VRPNodeWS();

    // Duplication
    void duplicate(VRPNodeWS *N);
    void show();
};

#endif
