#ifndef _SOLUTION_H
#define _SOLUTION_H

#include "Problem.h"

using namespace std;

#define MAX_VEH 300

typedef struct variationTQ *VariationptrTQ; //for varation TongQuat move on evaluatemove
struct variationTQ
{
  double var_cost;  //var_cost = var_costj + var_costNeigbor;
  double var_vioTWC, var_vioTWSP;
  double var_fitness;
  int var_vioCAP;
  short atIT;
  bool empty;

  variationTQ()
  {
      var_cost = 0; 
      var_fitness = 0;
      var_vioCAP = 0;
      var_vioTWC = 0;
	  var_vioTWSP=0;
	  empty=0;
  }

  variationTQ & operator=( const variationTQ& v)
  {
    if (&v == this) return *this;
    var_cost = v.var_cost;
	var_fitness = v.var_fitness;
    var_vioCAP = v.var_vioCAP;
    var_vioTWC = v.var_vioTWC;
	var_vioTWSP = v.var_vioTWSP;
	empty = v.empty;
	atIT = v.atIT;
    return *this;
  }
    
    ~variationTQ(){}
  
};

struct moveT
{
    VariationptrTQ obj;       //pointer to the cost associated to the move
    int idCus1, idCus2;            //index of customer to move
	int idVeh1, idVeh2;            //index of vehicle correspond to idCus1 and idCus2
	int idNeighbor;  //#idNeighbor of customer idCus1
	bool tabu;
    bool feasible;
    int type;
	int atIT;
  
    moveT(){}

    moveT & operator=(const moveT & m)
    {
       if (this ==&m) return *this;
	   atIT = m.atIT;
       idCus1 = m.idCus1;
	   idCus2 = m.idCus2;
	   idVeh1 = m.idVeh1;
	   idVeh2 = m.idVeh2;
	   idNeighbor = m.idNeighbor;
	   tabu = m.tabu;
	   feasible = m.feasible;
	   type = m.type;
	   *obj = *m.obj;      
       return *this;
    }
};


typedef struct variationDD *VariationptrDD; //for deleteCus
struct variationDD
{
  double var_cost;
  double var_vioTWC, var_vioTWSP;
  int atIT; //Nov1: also use for decide if do the move or not; set atIT=-100 if delete customer make #legs served the sp < minrequired --> don't delete this customer
  int var_vioCAP;
  bool empty;

  variationDD()
  {
      var_cost = 0; 
      var_vioCAP = 0;
      var_vioTWC = 0;
	  var_vioTWSP=0;
	  empty=0;
	  atIT=-1;
  }

  variationDD & operator=( const variationDD & v)
  {
    if (&v == this) return *this;
    var_cost = v.var_cost;
    var_vioCAP = v.var_vioCAP;
    var_vioTWC = v.var_vioTWC;
	var_vioTWSP = v.var_vioTWSP;
	empty = v.empty;
	atIT=v.atIT;
    return *this;
  }
    
    ~variationDD(){}
  
};


typedef struct variationI *VariationptrI; //for insertCus
struct variationI
{
  double var_cost, var_vioTWC, var_vioTWSP;
  int var_vioCAP, atIT;

  variationI()
  {
      var_cost = 0; 
      var_vioCAP = 0;
      var_vioTWC = 0;
	  var_vioTWSP=0;
	  atIT = 0;
  }

  variationI & operator=( const variationI& v)
  {
    if (&v == this) return *this;
    var_cost = v.var_cost;
    var_vioCAP = v.var_vioCAP;
    var_vioTWC = v.var_vioTWC;
	var_vioTWSP = v.var_vioTWSP;
	atIT = v.atIT;
    return *this;
  }
    
    ~variationI(){}
  
};


typedef struct variationIE *VariationptrIE; //for insertCusEmptyVeh
struct variationIE
{
  double var_cost, var_vioTWC;

  variationIE()
  {
      var_cost = 0; 
      var_vioTWC = 0;
  }

  variationIE & operator=( const variationIE& v)
  {
    if (&v == this) return *this;
    var_cost = v.var_cost;
    var_vioTWC = v.var_vioTWC;
	return *this;
  }
    
    ~variationIE(){}
  
};


typedef struct variationT *VariationptrT; //for twoOPTCus
struct variationT
{
  double var_cost, var_vioTWC, var_vioTWSP;
  int type;
  int var_vioCAP;
  int atIT;
//  bool isON; 
  bool empty;

  variationT()
  {
	  type = 10000;
      var_cost = 0; 
      var_vioCAP = 0;
      var_vioTWC = 0;
	  var_vioTWSP=0;
	  empty=0;
	  atIT = -2;
  }

  variationT & operator=( const variationT& v)
  {
    if (&v == this) return *this;
	type = v.type;
    var_cost = v.var_cost;
    var_vioCAP = v.var_vioCAP;
    var_vioTWC = v.var_vioTWC;
	var_vioTWSP = v.var_vioTWSP;
	atIT =v.atIT;
	empty = v.empty;
	//isON = v.isON;
    return *this;
  }
    
    ~variationT(){}
  
};


typedef struct variationEx *VariationptrEx; //for exchange customer only
struct variationEx
{
  double var_cost;  //var_cost = var_costj + var_costNeigbor;
  double var_costi, var_costj;
  double var_vioTWC, var_vioTWSP;
  double var_vioTWCi, var_vioTWCj;
  double var_vioTWSPi, var_vioTWSPj;
  int var_vioCAP;
  int atIT;
  //bool isON;

  variationEx()
  {
	  var_cost = 0; 
      var_costi = 0;var_costj = 0;
	  var_vioCAP = 0;
      var_vioTWC = 0; 
	  var_vioTWCi = 0; var_vioTWCj = 0;
	  var_vioTWSP=0;
	  var_vioTWSPi = 0; var_vioTWSPj = 0;
	  atIT = -2;
  }

  variationEx & operator=( const variationEx& v)
  {
	cout<<"CALL THIS ASSIGNMENT?"<<endl;  
    if (&v == this) return *this;
    var_cost = v.var_cost;
    var_costi = v.var_costi; var_costj = v.var_costj;
    var_vioCAP = v.var_vioCAP;
    var_vioTWC = v.var_vioTWC;
	var_vioTWCi = v.var_vioTWCi; var_vioTWCj = v.var_vioTWCj;

	var_vioTWSP = v.var_vioTWSP;
	var_vioTWSPi = v.var_vioTWSPi; var_vioTWSPj = v.var_vioTWSPj;
	//isON = v.isON;
	atIT = v.atIT;
    return *this;
  }
    
    ~variationEx(){}
  
};

typedef struct variationT1 *VariationptrT1; //for 2opt*
struct variationT1
{
  double var_cost;  //var_cost = var_costj + var_costNeigbor;
  double var_costi, var_costj;
  double var_vioTWC, var_vioTWSP;
  double var_vioTWCi, var_vioTWCj;
  double var_vioTWSPi, var_vioTWSPj;
  int var_vioCAP;
  int atIT;
  bool empty;
  //bool isON;

  variationT1()
  {
	  var_cost = 0; 
      var_costi = 0;var_costj = 0;
	  var_vioCAP = 0;
      var_vioTWC = 0; 
	  var_vioTWCi = 0; var_vioTWCj = 0;
	  var_vioTWSP=0;
	  var_vioTWSPi = 0; var_vioTWSPj = 0;
	  atIT = -2;
	  empty = 0;
  }

  variationT1 & operator=( const variationT1& v)
  {
    if (&v == this) return *this;
    var_cost = v.var_cost;
    var_costi = v.var_costi; var_costj = v.var_costj;
    var_vioCAP = v.var_vioCAP;
    var_vioTWC = v.var_vioTWC;
	var_vioTWCi = v.var_vioTWCi; var_vioTWCj = v.var_vioTWCj;

	var_vioTWSP = v.var_vioTWSP;
	var_vioTWSPi = v.var_vioTWSPi; var_vioTWSPj = v.var_vioTWSPj;
	empty = v.empty;
	atIT = v.atIT;
    return *this;
  }
    
    ~variationT1(){}
  
};

typedef struct variationSP *VariationptrSP;
struct variationSP
{
  double var_cost;  //var_cost = var_costj + var_costNeigbor;
  double var_vioTWSP;
  double var_vioTWC;
  bool empty;
  bool doIT;
  int atIT;
  int type;

  variationSP()
  {
      var_cost = 0;
	  var_vioTWSP=0;
	  var_vioTWC = 0;
	  empty=0;
	  doIT=0;
	  atIT=0;
	  type = 0;
  }

  variationSP & operator=( const variationSP& v)
  {
    if (&v == this) return *this;
    var_cost = v.var_cost;
	var_vioTWSP = v.var_vioTWSP;
	var_vioTWC = v.var_vioTWC;
	empty = v.empty;
	doIT = v.doIT;
	atIT = v.atIT;
	type = v.type;
    return *this;
  }
    
    ~variationSP(){}
  
};


typedef struct variationSPCAP *VariationptrSPCAP;
struct variationSPCAP
{
  double var_cost;  //var_cost = var_costj + var_costNeigbor;
  double var_costD;
  double var_vioTWSP;
  double var_vioTWCD;
  double var_vioTWC;
  int atIT,type;
  int var_vioCAP;
  bool empty;
  bool doIT;

  variationSPCAP()
  {
      var_cost = 0;
	  var_costD = 0; 
	  var_vioTWSP=0;
	  var_vioTWC =0;
	  var_vioTWCD = 0;
	  var_vioCAP = 0;
	  empty=0;
	  doIT=0;
	  atIT=0;
	  type=0;
  }

  variationSPCAP & operator=( const variationSPCAP & v)
  {
    if (&v == this) return *this;
    var_cost = v.var_cost;
	var_costD = v.var_costD; 
	var_vioTWSP = v.var_vioTWSP;
	var_vioTWC = v.var_vioTWC;
	var_vioTWCD = v.var_vioTWCD;
	var_vioCAP = v.var_vioCAP;
	empty = v.empty;
	doIT = v.doIT;
	atIT = v.atIT;
	type = v.type;
    return *this;
  }
    
   ~variationSPCAP(){}
  
};

typedef struct variationSPEmptyVeh *VariationptrSPEmptyVeh;
struct variationSPEmptyVeh
{
  double var_cost;  //var_cost = var_costj + var_costNeigbor;
  double var_costD;
  double var_vioTWSP;
  double var_vioTWC;
  int atIT;

  variationSPEmptyVeh()
  {
      var_cost = 0;
	  var_costD = 0; 
	  var_vioTWSP=0;
	  var_vioTWC =0;
	  atIT=0;
  }

  variationSPEmptyVeh & operator=( const variationSPEmptyVeh & v)
  {
    if (&v == this) return *this;
    var_cost = v.var_cost;
	var_costD = v.var_costD; 
	var_vioTWSP = v.var_vioTWSP;
	var_vioTWC = v.var_vioTWC;
	atIT = v.atIT;
    return *this;
  }
    
    ~variationSPEmptyVeh(){}

};


class Solution
{
	///
	/// Contains fields and methods to process solutions to the VRP.
	///

public:
   Solution();	
   Solution(Problem* ); //Construct with a pointer to problem data
  ~Solution();
	
    //double AVGARC;
	int nP, nD, nSP, nWS;			// # of non-DEPOT nodes in the solution
    double cost, fitness;		// objective function value
    int vioCAP;
	double vioTWC, vioTWSP;
	bool feasible;

	 // Solution storage
    int *nextArr,*predArr;
	double *startS, *depart;
	
    int *route_num;
	int *trip_num;
	int *pos; //position of customer in leg: e.g. Leg: 5, 1, 8 -->pos[5]=1; pos[1]=2, pos[8]=3
    bool *routed;            // Indicates whether the customer is in a route yet or not
   
	int *numCusPRouted, *SPToCusP; //SPToCusP[idCus] = idsp that is assigned to
	int *numCusPSP; //numCusPSP[idsp] = number of cusP assigned to idsp
	int **CusPSP; // CusPSP[sp][j] = id of customerP #j assigned to sp; j = 1,..,numCusPSP[sp] 
 
	int *numCusDRouted; //numCusDRouted[i] = #deliveryCustomer routed in customer zone i
    int numVeh, numVehUsed; //number of vehicle used
	class VRPSegment *seg;

	//int *numTripPSP;//count number of pickup trips assinged to each sp in the solution
	int *numTripDSP; //count number of delivery trips assinged to each sp in the solution
	int numTripP; //number of pickup trips in the solution
	int numTrips; //number of trips in the solution

	double *FvioTWCus, *BvioTWCus, *vioTWCus; //FvioTWCusD: accumulated vioTWCus
	int *FCAP, *BCAP;

	//bool in_IP;		// Flag to tell if the solution has been added to the IP before
	//double time;	// time at which the solution was first discovered
	//int hash(int salt);	

  //Utility functions.
  Solution(Solution &csol);//copy constructor
  Solution &operator = (Solution &csol);  //assignment; not copy constructor
  Problem* getProblem();
  void init(Problem *p);

  //------------------------------------ TABU ---------------------------------------------//
  void callTABU(int max_iteration);
  void init_parametersTABU_CUS(int nType);
  void init_parametersTABU_SP();
  void init_arrayTABU();
  bool TabuMAIN1(int max_iteration, Solution &solBEST);
  bool TabuMAIN(int max_iteration, Solution &solBEST);
  int Difference(int idElite);


  void update_solutionCusD(moveT best_move);
  void update_solutionCusP(moveT best_move);
  void update_solutionSP(moveT best_move);

  void update_parameters();


  void Update_deleteCusArrayD_FirstTrip_Empty(int idVeh);
  void Update_deleteCusArrayD_MiddleTrip_Empty(int idVeh, int idTrip); //update deleteCusD for middle delivery trip (idVeh, idTrip) with only 1 delivery customer
  void Update_deleteCusArrayD_FirstMiddleTrip(int idVeh, int idTrip); //only applied for the case where vehicle has >= 2 trips
  void Update_deleteCusArrayD_LastTrip_Empty(int idVeh, int idTrip);
  void Update_deleteCusArrayD_LastTrip(int idVeh, int idTrip);

  void Update_deleteCusArrayP_FirstTrip_Empty(int idVeh);
  void Update_deleteCusP_FirstTrip_2Cus(int idVeh);
  void Update_deleteCusArrayP_MiddleTrip_Empty(int idVeh, int idTrip); //update deleteCusP for middle delivery trip (idVeh, idTrip) with only 1 pickup customer
  void Update_deleteCusArrayP_FirstMiddleTrip(int idVeh, int idTrip); //only applied for the case where vehicle has >= 2 trips
  void Update_deleteCusArrayP_LastTrip_Empty(int idVeh, int idTrip);
  void Update_deleteCusArrayP_LastTrip(int idVeh, int idTrip);


  //Copy Update_deleteCusArrayP_FirstTrip_Empty
  bool cal_deleteCusArrayP_FirstTrip_Empty_TillTrip(int idVeh, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int update_idTrip);
 
   //i belongs to the first cus where there is only one customer --> delete i, this trip becomes empty
  //then insert i after j on the second trip --> after empty the first trip, this second trip becomes the first trip
  void cal_deleteCusArrayP_FirstTrip_Empty_insertNextTrip(int j, int i, int idVeh, double &varcosti, double &varvioTWCi, double &varvioTWSPi, double &varcostj, double &varvioTWCj, double &varvioTWSPj, int &varvioCAP);
  
  //Copy Update_deleteCusP_FirstTrip_2Cus:
  bool cal_deleteCusP_FirstTrip_2Cus_TillTrip(int idVeh, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP,int &varvioCAP, double &cStartS, int update_idTrip);
  //Copy Update_deleteCusArrayP_FirstMiddleTrip:
  bool cal_deleteCusArrayP_FirstMiddleTrip_TillTrip(int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP, double &cStartS,int update_idTrip);
  void cal_deleteCusArrayP_MiddleTrip_FromBeginning(int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP, double cStartS);

  //Copy Update_deleteCusArrayP_MiddleTrip_Empty:
  bool cal_deleteCusArrayP_MiddleTrip_Empty_TillTrip(int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int update_idTrip);
  void cal_deleteCusArrayP_MiddleTrip_Empty_FromBeginning(int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP,int &varvioCAP, double cStartS);

  //Copy Update_deleteCusArrayP_LastTrip:
  void cal_deleteCusArrayP_LastTrip(int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP);
  void cal_deleteCusArrayP_LastTrip_FromBeginning(int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP, double cStartS);

 //COPY Update_deleteCusArrayP_LastTrip_Empty:
  void cal_deleteCusArrayP_LastTrip_Empty(int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP);

  
  void GoThroughVehicle_FirstPT(int idVeh,  int curT, double &varcost, double &varvioTWC, double &varvioTWSP);
  void GoThroughVehicle_FirstPTBUON(int idVeh,  int curT, double &varcost, double &varvioTWC, double &varvioTWSP);

  bool GoThroughVehicle_FirstPT(int idVeh, int curT, int numTrips, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS);

  void GoThroughVehicle(int idVeh, int curT, double &varcost, double &varvioTWC, double &varvioTWSP, double cStartS, int preC, int curC);
  bool GoThroughVehicle(int idVeh, int curT, int numTrips, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int preC, int curC);
  bool GoThroughVehicleBUON(int idVeh, int curT, int numTrips, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int preC, int curC);



  bool calChangeCONNECTAtSP_PickTripBUON(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost, bool &vioTWSPTRUE);

  //update cStartS, varvioTWSP and varcost when connecting id1 and assSP of delivery trip (idVeh, idTrip): where id1 is cus or sp;
  //return 1 if don't need to calculate anymore; 0 otherwise
  bool calCONNECTCSPTOSPD_FORDELETE(int id1, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost);
  bool calChangeCONNECTCSPTOSPD_FORDELETE(int id1, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost);

 //calculte change in varcost, cStartS, varvioTWSP when connecting last delivery cus lastC of (idVeh, preT) with assSP of (idVeh, idTrip):
  bool calChangeCONNECTAtSP_DD(int lastC, int idVeh, int idTrip, int preT, double &cStartS, double &varvioTWSP, double &varcost);
  bool calChangeCONNECTAtSP_DD_LastCChanged(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost);
  bool calChangeCONNECTAtSP_DD_LastCChanged(int lastCOLD, int lastC, int idVeh, int curT, int preT, int varvioCAP, double varvioTWC, double varcost); //for deleteCusD procedure


  bool updateChangeCONNECTAtSP_DD(int lastC, int idVeh, int idTrip, int preT, double &cStartS); //for procedure UpdateVehicle
  bool updateChangeCONNECTAtSP_DD(int lastC, int idVeh, int idTrip, int preT, int varvioCAP, double varvioTWC, double varcost); //for deleteCusD procedure
  bool updateChangeCONNECTAtSP_DD(int lastC, int idVeh, int idTrip, int preT, int varvioCAP, double varvioTWC, double costTrip, double costOLD); //for twoOPTstarCusDTrip
  void updateChangeCONNECTAtSP_DD(int idVeh, int nextTrip, int preT, int varvioCAP); //for Update_EmptyTripD and Update_EmptyTripP: connect deliverytrip(idVeh, preT) with delivery trip (idVeh, nextTrip) where (idVeh, idTrip) is already deleted 
  void updateChangeCONNECTAtSP_DD_TillTrip(int idVeh, int nextTrip, int preT, int varvioCAP, int update_idTrip); //for delete_insertCusPTrip: connect deliverytrip(idVeh, preT) with delivery trip (idVeh, nextTrip) where (idVeh, idTrip) is already deleted 

  //both procedures will double the value of varvioTWSP in case nextTrip is delivery trip and sameSP:
  bool calChangeCONNECTAtSP_PickTrip(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost); 
  bool calChangeCONNECTAtSP_PickTrip(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost, bool &vioTWSPTRUE); 
  bool calChangeCONNECTAtSP_PickTrip2(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost); //dont check if there is a case p-dSAMESP as in the calChangeCONNECTAtSP_PickTrip
  void calChangeCONNECTAtSP_PickTrip2_1(int lastC, int idVeh, int idTrip, double &cStartS, double &vioTWSPNEW, double &varcost); //dont check if there is a case p-dSAMESP as in the calChangeCONNECTAtSP_PickTrip; used for MoveSP

  bool calChangeCONNECTAtSP_PickTrip_LastCChanged(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost);
  bool calChangeCONNECTAtSP_PickTrip_LastCChanged(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost, bool &vioTWSPTRUE);
  bool calChangeCONNECTAtSP_PickTrip_LastCChanged2(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost);//dont check if there is a case p-dSAMESP as in calChangeCONNECTAtSP_PickTrip_LastCChanged

  bool calChangeCONNECTAtSP_PickTrip(int lastC, int idVeh, int idTrip, double &cStartS); //for procedure UpdateVehicle

  bool calChangeCONNECTAtSP_SPSP(int idVeh, int idTrip, int preT, double &cStartS, double &varvioTWSP, double &varcost);
  bool calChangeCONNECTAtSP_SPSP_MoveSP(int idVeh, int idTrip, int assSP, double &cStartS, double &varvioTWSP, double &varcost);

  void GoThroughVehicle_InsertPTBUON(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double & varvioTWC, double &varvioTWSP);
  void GoThroughPickupTripBUON(int idVeh, int idTrip, int preC, int curC, double &cStartS, double &varvioTWC, double &vioTWSPNEW, double &varcost);
  void GoThroughVehicle_CombinePickupSucBUON(int idVehI, int preT, bool lastT, int nextT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);



  bool updateChangeCONNECTAtSP_SPSP(int idVeh, int idTrip, int preT, double &cStartS); //for procedure UpdateVehicle
  void updateChangeCONNECTAtSP_SPSP(int idVeh, int nextTrip, int preT, int varvioCAP); //for Update_EmptyTripD and Update_EmptyTripP: connect pickuptrip(idVeh, preT) with delivery trip (idVeh, nextTrip) where (idVeh, idTrip) is already deleted 
  void updateChangeCONNECTAtSP_SPSP_TillTrip(int idVeh, int nextTrip, int preT, int varvioCAP, int update_Trip); //for delete_insertCusPTrip: connect pickuptrip(idVeh, preT) with delivery trip (idVeh, nextTrip) where (idVeh, idTrip) is already deleted 

  void cal_InsertCusDIntoEmptyVeh(int idCus, int startSP);
  void cal_InsertCusDIAfterJ_LastC_LastTrip(int j, int k, int idVeh, int idTrip);
  void cal_InsertCusDIAfterJ_NotLastC_LastTrip(int j, int k, int idVeh, int idTrip);
  void cal_InsertCusDIAfterJ_LastC_NextTripP(int j, int k, int idVeh, int idTrip);
  void cal_InsertCusDIAfterJ_NotLastC_NextTripP(int j, int k, int idVeh, int idTrip);
  void cal_InsertCusDIAfterJ_LastC_NextTripD(int j, int k, int idVeh, int idTrip);
  void cal_InsertCusDIAfterJ_NotLastC_NextTripD(int j, int k, int idVeh, int idTrip);

  void cal_MoveCusDIAfterJ_oneTrip_LastTrip(int j, int k, int idVeh, int idTrip);
  void cal_MoveCusDIAfterJ_oneTrip_NextTripP(int j, int k, int idVeh, int idTrip);
  void cal_MoveCusDIAfterJ_oneTrip_NextTripD(int j, int k, int idVeh, int idTrip);


  bool SUBcal_InsertCusPIAfterJ_NotLastC(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_NotLastC1(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_NotLastC1(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, bool &vioTWSPTRUE); //Used for the case p-dSAMESP
  bool SUBcal_InsertCusPIAfterJ_NotLastC1_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_NotLastC1_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, bool &vioTWSPTRUE); //Used for the case p-dSAMESP
  void cal_InsertCusPIAfterJ_NotLastC_NextTripP(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_InsertCusPIAfterJ_NotLastC_NextTripD_SameSP(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_NotLastC_NextTripD_DifSP(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_NotLastC_NextTripP_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_InsertCusPIAfterJ_NotLastC_NextTripD_SameSP_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_NotLastC_NextTripD_DifSP_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  
  bool SUBcal_InsertCusPIAfterJ_LastC(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_LastC1(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &vioTWSPNEW, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_LastC1(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &vioTWSPNEW, double &cStartS, bool &vioTWSPTRUE); 
  bool SUBcal_InsertCusPIAfterJ_LastC1_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_LastC1_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, bool &vioTWSPTRUE); //Used for the case p-dSAMESP
  bool SUBcal_InsertCusPIAfterJ_LastC_1CusFirstTrip(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_LastC_1CusFirstTrip(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, bool &vioTWSPTRUE); //Used for the case p-dSAMESP

  void cal_InsertCusPIAfterJ_LastC_NextTripP(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_InsertCusPIAfterJ_LastC_NextTripP_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_InsertCusPIAfterJ_LastC_NextTripP_1CusFirstTrip(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_InsertCusPIAfterJ_LastC_NextTripD_SameSP(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_LastC_NextTripD_SameSP_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_LastC_NextTripD_SameSP_1CusFirstTrip(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_LastC_NextTripD_DifSP(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_LastC_NextTripD_DifSP_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_LastC_NextTripD_DifSP_1CusFirstTrip(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);

  //Copy SUBcal_InsertCusPIAfterJ_LastC_1CusFirstTrip:
  bool cal_InsertCusPIAfterJ_LastC_1CusFirstTrip_TillTrip(int j, int i, int idVeh, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int update_idTrip); 
  //Insert j after i on (idVeh, idTripj) where idTripj may first/middle/lastTrip; then update remaining trips till update_idTrip
  //If idTripj is the first trip, then it has >= 2 customers
   //COPY SUBcal_InsertCusPIAfterJ_LastC and SUBcal_InsertCusPIAfterJ_NotLastC
   bool cal_InsertCusPIAfterJ_OneVeh_TillTrip(int j, int i, int idVeh, int idTripj, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int update_idTrip);


  bool SUBcal_MoveCusPIAfterJ_oneTrip(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS);
  bool SUBcal_MoveCusPIAfterJ_oneTrip(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, bool &vioTWSPTRUE);
  void cal_MoveCusPIAfterJ_oneTrip_LastTrip(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS);
  void cal_MoveCusPIAfterJ_oneTrip_NextTripP(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_MoveCusPIAfterJ_oneTrip_NextTripD_SameSP(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_MoveCusPIAfterJ_oneTrip_NextTripD_DifSP(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP);


  //Move cus i on (idVeh, idTripi) after cus j on (idVeh, idTripj):
  void cal_MoveCusPIAfterJ_oneVeh(int j, int idTripj, int i, int idTripi, int idVeh, double &varcost, double &varcosti, double &varcostj, double &varvioTWC, double &varvioTWCi, double &varvioTWCj, double &varvioTWSP, double &varvioTWSPi, double &varvioTWSPj, int &varvioCAP);
  //idTripj > 0:
  void cal_InsertCusPIAfterJ_FromBeginning(int j, int i, int idVeh, int idTripj,int preT, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double cStartS);


  void cal_ExchangeCusDIAndJ(int j, int k);
  void cal_ExchangeCusPIAndJ(int j, int k);

  void cal_twoOPTstarCusDIAndJ_oneTrip_LastTrip(int j, int k); //connect j with i where j and i are in the same trip
  void cal_twoOPTstarCusDIAndJ_oneTrip_NextTripD(int j, int k); //connect j with i where j and i are in the same trip
  void cal_twoOPTstarCusDIAndJ_oneTrip_NextTripP(int j, int k); //connect j with i where j and i are in the same trip
  void cal_twoOPTstarCusDIAndJ(int j, int k); //connect j with i where j and i are in different trips

  bool SUBcal_twoOPTstarCusPIAndJ_oneTrip(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); //connect j with i where j and i are in the same trip
  bool SUBcal_twoOPTstarCusPIAndJ_oneTrip(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, bool &vioTWSPTRUE); //connect j with i where j and i are in the same trip
  void cal_twoOPTstarCusPIAndJ_oneTrip_LastTrip(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); //connect j with i where j and i are in the same trip
  void cal_twoOPTstarCusPIAndJ_oneTrip_NextTripP(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_twoOPTstarCusPIAndJ_oneTrip_NextTripD_SameSP(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_twoOPTstarCusPIAndJ_oneTrip_NextTripD_DifSP(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_twoOPTstarCusPIAndJ_oneTrip(int j, int k);
  void cal_twoOPTstarCusPIAndJ(int j, int k); //where j and its neighbor #k belong to different vehicles

  void CallEvaluate(int type);
  void evaluate_moveCusD(moveT move);
  void evaluate_moveCusP(moveT move);
  void evaluate_moveSP(moveT move);
  void evaluate_moveSP_EmptyVeh(moveT move);
  void evaluate_exchangeSP(moveT move);


  int CreateNewVeh_DTrip(int idCus);
  void deleteCusDTrip(int fromVeh, int fromTrip, int idCus);
  void insertCusDTrip(int insert_after, int IDinsert, int idVeh, int idTrip); //insert IDinsert after insert_after: note that insert_after is always a customer, so don't need tocheck if it is startSP or endSP or not
  void moveCusDTrip(int j, int i, int idVeh, int idTrip); //move customer i from its current place to place after j: both i and j are at the same trip
  void exchangeCusDTrip(int j, int i); //exchange customer j and its neighbor i: in 1 trip OR 2 trips
  void SUBexchangeCusDTrip(int idVeh, int idTrip, int prei, int suci, int i, int j, double varcost); //replace i by j
  void twoOPTstarCusDTrip_oneTrip(int j, int i, int idVeh, int idTrip); //2opt* between 2 customer j and its neighbor (i) where j and i are in the same trip
  void twoOPTstarCusDTrip(int j, int idVehj, int idTripj, int i, int idVehi, int idTripi); //2opt* between 2 customer j and its neighbor (i) where j and i belong to different trips
  void moveCusPTrip_OneTrip(int j, int i, int idVeh, int idTrip); //move customer i from its current place to place after j: both i and j are at the same trip
  void UpdateChangeCONNECTAtSP_PickTrip(int idVeh, int idTrip, double varcost, double varcostpreT, double varvioTWC, int varvioCAP=0); //for  moveCusPTrip_OneTrip
  void UpdateChangeCONNECTAtSP_PickTrip(int idVeh, int idTrip, double varcostpreT, double varvioTWC, int varvioCAP, double costTrip); //for twoOPTstarCusPTrip
  
  void moveCusPTrip_OneVeh(int j, int toTrip, int i, int fromTrip, int fromVeh);//move cus i on (fromVeh, fromTrip) after cus j on (fromVeh, toTrip) where i and j are on the same vehicle fromVeh

  void deleteCusPTrip(int idVeh, int idTrip, int pre, int idCus, int next); //for the trip >= 2customers
  void deleteCusPTrip_FromBeginning(int idVeh, int idTrip, int pre, int idCus, int next); //for the trip >= 2customers
  void insertCusPTrip(int insert_after, int IDinsert, int idVeh, int idTrip); //insert IDinsert after insert_after: note that insert_after is always a customer, so don't need tocheck if it is startSP or endSP or not
  void insertCusPTrip_FromBeginning(int insert_after, int IDinsert, int idVeh, int idTrip); //insert IDinsert after insert_after: note that insert_after is always a customer, so don't need tocheck if it is startSP or endSP or not
  void insertCusPTrip_TillTrip(int insert_after, int IDinsert,int idVeh, int idTrip, int update_idTrip);	//insert IDinsert after insert_after on (idVeh, idTrip) and update remaining trips till trip update_idTrip
  void UpdateChangeCONNECTAtSP_PickTrip_TillTrip(int idVeh, int idTrip, double varcost, double varcostpreT, double varvioTWC, int varvioCAP, int update_idTrip); //for insertCusPTrip_TillTrip and deleteCusPTrip_TillTrip
  void deleteCusPTrip_TillTrip(int idVeh, int idTrip, int idCus, int update_idTrip); //delete cus idCus from (idVeh, idTrip) where idTrip >= 2 customers; and update remaining trips till trip update_idTrip	
  void delete_insertCusPTrip(int i, int idTripi, int j, int idTripj, int idVeh); //empty idTripi which is before idTripj, (NOT NECCESSARY RIGHT BEFORE), then insert i after j on trip idTripj, then DON TOA and update all remaining trips of idVeh

  void replaceCusPTrip(int pre, int IDReplaced, int suc, int IDinsert, int lastCOLD, int idVeh, int idTrip); //insert IDinsert between pre and suc (delete the cus IDReplaced between pre and suc): if pre < 0 that means insert IDinsert at the beginning of the trip, and then update all remaining trips in the vehicle idVeh
  void replaceCusPTrip_FromBeginning(int pre, int IDReplaced, int suc, int IDinsert, int lastCOLD, int idVeh, int idTrip); //insert IDinsert between pre and suc (delete the cus IDReplaced between pre and suc): if pre < 0 that means insert IDinsert at the beginning of the trip, and then update all remaining trips in the vehicle idVeh
  
  void replaceCusPTrip_TillTrip(int pre, int IDReplaced, int suc, int IDinsert, int lastCOLD, int idVeh, int idTrip, int update_idTrip); //insert IDinsert between pre and suc (delete the cus IDReplaced between pre and suc): if pre < 0 that means insert IDinsert at the beginning of the trip, and then update all remaining trips in the vehicle idVeh
  void exchangeCusPTrip_OneTrip(int j, int i, int idVeh, int Trip); //exchange cus i and j on (idVeh, idTrip)
  void exchangeCusPTrip_OneVeh(int idCusj, int idTripj, int idCusi, int idTripi, int idVeh); //exchange cus idCusj on (idVeh, idTripj) with cus idCusi on (idVeh, idTripi) of the same vehicle idVeh
  void twoOPTstarCusPTrip_oneTrip(int j, int i, int idVeh, int idTrip); //2opt* between 2 customer j and its neighbor (i) where j and i are in the same trip
  void twoOPTstarCusPTrip(int j, int idVehj, int idTripj, int i, int idVehi, int idTripi); //2opt* between 2 customer j and its neighbor (i) where j and i belong to different trips
 
  bool Update_EmptyTripP(int idVeh, int idTrip);
  bool Update_EmptyTripD(int idVeh, int idTrip);
  bool Update_EmptyTripPD(int idVeh, int idTrip);

 void UpdateMoveSPArray_MoveOut(int idVeh);
 void UpdateMoveSPArray_MoveIn(int idVeh);
 void UpdateMoveSPArray_MoveOut_Except(int idVeh, int idVehE);
 void UpdateMoveSPArray_MoveIn_Except(int idVeh, int idVehE);
 void UpdateMoveSPArray(int idVeh1, int idVeh2);
 void UpdateMoveSPArray(int idVeh);
 void UpdateMoveSPArrayEmptyVeh(int idVeh);

 void UpdateExchangeSPArray(int idVeh);

 void GoThroughVehicle_InsertDT(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double & varvioTWC, double &varvioTWSP);  
 void GoThroughVehicle_InsertDT_ExchangeSP(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double & varvioTWC, double &varvioTWSP);  
 void GoThroughVehicle_InsertDT_EmptyVeh_ExchangeSP(int idVehI, int idVehM, int idTripM, double &varcost, double & varvioTWC, double &varvioTWSP);  
 void GoThroughDeliveryTrip(int lastC, int preC, int curC, double &cStartS, double &varvioTWC);

 void GoThroughVehicle_InsertPT(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double & varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_InsertPT_ExchangeSP(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double & varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_InsertPT_EmptyVeh_ExchangeSP(int idVehI, int idVehM, int idTripM, double &varcost, double & varvioTWC, double &varvioTWSP);
 void GoThroughPickupTrip(int idVeh, int idTrip, int preC, int curC, double &cStartS, double &varvioTWC, double &vioTSPNEW, double &varcost);
 void GoThroughVehicle_InsertPDTSAMESP(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double & varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_InsertPDTSAMESP_ExchangeSP(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double & varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_InsertPDTSAMESP_EmptyVeh_ExchangeSP(int idVehI, int idVehM, int idTripM, double &varcost, double & varvioTWC, double &varvioTWSP);
 void GoThroughPDTripSAMESP(int idVeh, int idTrip, int preC, int curC, double &cStartS, double &varvioTWC, double &varvioTWSP, double &varcost);
 void GoThroughFirstPickupTrip(int idVeh, int idTrip, double &cStartS, double &varvioTWC, double &vioTWSPNEW, double &varcost);

 void GoThroughVehicle_CombinePickupPre(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_CombinePickupSuc(int idVehI, int preT, bool lastT, int nextT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);
 
 void GoThroughVehicle_CombinePickupPre_NotAlterPre_ExchangeSP(int idVehI, int preT, int assSP, int nextT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_CombinePickupPre_NotAlterPre_ExchangeSP_PD(int idVehI, int preT, int assSP, int nextT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_CombinePickupPre_AlterPre_ExchangeSP(int idVehI, int preT, int curT, int assSP, int nextT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_CombinePickupSuc_AlterPre_ExchangeSP(int idVehI, int preT, int curT, int assSP, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);
 
 void GoThroughVehicle_CombineDeliveryPre(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_CombineDeliveryPre_AlterPre_ExchangeSP(int idVehI, int preT, int curT, int assSP, int nextT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_CombineDeliveryPre_NotAlterPre_ExchangeSP(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);

 void GoThroughVehicle_CombineDeliverySuc(int idVehI, int curT, int assSP, bool lastT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_CombineDeliverySuc_AlterPre_ExchangeSP(int idVehI, int preT, int curT, int assSP, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_CombineDeliverySuc_AlterPre_ExchangeSP_PD(int idVehI, int preT, int curT, int assSP, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);

 void GoThroughVehicle_DeleteTrip(int idVeh, int idTrip, double &varcost, double & varvioTWC, double &varvioTWSP);
 void GoThroughVehicle_DeletePDTrip(int idVeh, int idTrip, double &varcost, double & varvioTWC, double &varvioTWSP);

 void updateMoveSP_InsertDeliveryTrip(int idVehM, int idTripM, int idVehI, double varcost, double varvioTWC, double varvioTWSP, bool empty);
 void updateMoveSP_InsertPickupTrip(int idVehM, int idTripM, int idVehI, double varcost, double varvioTWC, double varvioTWSP, bool empty);
 void updateMoveSP_InsertPDTrip(int idVehM, int idTripM, int idVehI, double varcost, double varvioTWC, double varvioTWSP, bool empty);
 void updateMoveSP_InsertDeliveryTrip_EmptyVeh(int idVehM, int idTripM);
 void updateMoveSP_InsertPickupTrip_EmptyVeh(int idVehM, int idTripM);
 void updateMoveSP_InsertPDTrip_EmptyVeh(int idVehM, int idTripM);

 //28/11/2013:
 void updateExchangeSP_InsertDeliveryTrip(int idVehI, int preT, int nextT, int idVehM, int idTripM, double &varcost, double &varvioTWC, double &varvioTWSP);

 //21/11/2013
 bool moveSPVeh(int assSPM, int idVehM, int assSPI, int idVehI);//move assSPM of idVehM after assSPI of idVehI
 
 void InsertDeliveryTrip(int idVehM, int idTripM, int assSPM, int idVehI, int assSPI); //insert (idVehM, idTripM) after (idVehI,idTripI) 
 void InsertDeliveryTrip_ExchangeSP(int idVeh, int idTripI, int idTripM); 
 void subInsertDeliveryTrip(int idVeh, int idTripI, int nextT, double cStartS, double varcostpreT);
 void UpdateVehicle_CONNECTAtSP_DD_InsertDT(int idVeh, int nextT, int idTripI, double varcostpreT); //for subInsertDeliveryTrip
 void UpdateVehicle_CONNECTAtSP_DD_InsertDT(int idVehI, int preTI, int nextTI); //insert (idVehM, idTripM) between delivery trip(idVehI, preTI) and trip nextTI
 void UpdateVehicle_CONNECTAtSP_PD_InsertDT(int idVehI, int preTI, int nextTI); //insert (idVehM, idTripM) between pickup trip(idVehI, preTI) and trip nextTI
 void insertCusDArrayPre(int idVehI, int idTripI, int idVehM, int idTripM); //assSPI - d + (assSPM - d);COPY twoOPTstarCusDTrip
 void insertCusDArraySuc(int idVehI, int idTripI, int idVehM, int idTripM); //assSPM - d + (assSPI - d)

 void InsertPickupTrip(int idVehM, int idTripM, int assSPM, int idVehI, int assSPI); //insert pickup (idVehM, idTripM) after (idVehI,idTripI) 
 void InsertPickupTrip_ExchangeSP(int idVeh, int idTripI, int idTripM); 

 void subInsertPickupTrip(int idVeh, int idTripI, int nextT, int idTripM, double cStartS, int preC, double varcostpreT);
 void insertCusPArrayPre(int idVehI, int idTripI, int idVehM, int idTripM); //assSPI - p + (assSPM - p);COPY twoOPTstarCusPTrip
 void insertCusPArraySuc(int idVehI, int idTripI, int idVehM, int idTripM); //assSPM - p + (assSPI - p)
 void calChangeCONNECTAtSP_PickTrip2(int lastC, int idVeh, int idTrip, double cStartS); //for procedure subInsertPickupTrip
 void UpdateVehicle_CONNECTAtSP_SPSP_InsertPT(int idVeh, int nextT,int idTripI, double varcostpreT);//for subInsertPickupTrip

 void InsertPDTrip(int idVehM, int idTripM, int assSPM, int idVehI, int assSPI); //insert P-D trip (idVehM, idTripM, idTripM+1) after (idVehI,idTripI) 
 void subInsertPDTrip(int idVeh, int idTripI, int nextT, int idTripM, double cStartS, int preC, double varcostpreT);
 
 void InsertPDTrip_ExchangeSP(int idVeh, int idTripI, int idTripM); //insert P-D trip (idVeh, idTripI, idTripI+1) between (idVehI,preT) and (idVeh, nextT) 
 void exchangeSPVeh(int assSP1, int idVeh1, int assSP2, int idVeh2); // exchange sp assSP1 on idVeh1 with assSP2 in idVeh2

 //29/11/2013
 void sub_ExchangeSP_DEmptyDFirst(int idVehi, int si, int idVehj); //idVehj has >= 2 trips
 void sub_ExchangeSP_DEmptyPFirst(int idVehi, int si, int idVehj);//idVehj has >= 2 trips
 void sub_ExchangeSP_DEmptyPDFirst(int idVehi, int si, int idVehj); //idVehj has >= 3 trips
 void sub_ExchangeSP_DEmptyDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT);
 void sub_ExchangeSP_DEmptyPMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT);//SAME AS sub_ExchangeSPDEmptyDMiddle
 void sub_ExchangeSP_DEmptyPDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj);//SAME AS sub_ExchangeSPDEmptyDMiddle
 void sub_ExchangeSP_DEmptyPDLast(int idVehi, int si, int idVehj, int preT, int &idTripj);
 void sub_ExchangeSP_DEmptyDLast(int idVehi, int si, int idVehj, int idTripj);
 void sub_ExchangeSP_DEmptyPLast(int idVehi, int si, int idVehj, int idTripj);

 void sub_ExchangeSP_PEmptyDFirst(int idVehi, int si, int idVehj); //idVehj has >= 2 trips
 void sub_ExchangeSP_PEmptyPFirst(int idVehi, int si, int idVehj);//idVehj has >= 2 trips
 void sub_ExchangeSP_PEmptyPDFirst(int idVehi, int si, int idVehj);//idVehj has >= 3 trips
 void sub_ExchangeSP_PEmptyDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT);
 void sub_ExchangeSP_PEmptyPMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT); //SAME AS sub_ExchangeSPPEmptyDMiddle
 void sub_ExchangeSP_PEmptyPDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj);//SAME AS sub_ExchangeSPPEmptyPMiddle,sub_ExchangeSPPEmptyDMiddle
 void sub_ExchangeSP_PEmptyPDLast(int idVehi, int si, int idVehj, int preT, int &idTripj);
 void sub_ExchangeSP_PEmptyDLast(int idVehi, int si, int idVehj, int idTripj);
 void sub_ExchangeSP_PEmptyPLast(int idVehi, int si, int idVehj, int idTripj);


 void sub_ExchangeSP_PDEmptyDFirst(int idVehi, int si, int idVehj); //idVehj has >= 2 trips
 void sub_ExchangeSP_PDEmptyPFirst(int idVehi, int si, int idVehj);//idVehj has >= 2 trips
 void sub_ExchangeSP_PDEmptyPDFirst(int idVehi, int si, int idVehj);//idVehj has >= 3 trips

 void sub_ExchangeSP_PDEmptyDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT); //SAME AS sub_ExchangeSPPDEmptyPMiddle, sub_ExchangeSPPDEmptyPDMiddle
 void sub_ExchangeSP_PDEmptyPMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT); //SAME AS sub_ExchangeSPPDEmptyDMiddle
 void sub_ExchangeSP_PDEmptyPDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj);//SAME AS sub_ExchangeSPPDEmptyPMiddle,sub_ExchangeSPPDEmptyDMiddle
 void sub_ExchangeSP_PDEmptyPDLast(int idVehi, int si, int idVehj, int preT, int &idTripj);
 void sub_ExchangeSP_PDEmptyDLast(int idVehi, int si, int idVehj, int idTripj);
 void sub_ExchangeSP_PDEmptyPLast(int idVehi, int si, int idVehj, int idTripj);

 void sub_ExchangeSP_DFirstDFirst(int idVehi, int si, int idVehj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_DFirstDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT); //COPY sub_ExchangeSP_DEmptyDMiddle
 void sub_ExchangeSP_DFirstPMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT); //COPY sub_ExchangeSP_DEmptyPMiddle
 void sub_ExchangeSP_DFirstPDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj); //COPY sub_ExchangeSP_DEmptyPDMiddle

 void sub_ExchangeSP_PFirstDFirst(int idVehi, int si, int idVehj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_PFirstPFirst(int idVehi, int si, int idVehj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_PFirstPDFirst(int idVehi, int si, int idVehj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_PFirstDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT);  //COPY ::sub_ExchangeSP_PEmptyDMiddle
 void sub_ExchangeSP_PFirstPMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT);  //COPY ::sub_ExchangeSP_PEmptyPMiddle
 void sub_ExchangeSP_PFirstPDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj);  //COPY ::sub_ExchangeSP_PEmptyPDMiddle
 void sub_ExchangeSP_PFirstDLast(int idVehi, int si, int idVehj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_PFirstPLast(int idVehi, int si, int idVehj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_PFirstPDLast(int idVehi, int si, int idVehj, int sj); //idVehi has >= 2 trips; idVehj >= 3 trips

 void sub_ExchangeSP_PDFirstDFirst(int idVehi, int si, int idVehj, int sj); //idVehi has >= 3 trips; idVehj >= 2 trips
 void sub_ExchangeSP_PDFirstDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT);  //COPY ::sub_ExchangeSP_PDEmptyDMiddle
 void sub_ExchangeSP_PDFirstPMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj, int nextT);  //COPY ::sub_ExchangeSP_PDEmptyPMiddle
 void sub_ExchangeSP_PDFirstPDMiddle(int idVehi, int si, int idVehj, int preT, int &idTripj);  //COPY ::sub_ExchangeSP_PDEmptyPDMiddle
 void sub_ExchangeSP_PDFirstDLast(int idVehi, int si, int idVehj, int sj); //idVehi has >= 3 trips; idVehj >= 2 trips
 void sub_ExchangeSP_PDFirstPDLast(int idVehi, int si, int idVehj, int sj); //idVehi has >= 3 trips; idVehj >= 2 trips
 void sub_ExchangeSP_PDFirstPLast(int idVehi, int si, int idVehj, int sj); //idVehi has >= 3 trips; idVehj >= 2 trips

 void sub_ExchangeSP_DLastDFirst(int idVehi, int si, int idVehj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_DLastDLast(int idVehi, int idTripi, int si, int idVehj, int idTripj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_DLastPLast(int idVehi, int idTripi, int si, int idVehj, int idTripj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_DLastPDLast(int idVehi, int idTripi, int si, int idVehj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_DLastDMiddle(int idVehi, int idTripi, int si, int idVehj, int preT, int &idTripj, int nextT);  

 void sub_ExchangeSP_PLastDFirst(int idVehi, int si, int idVehj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips
 void sub_ExchangeSP_PLastPLast(int idVehi, int idTripi, int si, int idVehj, int idTripj, int sj); //idVehi has >= 2 trips; idVehj >= 2 trips

 void sub_ExchangeSP_PDLastDFirst(int idVehi, int si, int idVehj, int sj); //idVehi has >= 3 trips; idVehj >= 2 trips



 void cal_ExchangeSPType1(int idVeh); //vehicle has only one delivery trip 
 void cal_ExchangeSPType2(int idVeh);//vehicle has only one pickup trip
 void cal_ExchangeSPType3(int idVeh); //vehicle has only one P-D trip
 void cal_ExchangeSPType4(int idVeh); //exchange first delivery trip, nextT is delivery trip 
 void cal_ExchangeSPType5(int idVeh); //exchange first delivery trip, nextT is pickup trip 
 void cal_ExchangeSPType6(int idVeh); //exchange first P-D trip, nextT is delivery trip 
 void cal_ExchangeSPType7(int idVeh); //exchange first P-D trip, nextT is pickup trip 
 void cal_ExchangeSPType8(int idVeh); //exchange first pickup, nextT is delivery trip 
 void cal_ExchangeSPType9(int idVeh); //exchange first pickup, nextT is pickup trip 
 void cal_ExchangeSPType10(int idVeh, int idTrip, int preT, int nextT); //exchange middle delivery trip, preT and nextT is delivery
 void cal_ExchangeSPType11(int idVeh, int idTrip, int preT, int nextT); //exchange middle delivery trip, preT is delivery, nextT is pickup
 void cal_ExchangeSPType12(int idVeh, int idTrip, int preT, int nextT); //exchange middle delivery trip, preT is pickup and nextT is delivery
 void cal_ExchangeSPType13(int idVeh, int idTrip, int preT, int nextT); //exchange middle delivery trip, preT is pickup, nextT is pickup
 void cal_ExchangeSPType14(int idVeh, int idTrip, int preT, int nextT1); //exchange middle P-D trip, preT is delivery and nextT1 is delivery
 void cal_ExchangeSPType15(int idVeh, int idTrip, int preT, int nextT1); //exchange middle P-D trip, preT is delivery and nextT1 is pickup
 void cal_ExchangeSPType16(int idVeh, int idTrip, int preT, int nextT1); //exchange middle P-D trip, preT is pickup and nextT1 is delivery
 void cal_ExchangeSPType17(int idVeh, int idTrip, int preT, int nextT1); //exchange middle P-D trip, preT is pickup and nextT1 is pickup
 void cal_ExchangeSPType18(int idVeh, int idTrip, int preT); //exchange last P-D trip, preT is delivery
 void cal_ExchangeSPType19(int idVeh, int idTrip, int preT); //exchange last P-D trip, preT is pickup
 void cal_ExchangeSPType20(int idVeh, int idTrip, int preT, int nextT); //exchange middle pickup trip, preT is delivery and nextT is delivery
 void cal_ExchangeSPType21(int idVeh, int idTrip, int preT, int nextT); //exchange middle pickup trip, preT is pickup and nextT is delivery
 void cal_ExchangeSPType22(int idVeh, int idTrip, int preT, int nextT); //exchange middle pickup trip, preT is delivery and nextT is pickup
 void cal_ExchangeSPType23(int idVeh, int idTrip, int preT, int nextT); //exchange middle pickup trip, preT is pickup and nextT is pickup
 void cal_ExchangeSPType24(int idVeh, int lastT, int preT); //exchange last delivery trip, preT is delivery trip
 void cal_ExchangeSPType25(int idVeh, int lastT, int preT); //exchange last delivery trip, preT is pickup trip
 void cal_ExchangeSPType26(int idVeh, int lastT, int preT); //exchange last pickup trip, preT is delivery trip
 void cal_ExchangeSPType27(int idVeh, int lastT, int preT); //exchange last pickup trip, preT is pickup trip


  void UpdateTripnum(int idVeh, int idTrip);
  void UpdateRouteTripnum(int idVeh, int idTrip);
  void UpdateVehicle(int idVeh, int curT, int preC, double cStartS, int varvioCAP);
  //dif from UpdateVehicle is: just update till trip update_idTrip rather than update all remaining trips of vehicle
  void UpdateVehicle_TillTrip(int idVeh, int curT, int preC, double cStartS, int varvioCAP, int update_idTrip);

  void UpdateBvioTWCus(int idVeh, int idTrip, int idCus); 
  void UpdateBvioTWCAPCus(int idVeh, int idTrip, int idCus); 
  void UpdateFBvioTWCus(int idVeh, int idTrip, int idCus); 

  void UpdateArray(int idVeh);
  void UpdateDeleteArray(int idVeh);
  void UpdateInsertArray(int idVeh);
  void UpdateMoveCusOneTripArray(int idVeh);
  void UpdateMoveCusPOneVehArray(int idVeh);
  void UpdateExchangeArray(int idVeh);
  void UpdatetwoOPTstarArray(int idVeh);

  void checkEndSFirstPT(int idVeh);


 void pushElite(int PROB);
 int popElite();


//------------------------------------ CREATE INIT SOLUTION-----------------------------//
  void initSol(int typeofseed, int typeofINIT, int typeofAssCusPToSP, int typeofFEE);
  void AssignCusPToSP(int typeofAssCusPToSP);
  bool Create_SequencePickupTripFullCAP(int startSP, int &endSP, int idVeh, int idTrip);
  void Create_FirstPickupTripLeavingDepot(int assignSP);
  //return 0: already last delivery trip; 1: nextTrip will be delivery trip; 2: nextTrip will be pickup trip or trip between 2 sp 
  int Create_DeliveryTrip(int startSP, int& idTrip, double leave_startSP, int &nextSP, int typeofseed, int typeofINIT, int valueFEE); 	//Create a delivery trip (idVeh, idTrip) of sp assignSP where the time to leave assignSP is leave_startSP:
  void Init_NewSolomonI1_NewDeliveryTrip(int startSP, int endSP, int idTrip, double leavestartSP, double &feeMIN, int &nextSP, int &DelPick, int typeofseed, int valueFEE, int typeofINIT, bool &canRoute);  //always create feasible leg
  
  int Create_PickupTrip(int departSP, int &numTrip, double leave_departSP, int &nextSP, int typeofseed, int typeofINIT, int valueFEE);
  void Init_NewSolomonI1_NewPickupTrip(int departSP, int endSP, int idTrip, double leave_departSP, int &nextSP, double &feeMIN, bool &NULLTrip, int typeofseed, int valueFEE, int typeofINIT, bool &canRoute);


  void updateStartSP_FEASIBLETRIP(int idVeh, int idTrip, int preC, int idCus); //update startSP and departP array from idCus on feasible pickup trip(idVeh, idTrip): idCus is not the first customer
  
  /********************************************* CALCULATE INFORMATION *****************************************/
  void calCostFromBeginning();
  void CalInfromationTripD(int idVeh, int idTrip);  
  void CalInfromationTripP(int idVeh, int idTrip);  
  void calFitCostSumTrip();
  void UpdateCostFitnessFeasible();
  void UpdateCostFitnessFeasible1();

  void UpdatePos(int idVeh, int idTrip);

  //------------------------------------------- SHOW SOLUTION-------------------------------//
  void showTripCost(int idVeh, int idTrip);
  void showTripCus(int idVeh, int idTrip);
  void showTripCostTemp(int idVeh, int idTrip);
  void showSolution();
  void showSegment(int idVeh);
  void calReport();
  //----------------------------------------- CHECK SOLUTION --------------------------------//
  bool checkSolution();
  bool checkVehicle(int idVeh);
  bool checkSolutionBuon();
  bool checkTripD_checkSolution(int idVeh, int idTrip);
  bool checkTripP_checkSolution(int idVeh, int idTrip);
  bool checkTripD_ALLFIELDS(int idVeh, int idTrip); //just checking field of trip; for both FEASIBLE and INFEASIBLE TRIP
  bool checkTripP_ALLFIELDS(int idVeh, int idTrip); //just checking field of trip; for both FEASIBLE and INFEASIBLE TRIP

  void GoThroughVehicleBUON(int idVeh, int curT, double &varcost, double &varvioTWC, double &varvioTWSP, double cStartS, int preC, int curC);

  void freeMEM(int nTypeCus);
protected:
  static Problem* problem; //a pointer to problem data
};

#endif



