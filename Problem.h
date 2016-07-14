#ifndef PROBLEM_H
#define PROBLEM_H

///The Problem class. 
/// Github
class Problem 
{
public: 
  //double AVGARC; //average cost of all arcs
  //double AVGDEMANDP; //average pickup demands
  int nOrder;	//number of orders - |R|	 - Number of requests = number of c2c-pickup customer 
  int nC2C;		//Total number of C2C demands = 2*nOrder
  int nP;		// number of pickup-customer demands
  int nD;		// number of delivery-customer demands
  int nSP;		// |S|	 - Number of suppy points
  int nWS;		//|W|	 - Number of waiting stations
  int g_inFixedCost;
  int nC2C1,nP1, nD1, nSP1, nWS1; //=NP+1; nD+1; nSP+1, nWS+1
  
  class VRPNode *node;		// for all Nodes
  class VRPNodeSP *nodeSP;	// for supply points
  class VRPNodeWS *nodeWS;	// for waiting stations
  class NodeC2C *nodeC2C;	// for c2c nodes
  
  int startidWS;	//Start id of waiting stations
  int endidWS;		//End id of waiting stations
  int startidP;		//Start id of pickup
  int endidP;		//End if of pickup
  int startidD;		//Start id of delivery
  int endidD;		//End id of delivery
  int startidC2C;	//Start id of c2c
  int endidC2C;		//End id of c2c
  int endidC2CP;	//End id of c2c-pickup;
  int startidC2CD;	//Start id of c2c-delivery

  int MAXNUMCUSD;

  int numCusDSP[PDS_MAX_NUM_SUPPLYPOINTS]; //#customersD in each supply points
  int startCusDSP[PDS_MAX_NUM_SUPPLYPOINTS]; //startID of customers in each supply points 
  int endCusDSP[PDS_MAX_NUM_SUPPLYPOINTS]; //endID of customers in each supply points 
  int capDSP[PDS_MAX_NUM_SUPPLYPOINTS]; //total DELIVERY capacity in each suppy point
  int capPSP[PDS_MAX_NUM_SUPPLYPOINTS]; //total pickup capacity in each suppy point
  bool **belong; //belong[i][j] represents whether pickup i is assigned to supply point j or not
  double **startSOneP, **leaveSPOneP;
  bool   **waitingSOneP;
  //bool **sameLeg; //sameLeg[order1][order2] = 1 if order1 and order2 can be assigned to the same vehicle

  int minTripSP[PDS_MAX_NUM_SUPPLYPOINTS]; //minimum numlegs required for each supply point; =up(capSP/Q)
  int minTripCusP; //minimum numTrips required to serve all pickup customer; = up(TOTALCAPPICKUPCUSTOMERS/Q)
  int maxVeh; //maximum number of vehicles;
  int Q; //capacity of vehicles

  int numCusPAvailSP[PDS_MAX_NUM_SUPPLYPOINTS]; //number of pickup demands that can be serviced by a sp
  int **closestPSP; //closestPSP[idSP][#i] = idCusP that is closest #ith to sp idSP 

 // double **soonestC, **latestC; //soonestP[idCus][idSP]: soonest and latest start service time of pickup customer idCus so that it can go directly to sp idSP
 // bool **CSPDirect; //PSPDirect[i][j]=1 if pickcustomer i can (=has chance) go directly to supply point j


  double **leaveSP; //the time to leave sp: leaveSP[i][j], j=0,1,2 --> leaveSP[i][0] = eTW[i] + unloading[i]; leaveSP[i][1] = eTW[i] + loading[i]; leaveSP[i][2] = eTW[i] + unloading[i]+loading[i];
  
  double **c;  //distance matrix

 
  int **CWSSP; //the id of waiting station such that the total distance (C, waiting station, supply point) is minimum, where C is pickup customer or delivery customer or sp	
  double **disCWSSP; //the distance of C_WS_SP where C is pickup customer, delivery customer or sp
  
  ///Constructor of Problem. The argument is used for accessing input instance file
  Problem(char *filename);
  Problem(char *filename1, char *filename2, int num_assSP);

  Problem();
  ~Problem();

  void CalculateAssignC2CToSP_Q(int &count); //calculate each C2C can assign to which SP
  void CalculateAssignC2CToSP(int &count); //calculate each C2C can assign to which SP
  void CalculatePWSSP(); void CalculateDWSSP(); void CalculateSPWSSP(); void CalculateC2CDWSSP();
  void CalculateClosestPSP();
  void CalculateClosestSPOfP(); void CalculateSoonestLatestPD();
  void CalculateWSAssignment_Q();
  void initData_Q(); void	CalSameLegC2C();

  void ReconstructInputData(char *filename); //To validate input data


  void createInstance(char *filename1, char *filename2);
  void createInstance1(char *filename1, char *filename2, int num_assSP);
  void createInstanceC2C_SP4(char *filename1,char *filename2);
  void createInstanceC2C_SP8(char *filename1,char *filename2);
  void DistributeCAP(int TOTALCAP,int numCus, int *capT, int range1, int range2);


  void findNeighborNodes();
  bool sameListSP(int idCus1, int idCus2); //=1 if cusP idCus1 and idCus2 has the same avail sp; 0 otherwise
  void takenNeighborNodes();
  void calstartSOneP_OR_OneC2C();
  void CalculateStartDepart_Depot();
  void checkingData();

};



#endif

