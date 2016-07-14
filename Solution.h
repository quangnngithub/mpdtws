#ifndef _SOLUTION_H
#define _SOLUTION_H

using namespace std;


#define MAX_VEH						  100
#define MAX_DESTROY_OPERS		      8
#define MAX_REPAIR_OPERS		      4
#define MAX_ELITE_SET				  20

#define  Q_JOINT_POINT_ALLOWANCE	0
#define Q_WAITING_AT_CUST	1

// Definition for DEBUGGING
#define Q_DEBUG						1		//Debug handling

#define Q_DEBUG_PROB_SEL			0

#define Q_DEBUG_LOAD_PARAM			0
#define Q_DEBUG_CHK_SOL				0

#define Q_DEBUG_READ_INPUT			0
#define Q_DEBUG_PU_ASSIGNMENT		0
#define Q_DEBUG_DESTROY				0		//Debug handling
#define Q_DEBUG_REPAIR				0		//Debug handling

#define Q_DEBUG_VALID_TRIP			0		//Debug handling
#define Q_DEBUG_INIT_SOL			1		//Debug handling
#define Q_DEBUG_C2C					0
#define Q_DEBUG_DELIVERY			0  			
#define Q_DEBUG_PICKUP  			0

#define Q_DEBUG_RANDOM_DES			0
#define Q_DEBUG_WORST_COST_DES		0
#define Q_DEBUG_WRONG_POS_DES		0
#define Q_DEBUG_RELATENESS_DES		0
#define Q_DEBUG_SHAW_DES			0

#define Q_DEBUG_RANDOM_REP			0		
#define Q_DEBUG_BEST_FIRST_REP		0
#define Q_DEBUG_2_REGRET_REP		0
#define Q_DEBUG_2_REGRET_VAL		0

#define Q_DEBUG_CALC_RELATEDNESS    0
#define Q_DEBUG_GET_NODE_INFO	    0
#define Q_DEBUG_GET_COST			0		// Calc. cost for a vehicle
#define Q_DEBUG_C2C_DES				0		// Remove_A_C2C_Pair
#define Q_DEBUG_PICKUP_DES			0		// Remove_A_Pickup
#define Q_DEBUG_DELIVERY_RM			0		// Remove_A_Delivery
#define Q_DEBUG_PICKUP_INS			0		// Insert_A_Pickup
#define Q_DEBUG_DELIVERY_INS		0		// Insert_A_Delivery
#define Q_DEBUG_C2C_INS				0		// Insert_A_C2C
#define Q_DEBUG_SET_VEH_TIME		0

#define Q_DEBUG_CHK_LIFO            0
#define Q_DEBUG_CALC_WS_ASSIGN		0

#define CP_PORTING					  0
#define Q_DEBUG_LOAD_SOL			  0
#define Q_DEBUG_RESET_TIME_LOADED_SOL 0
#define Q_DEBUG_SET_TIME			  0

#define Q_DEBUG_ALNS				  0
#define Q_D_CUS_DEMAND_NBR			  0

#define Q_DEBUG_NEIGHBOR_GRAPH        1

#define Q_CRIT_SKIP						0

#define Q_DEBUG_POST_OPT				0

#define Q_D_CALC_LEG_COST				0
#define Q_D_GET_TOT_DEMANDS				0
#define Q_D_CALC_LEG_UTILIZATION		0

#define Q_D_WORST_COST_LEG_DESTROY		0
#define Q_D_REMOVE_A_LEG				0
#define Q_D_INS_A_LEG					0
#define Q_D_INS_A_LEG_BEGINNING			0
#define Q_D_INS_A_LEG_END				0

#define Q_D_RAND_REPAIR_LEG				0

#define Q_D_TIME_RELATED_DES_LEG		0

#define Q_D_HAMMING_DIS_LEG				0

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

struct K_Elite
{
	int arrStartNode[2000];
	int arrEndNode[2000];
	int inNodeTot;
};
class Solution
{
	///
	/// Contains fields and methods to process solutions to the VRP.
	///

public:
   Solution();	
   Solution(Problem* ); //Construct with a pointer to problem data
   Solution(Problem* p, int flag); //QuangNN++
  ~Solution();
	
	// Global paramters
	double g_dbBeta;									    // in [0, 1]	: Control amount of penalty in fitness function
	double g_dbAlpha1, g_dbAlpha2, g_dbAlpha3, g_dbAlpha4;  // Control the bias of SHAW destroy neighborhood (customer level)
	double g_dbPenta, g_dbPenta_com;					    // >= 1	: Control the intensity of the bias on the relatedness
	int	g_inEliteSz;									    // Number of elite solutions in the elite set
	int g_inAlpha;										    // >= 2			: Number of similar legs defines=d in the Relatedness-based-history destroy (customer level)
	double g_dbN_rl, g_dbN_ru, g_dbN_ru0;							    // The range of select the number of removed customer demands (The case not change the size of the destroy)
	double g_dbN_rl_com, g_dbN_ru_com;						// 
	int g_inITnonImp;										/* Number of iterations without improvement on the best solution to switch from small to large search space
															 * (The case changes the size of the destroy)
															 */
	int g_inN_ll, g_inN_lu;									// indicates the range to select the number of removed legs
	double g_dbSigma1, g_dbSigma2, g_dbSigma3;				// Control the amount being increased of the score for a neighborhood (using in Adaptive weight adjustment)
	double g_dbSigma4;										// Control how quickly the weight adjustment reacts to changes in the effectiveness of the neighborhoods
	int g_inIs_Iterations;
	double g_dbTheta0;										// The initial temperature (used in ACCEPTANCE criterion)
	double g_dbTheta_cool;									// in (0, 1): The cooling rate (used in ACCEPTANCE criterion)
	int g_inITmax;											// The maximum number of iterations of ALNS
	int g_inITimpSTOP;										/* The number of iterations without improvement of the best solution, the ALNS will stop
															 * (Use one of them to determine the condition to stop the ALNS)
															 */
	// End of GLOBAL parameters

    //double AVGARC;
	int nOrder, nC2C, nP, nD, nSP, nWS;			// # of non-DEPOT nodes in the solution
    double cost, fitness;		// objective function value
    int vioCAP;
	double vioTWC, vioTWSP;
	bool feasible;

	double g_dbBestTotalCost; // transportation cost + used vehicle cost
	double g_dbBestFitness; // transportation cost + used vehicle cost

	 // Solution storage
    int *nextArr,*predArr;
	double *startS, *depart;
	
    int *route_num;
	int *trip_num;
	int *pos; //position of customer in leg: e.g. Leg: 5, 1, 8 -->pos[5]=1; pos[1]=2, pos[8]=3
   
    int numVeh, numVehUsed; //number of vehicle used
	int g_inInitVehNum;
	int g_inUsedVehNum;
	VRPSegment seg[MAX_VEH+2]; //class VRPSegment *seg;
	double f_star[2000][2000]; //neighborhood graph: best fitness value found so far
	
	VRPTrip removedTrip[10];
	int removeTripNbr;

	K_Elite eliteSet[MAX_ELITE_SET+1];

	int numCusC2CRouted;

	int minTripSP[PDS_MAX_NUM_SUPPLYPOINTS]; // QuangNN++ minimum numlegs required for each supply point; =up(capSP/Q)
	int minTripCusP;						 // QuangNN++minimum numTrips required to serve all pickup customer; = up(TOTALCAPPICKUPCUSTOMERS/Q)

	//int *numTripPSP;//count number of pickup trips assinged to each sp in the solution
	int *numTripDSP; //count number of delivery trips assinged to each sp in the solution
	int numTripP; //number of pickup trips in the solution
	int numTrips; //number of trips in the solution

	double *FvioTWCus, *BvioTWCus, *vioTWCus; //FvioTWCusD: accumulated vioTWCus
	int *vioCAPCus, *CAPCus;
	int *FCAP, *BCAP;


	//bool in_IP;		// Flag to tell if the solution has been added to the IP before
	//double time;	// time at which the solution was first discovered
	//int hash(int salt);	

  //Utility functions.
  Solution(Solution &csol);//copy constructor
  Solution &operator = (Solution &csol);  //assignment; not copy constructor
  Problem* getProblem();
  void init(Problem *p);
  void init_Q(Problem *p); //QuangNN++ specific for MTT-PDTWS with ALNS

  /****************************************************************************************
  /-------------------------------Adaptive Large Neighborhood Search----------------------/
  ****************************************************************************************/
  void vdDumpMemory(int inLogLevel, char *szInfo, double *arrBuf, int inBufSz);
  bool Load_A_Solution(char *szFileName);
  bool Export_Loaded_Solution(char *szFileName);
  bool Save_Best_As_StdFormat(double dbCost, char *szTestIdx, char *szTestOffset);
  void showSolutionNewFormat(char *szTestIdx, char *szTestOffset, bool bOpt);
  void ExportStatPO(char *szTestIdx, char *szTestOffset, double dbRunningTime);
  bool Reset_Time_For_Loaded_Solution();
  void ExportStatSol( char *szFileName,
					  int inBestSegCnt, int inBestItCnt, 
					  int inLastITMax, int inLastImpIT, 
					  double dbRunningTime,
					  double dbBestCost,
					  double dbFitness,
					  int * arrUsedNeighbor); 
 
  void ExportStatSeg( int inCurrSeg,
					  int inBestSegCnt, int inBestItCnt, double dbBestCost,
					  int inRollBackCnt,
					  int * arrUsedNeighbor); 
 
  void vdLoadParameters(void);
  bool ValidParameters(void);
  void vdDumpParameters(void);

  // Main
  void ALNS_Main(char *szStatFileName, char *szTestIdx, char *szTestOffset);

  // Acceptance criteria
  bool Acceptance_Criteria(double dbCurTotalCost, double dbNewTotalCost, double dbTemperature);
  bool Acceptance_Criteria2(double dbCurTotalCost, double dbNewTotalCost, double dbTemperature);
  // Stop conditions
  bool Stop_Condition_ALNS(int inSegmentCounter, int inNoNewBestSolCounter);

  //Select customer-demands: pickup/delivery/c2c
  int Select_Cus_Demand_Type(void);
  // Select number of customer-demands
  int Select_Cus_Demand_Nbr(int cusDemandType, bool bChanged);
  // Select Destroy/Repair Operators
  //int Select_ALNS_Operator(int *OperWeights, int OperNbr);

  bool IsLIFO_Q(int* arrC2CTrip, int inNodeNbr);
  int Select_Probability_By_Weight(double *OperWeights, int OperNbr, bool isRandom);
  int Select_Probability_By_Weight2(double *OperWeights, int StartIdx, int OperNbr, bool isRandom);
  int Get_Node_Tot_In_Veh(int inVehIdx);
  int Build_Seq_Leg(int inStartSP, int inEndSP, double dbDepartureTime);

  //**** DESTROY OPERATORS
  bool Random_Destroy(int inCusDemandType, int inCusDemandNbr);
  bool Worst_Cost_Destroy(int inCusDemandType, int inCusDemandNbr);
  bool Shaw_Destroy(int inCusDemandType, int inCusDemandNbr);
  bool Relatedness_Destroy(int inCusDemandType, int inCusDemandNbr);
  bool Wrong_Position_Destroy(int inCusDemandType, int inCusDemandNbr);

  void Calc_Leg_Utilization();
  int Get_Total_Demands(int inVehIdx, int inTripIdx);
  double Calc_Leg_Cost(int inVehIdx, int inTripIdx);
  bool Worst_Util_Leg_Destroy();
  bool Time_Related_Leg_Destroy();
  
  
	// REPAIR OPERATORS
  bool Insert_A_Leg(int inVehIdx, int inTripIdx, int inRemovedTripIdx, bool isEvaluated, double *dbCostTmp);
  bool Insert_A_Del_Leg_At_Begin(int inVehIdx, int inTripIdx, int inRemovedTripIdx, bool isEvaluated, double *dbCostTmp);
  bool Insert_A_Del_Leg_At_End(int inVehIdx, int inTripIdx, int inRemovedTripIdx, bool isEvaluated, double *dbCostTmp);
  bool Insert_A_Pick_Leg_At_Begin(int inVehIdx, int inTripIdx, int inRemovedTripIdx, bool isEvaluated, double *dbCostTmp);
  bool Insert_A_Pick_Leg_At_End(int inVehIdx, int inTripIdx, int inRemovedTripIdx, bool isEvaluated, double *dbCostTmp);
  bool isHasDeliveryTrip(int inVehIdx, int inSupplyPoint);
  bool isHasPickupTrip(int inVehIdx, int inSupplyPoint);
  void Get_Shared_SP_Pick_Demands(int inRemovedTripIdx, int *arrSP, int *inSPSz);
  bool Random_Greedy_Repair_Leg();
  bool Random_Greedy_Repair();
  bool Best_First_Repair();
  bool Two_Regret_Repair();
  double Calc_Regret_Value(int inNodeIdx);
  bool Insert_A_Pickup(int inPickupIdx, int inVehIdx, int inTripIdx, bool isCreated, double *dbEvaluatdCost);
  bool Insert_A_Delivery(int inDeliveryIdx, int inVehIdx, int inTripIdx, bool isCreated, double *dbEvaluatdCost);
  bool Insert_A_C2C(int inC2CPickupIdx, int inVehIdx, int inTripIdx, bool isCreated, double *dbEvaluatdCost);

  // Elite set operator
  int Calc_Hamming_Dis(K_Elite z1, K_Elite z2);
  double Calc_Hamming_Dis_Leg(int inVehIdx1, int inTripIdx1, int inVehIdx2, int inTripIdx2);
  void Init_Elite_Set();
  void vdDumpEliseSet();
  bool Update_Elite_Set();
  int Calc_Relatedness_Based_Mem(int inNodeIdx1, int inNodeIdx2);

  // Neighborhood graph
  void Init_Neighborhood_Graph();
  bool Update_Neighborhood_Graph(double dbBestFitness);
  double Calc_Score_Neighborhood_Graph(int inNodeIdx);
  bool Get_Incident_Of_Node(int inNodeIdx, int *inPredNodeIdx, int *inSucNodeIdx);

  double Calc_Relatedness(int inNodeIdx1, int inNodeIdx2, double dbMaxCij, double dbDeltaTime, double dbDeltaCap);
  bool Calc_Delta_Norm_Shaw_Destroy(int inTripType, double *dbDeltaTime);
  bool Get_Node_Info(int inNodeIdx, double *dbTime2Served, int *inVehicleIdx, int *inTripIdx); 
  void Where_Are_Nodes();
  int Get_Node_Type(int inNodeIdx);
  double Get_InsertedNode_Cost(int inVehIdx, int inTripIdx, int inNodeIdx);
  double Evaluate_InsertedNode_Cost(int *arrTrip, int inTripSz, int inStartNodeIdx, int inEndNodeIdx, int inNodeIdx);

  bool Check_Shared_SP(int inNodeIdx1, int inNodeIdx2);
  bool Check_Assign_SP(int inNodeIdx, int inSPIdx);
  bool Belong_To_SP(int inNodeIdx, int inSPIdx);
  bool Get_Assigned_SP_List(int inNodeIdx, int *arrAssignedSP, int *inSPnbr);
  int Get_Assigned_SP(int inNodeIdx);

  void Remove_A_Vehicle(int inVehIdx);
  void Remove_A_Trip(int inVehIdx, int inTripIdx, int flag);
  bool Remove_A_Leg(int inVehIdx, int inTripIdx);

  int Update_SP_Status(void);
  int Show_Demands_Status(void);

  int Remove_A_Pickup(int inPickupIdx);
  int Remove_A_Delivery(int inDeliveryIdx);
  int Remove_A_C2C_Pair(int inC2CPickupIdx);

  bool Valid_A_Trip(int inVehIDx, int inTripIdx);
  bool Valid_A_Trip_With_TimeStart(int inVehIDx, int inTripIdx, double dbStartTime);
  bool Set_Time_For_A_Vehicle(int inVehIdx, int inTripIdx, double dbStartTime, bool bUpdate);

  bool Re_Set_Joint_For_A_Vehicle(int inVehIDx);
  bool Re_Set_Time_For_A_Vehicle(VRPSegment veh, double dbStartTime);

  bool Join_Trips(int inVehIdx);

  void Show_A_Trip(int inVehIDx, int inTripIdx);
  void Get_Time_For_Hamming_Dis_Calc(int inVehIDx, int inTripIdx, double *dbStartFirstCus, double *dbStartLastCus);
  bool Check_Solution(double *dbTotalCost);
  void showSolution_Old(char *szSuffFn);
  void Show_A_Vehicle(int inVehIdx);
  void Show_A_Vehicle2(VRPSegment veh);
  double Get_Cost_A_Vehice(int inVehIdx);
  bool Check_TW(int inNodeIdx, double db_TimePoint, bool isWaiting);
  bool Check_vioTW(void);
  double Get_Cost_A_Vehice_160616(int inVehIdx);

#if CP_PORTING
  void initSol_CP(int typeofseed, int typeofINIT, int typeofAssCusPToSP, int valueFEE);
#endif
  //------------------------------------ TABU ---------------------------------------------//
  void callTABU(int max_iteration);
  void init_parametersTABU_CUS(int nType);
  void init_parametersTABU_SP(int nType); //Quangnn++ for compiling
  void init_arrayTABU();
  bool TabuMAIN1(int max_iteration, Solution &solBEST);
  bool TabuMAIN(int max_iteration, Solution &solBEST);
  int Difference(int idElite);


  void update_solutionCusD(moveT best_move);
  void update_solutionCusP(moveT best_move);
  void update_solutionC2C(moveT best_move);

  void update_parameters();


  void Update_deleteCusArrayD_FirstTrip_Empty(int idVeh);
  void Update_deleteCusArrayD_MiddleTrip_Empty(int idVeh, int idTrip); //update deleteCusD for middle delivery trip (idVeh, idTrip) with only 1 delivery customer
  void Update_deleteCusArrayD_FirstMiddleTrip(int idVeh, int idTrip); //only applied for the case where vehicle has >= 2 trips
  void Update_deleteCusArrayD_LastTrip_Empty(int idVeh, int idTrip);
  void Update_deleteCusArrayD_LastTrip(int idVeh, int idTrip);

  void Update_deleteCusArrayP_FirstTrip_Empty(int idVeh);
  void Update_deleteCusP_FirstTrip_2Cus(int idVeh);
  void Update_deleteCusArrayPORC2C_MiddleTrip_Empty(int idVeh, int idTrip); //update deleteCusP for middle delivery trip (idVeh, idTrip) with only 1 pickup customer
  void Update_deleteCusArrayP_FirstMiddleTrip(int idVeh, int idTrip); //only applied for the case where vehicle has >= 2 trips
  void Update_deleteCusArrayP_LastTrip_Empty(int idVeh, int idTrip);
  void Update_deleteCusArrayP_LastTrip(int idVeh, int idTrip);

  void Update_deleteCusArrayC2C_FirstTrip_Empty(int idVeh);
  void Update_deleteCusArrayC2C_FirstTrip_4Cus_nextD(int idVeh);
  void Update_deleteCusArrayC2C_FirstMiddleTrip(int idVeh, int idTrip);
  void Update_deleteCusArrayC2C_LastTrip_Empty(int idVeh, int idTrip);
  void Update_deleteCusArrayC2C_LastTrip(int idVeh, int idTrip, int lastCSP_preT, double cStartS); 

  void Update_deleteCusC2CTrip_FirstTrip_4Cus_nextD(int idVeh, int pre_deletednode,int deletednode, int next_deletednode, int nextSP, int update_idTrip);
  void Update_deleteCusC2C_LastTrip(int idVeh, int idTrip, int pre_deletednode,int deletednode, int next_deletednode); 
  void Update_deleteCusC2C_LastTrip_FromBeginning(int idVeh, int idTrip, int deletednode); 
  void Update_deleteCusC2C_FirstMiddleTrip(int idVeh, int idTrip, int pre_deletednode, int deletednode, int next_deletednode, int update_idTrip);
  void Update_deleteCusC2C_FirstMiddleTrip_FromBeginning(int idVeh, int idTrip, int deletednode);
  
  void Update_deleteCusArrayP_MiddleTrip_Empty(int idVeh, int idTrip); //Quangnn++ for compiling

  //COPY void Update_deleteCusArrayC2C_LastTrip(int idVeh, int idTrip, int lastCSP_preT, double cStartS):
  void SUBcal_deleteCusArrayC2C_FromBeginning(int preC, bool changed_preT, int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP,double &cStartS, bool &contTW, int &lastCNEW);
  void SUB_Update_deleteCusC2C_FromBeginning(int idVeh,int idTrip, int deletednode, double &varcostpreT, double &varcost);

  void cal_deleteCusArrayC2C_LastTrip_FromBeginning(int preC, bool changed_preT, int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP,double cStartS);
  void cal_deleteCusArrayC2C_MiddleTrip_FromBeginning(int preC, bool changed_preT, int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP,double cStartS);

  //Copy Update_deleteCusArrayC2C_FirstTrip_Empty(int idVeh);
  bool cal_deleteCusArrayC2C_FirstTrip_Empty_TillTrip(int idVeh, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int update_idTrip);
  //Copy Update_deleteCusArrayC2C_FirstTrip_4Cus_nextD(int idVeh)
  bool cal_deleteCusC2C_FirstTrip_4Cus_nextD_TillTrip(int idVeh, int i, double &varcost, double &varvioTWC, double &varvioTWSP, int varvioCAP, double &cStartS, int update_idTrip);

  //COPY  void Update_deleteCusArrayC2C_FirstMiddleTrip(int idVeh, int idTrip);
  bool cal_deleteCusArrayC2C_FirstMiddleTrip_TillTrip(int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP, double &cStartS, int update_idTrip);

   //i is the first cus where there is only one ORDER --> delete i and its delID, this trip becomes empty
  //then insert i after j on the second trip --> after empty the first trip, this second trip becomes the first trip
  void cal_deleteCusArrayC2C_FirstTrip_Empty_insertNextTrip_LastC(int j, int i, int idVeh, double &varcosti, double &varvioTWCi, double &varcostj, double &varvioTWCj, double &varvioTWSPj, int &varvioCAP);
  void cal_deleteCusArrayC2C_FirstTrip_Empty_insertNextTrip_NotLastC(int j, int i, int idVeh, double &varcosti, double &varvioTWCi, double &varcostj, double &varvioTWCj, double &varvioTWSPj, int &varvioCAPj, int &moveAfter);


  //copy void Update_deleteCusArrayC2C_LastTrip(int idVeh, int idTrip, int lastCSP_preT, double cStartS); 
  void cal_deleteCusArrayC2C_LastTrip(int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, int &varvioCAP);

  //Copy Update_deleteCusArrayP_FirstTrip_Empty
  bool cal_deleteCusArrayP_FirstTrip_Empty_TillTrip(int idVeh, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int update_idTrip);
 
   //i is the first cus where there is only one customer --> delete i, this trip becomes empty
  //then insert i after j on the second trip --> after empty the first trip, this second trip becomes the first trip
  void cal_deleteCusArrayP_FirstTrip_Empty_insertNextTrip(int j, int i, int idVeh, double &varcosti, double &varvioTWCi, double &varvioTWSPi, double &varcostj, double &varvioTWCj, double &varvioTWSPj, int &varvioCAP);
  
  //Copy Update_deleteCusP_FirstTrip_2Cus:
  bool cal_deleteCusP_FirstTrip_2Cus_TillTrip(int idVeh, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP,int &varvioCAP, double &cStartS, int update_idTrip);
  //Copy Update_deleteCusArrayP_FirstMiddleTrip:
  bool cal_deleteCusArrayP_FirstMiddleTrip_TillTrip(int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP, double &cStartS,int update_idTrip);
  void cal_deleteCusArrayP_MiddleTrip_FromBeginning(int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP, double cStartS);

  //Copy Update_deleteCusArrayPORC2C_MiddleTrip_Empty
  bool cal_deleteCusArrayPORC2C_MiddleTrip_Empty_TillTrip(int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int update_idTrip);
  void cal_deleteCusArrayPORC2C_MiddleTrip_Empty_FromBeginning(int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, double cStartS);

  void cal_deleteCusArrayC2C_MiddleTrip_Empty_FromBeginning(int preC, int idVeh, int idTrip, double &varcosti, double &varvioTWC, double &varvioTWSP, double cStartS);

  //Copy Update_deleteCusArrayP_LastTrip:
  void cal_deleteCusArrayP_LastTrip(int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP);
  void cal_deleteCusArrayP_LastTrip_FromBeginning(int idVeh, int idTrip, int deletednode, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP, double cStartS);

 //COPY Update_deleteCusArrayP_LastTrip_Empty:
  void cal_deleteCusArrayPORC2C_LastTrip_Empty(int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP);

  
  void GoThroughVehicle_FirstPT(int idVeh,  int curT, double &varcost, double &varvioTWC, double &varvioTWSP);
  void GoThroughVehicle_FirstC2C(int idVeh,  int curT, double &varcost, double &varvioTWC, double &varvioTWSP);

  bool GoThroughVehicle_FirstPT(int idVeh, int curT, int numTrips, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS);
  bool GoThroughVehicle_FirstC2C(int idVeh, int curT, int numTrips, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS);

  void GoThroughVehicle(int idVeh, int curT, double &varcost, double &varvioTWC, double &varvioTWSP, double cStartS, int preC, int curC);
  bool GoThroughVehicle(int idVeh, int curT, int numTrips, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int preC, int curC);

  bool GoThroughVehicle(int idVeh, int curT, int numTrips, double &varcost, double &varvioTWC, double &varvioTWSP, double cStartS, int preC, int curC); //Quangnn++ for compiling

  //update cStartS, varvioTWSP and varcost when connecting id1 and assSP of delivery trip (idVeh, idTrip): where id1 is cus or sp;
  //return 1 if don't need to calculate anymore; 0 otherwise
  bool calCONNECTCSPTOSPD_FORDELETE(int id1, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost);
  bool calChangeCONNECTCSPTOSPD_FORDELETE(int id1, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost);

 //calculte change in varcost, cStartS, varvioTWSP when connecting last delivery cus lastC of (idVeh, preT) with assSP of (idVeh, idTrip):
  bool calChangeCONNECTAtSP_DD(int lastC, int idVeh, int idTrip, int preT, double &cStartS, double &varvioTWSP, double &varcost);
  bool calChangeCONNECTAtSP_DD_LastCChanged(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost);
  bool calChangeCONNECTAtSP_DD_LastCChanged(int lastCOLD, int lastC, int idVeh, int curT, int preT, int varvioCAP, double varvioTWC, double varcost); //for deleteCusD procedure

  bool updateChangeCONNECTAtSP_DDORC2CD(int lastC, int idVeh, int idTrip, int preT, double &cStartS); //for procedure UpdateVehicle
  bool updateChangeCONNECTAtSP_DD(int lastC, int idVeh, int idTrip, int preT, int varvioCAP, double varvioTWC, double varcost); //for deleteCusD procedure
  bool updateChangeCONNECTAtSP_DD(int lastC, int idVeh, int idTrip, int preT, int varvioCAP, double varvioTWC, double costTrip, double costOLD); //for twoOPTstarCusDTrip
  void updateChangeCONNECTAtSP_DDORC2CD(int idVeh, int nextTrip, int preT, int varvioCAP, int update_idTrip); //for Update_EmptyTripD and Update_EmptyTripP, delete_insertCusPTrip: connect deliverytrip(idVeh, preT) with delivery trip (idVeh, nextTrip) where (idVeh, idTrip) is already deleted 

  //both procedures will double the value of varvioTWSP in case nextTrip is delivery trip and sameSP:
  bool calChangeCONNECTAtSP_PickTrip(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost); 
  bool calChangeCONNECTAtSP_PickTrip(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost, bool &vioTWSPTRUE); 
  bool calChangeCONNECTAtSP_PickTrip_NextDSameSP(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost); 
  bool calChangeCONNECTAtSP_PickTrip2(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost); //dont check if there is a case p-dSAMESP as in the calChangeCONNECTAtSP_PickTrip
  void calChangeCONNECTAtSP_PickTrip2_1(int lastC, int idVeh, int idTrip, double &cStartS, double &vioTWSPNEW, double &varcost); //dont check if there is a case p-dSAMESP as in the calChangeCONNECTAtSP_PickTrip; used for MoveSP

  bool calChangeCONNECTAtSP_C2CTrip(int lastC, int idVeh, int idTrip, int nextT, double &cStartS, double &varvioTWSP, double &varcost); 
  bool calChangeCONNECTAtSP_C2CTrip_LastCChanged(int lastC, int idVeh, int idTrip, int nextT, double &cStartS, double &varvioTWSP, double &varcost); 

  bool calChangeCONNECTAtSP_PickTrip_LastCChanged(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost);
  bool calChangeCONNECTAtSP_PickTrip_LastCChanged(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost, bool &vioTWSPTRUE);
  bool calChangeCONNECTAtSP_PickTrip_LastCChanged2(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost);//dont check if there is a case p-dSAMESP as in calChangeCONNECTAtSP_PickTrip_LastCChanged

  bool calChangeCONNECTAtSP_PickTrip(int lastC, int idVeh, int idTrip, double &cStartS); //for procedure UpdateVehicle

  bool calChangeCONNECTAtSP_SPSP(int idVeh, int idTrip, int preT, double &cStartS, double &varvioTWSP, double &varcost);
  bool calChangeCONNECTAtSP_SPSP_MoveSP(int idVeh, int idTrip, int assSP, double &cStartS, double &varvioTWSP, double &varcost);

  bool calChangeCONNECTAtSP_PickTrip_FORDELETE(int lastC, int idVeh, int idTrip, double &cStartS, double &varvioTWSP, double &varcost); //Quangnn++ for compiling
  bool calChangeCONNECTAtSP_DD(int lastC, int idVeh, int idTrip, int preT, double &cStartS); //Quangnn++ for compiling
  bool calChangeCONNECTAtSP_DD(int lastC, int idVeh, int idTrip, int preT, int varvioCAP, double varvioTWC, double varcost);  //Quangnn++ for compiling
  bool calChangeCONNECTAtSP_SPSP(int idVeh, int idTrip, int preT, double &cStartS); //Quangnn++ for compiling
  bool calChangeCONNECTAtSP_DD(int lastC, int idVeh, int idTrip, int preT, int varvioCAP, double varvioTWC, double costTrip, double costOLD); //Quangnn++ for compiling
  void cal_InsertCusDIAfterJ_LastC_NextTripP(int j, int k, int idVeh, int idTrip); //Quangnn++ for compiling
  void cal_InsertCusPIAfterJ_LastC_NextTripP(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); //Quangnn++ for compiling
  void cal_InsertCusPIAfterJ_NotLastC_NextTripP(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_MoveCusDIAfterJ_oneTrip_NextTripP(int j, int k,int idVeh, int idTrip);
  void cal_MoveCusPIAfterJ_oneTrip_NextTripP(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_twoOPTstarCusDIAndJ_oneTrip_NextTripP(int j, int k);
  void twoOPTstarCusDTrip_oneTrip(int j, int i);
  bool twoOPTstarCusDTrip(int j, int i);
  void UpdateVehicle(int idVeh, int curT, int preC, double cStartS, int varvioCAP);
  void Create_FirstPickupTripLeavingDepot(int assignSP);
  void Init_NewSolomonI1_NewDeliveryTrip(int startSP, int endSP, int idTrip, double leave_startSP, double &feeMIN, int &nextSP, int &createTrip, int typeofseed, int valueFEE, int typeofINIT, bool &canRoute);
  int Create_PickupTrip(int departSP, int &numTrip, double leave_departSP, int &nextSP, int typeofseed, int typeofINIT, int valueFEE);
  void Init_NewSolomonI1_NewPickupTrip(int departSP, int endSP, int idTrip, double leave_departSP, int &nextSP, double &feeMIN, bool &NULLTrip, int typeofseed, int valueFEE, int typeofINIT, bool &canRoute);

  int Create_Single_Delivery_Trip_Q(
							int inDEIdx,
							int startSPIdx,
							int inVehNum);

  int Create_Single_Pickup_Trip_Q(
							int inPUIdx,
							int endSPIdx,
							int inVehNum);

  int Create_Single_C2C_Trip_Q(
							int inC2CPUIdx,
							int inVehNum);

  int Create_New_C2C_Trip_Q(
							int inC2CPUIdx,
							int inVehNum,
							int inStartIdx,
							double leaveDepartSPIdx ,
							int inEndIdx
							);

  bool Create_PickupTrip_Q(int departSPIdx, 
					   	   double leaveDepartSPIdx, 
						   int endSPIdx,
						   int inEndPointType,

						   int inCurrVehCap,
						   int inCurrVehIdx,
						   int inCurrTripIdx);

  bool Create_DeliveryTrip_Q2(int departSPIdx, 
					   	   double leaveDepartSPIdx, 
						   int endSPIdx,
						   int inEndPointType,

						   int inCurrVehCap,
						   int inCurrVehIdx,
						   int inCurrTripIdx);


  int Create_PickupTrip_Q_Old(int departSP, int &numTrip, 
					   	  double leave_departSP, int &nextSP, 
						  int endSP,
						  int typeofseed, int typeofINIT, int valueFEE);


int Get_Node_Nbr_In_DeliveryTrip_Q(int startSP, int &numTrip, 
						  double leave_startSP, int &nextSP, 
						  int endSP,
						  int typeofseed, int typeofINIT, int valueFEE);

  bool SetBeginTimeForPath_Q(double inStartTime, int inStartPointIdx,
						   int *arrPath,
						   int inPathSz,
						   double *arrBeginTime,
						   int inEndpointIdx,
						   double *dbEndS,
						   int *inWSId,
						   int *inCapacity);
  bool CreateC2CTripSolomonI1_Q(int inVehicleIdx, int inTripIdx, 
								int inVehCurrCap,
								int inStartPointIdx, int inDepartTimeAtStart, 
								int inEndPointIdx, int inEndPointType, int inSTWAtEndPoint, int inETWAtEndPoing, 
								int *arrC2CPCandiadte, int inCandiadateNbr);

  bool updateChangeCONNECTAtSP_SPSP(int idVeh, int idTrip, int preT, double &cStartS); //for procedure UpdateVehicle
  void updateChangeCONNECTAtSP_SPSP(int idVeh, int nextTrip, int preT, int varvioCAP, int update_Trip); //for Update_EmptyTripD and Update_EmptyTripP, delete_insertCusPTrip: connect pickuptrip(idVeh, preT) with delivery trip (idVeh, nextTrip) where (idVeh, idTrip) is already deleted 
  void updateChangeCONNECTAtSP_SameSP(int idVeh, int nextTrip, int preT, int varvioCAP, int update_Trip); //for Update_EmptyTripC2C

  void cal_InsertCusDIntoEmptyVeh(int idCus, int startSP);
  void cal_InsertCusDIAfterJ_LastC_LastTrip(int j, int k, int idVeh, int idTrip);
  void cal_InsertCusDIAfterJ_NotLastC_LastTrip(int j, int k, int idVeh, int idTrip);
  void cal_InsertCusDIAfterJ_LastC_NextTripP_OR_C2C(int j, int k, int idVeh, int idTrip);
  void cal_InsertCusDIAfterJ_NotLastC_NextTripP_OR_C2C(int j, int k, int idVeh, int idTrip);
  void cal_InsertCusDIAfterJ_LastC_NextTripD(int j, int k, int idVeh, int idTrip);
  void cal_InsertCusDIAfterJ_NotLastC_NextTripD(int j, int k, int idVeh, int idTrip);

  void cal_InsertCusDIAfterJ_NotLastC_NextTripP(int j, int k, int idVeh, int idTrip); //Quangnn++ for compiling

  void cal_MoveCusDIAfterJ_oneTrip_LastTrip(int j, int k, int idVeh, int idTrip);
  void cal_MoveCusDIAfterJ_oneTrip_NextTripPORC2C(int j, int k, int idVeh, int idTrip);
  void cal_MoveCusDIAfterJ_oneTrip_NextTripD(int j, int k, int idVeh, int idTrip);


  bool SUBcal_InsertCusPIAfterJ_NotLastC(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_NotLastC1(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_NotLastC1(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, bool &vioTWSPTRUE); //Used for the case p-dSAMESP
  bool SUBcal_InsertCusPIAfterJ_NotLastC1_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_NotLastC1_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, bool &vioTWSPTRUE); //Used for the case p-dSAMESP
  void cal_InsertCusPIAfterJ_NotLastC_NextTripPORC2C(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_InsertCusPIAfterJ_NotLastC_NextTripD_SameSP(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_NotLastC_NextTripD_DifSP(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_NotLastC_NextTripPORC2C_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_InsertCusPIAfterJ_NotLastC_NextTripD_SameSP_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusPIAfterJ_NotLastC_NextTripD_DifSP_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP);
  
  bool SUBcal_InsertCusPIAfterJ_LastC(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_LastC1(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &vioTWSPNEW, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_LastC1(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &vioTWSPNEW, double &cStartS, bool &vioTWSPTRUE); 
  bool SUBcal_InsertCusPIAfterJ_LastC1_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_LastC1_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, bool &vioTWSPTRUE); //Used for the case p-dSAMESP
  bool SUBcal_InsertCusPIAfterJ_LastC_1CusFirstTrip(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS); 
  bool SUBcal_InsertCusPIAfterJ_LastC_1CusFirstTrip(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, bool &vioTWSPTRUE); //Used for the case p-dSAMESP


  bool SUBcal_InsertCusC2CIAfterJ_2CusFirstTrip_nextD(int j, int i, int idVeh, int nextSP, int update_idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP, int &insertAfter, double &cStartS);
  void SUBcal_InsertCusC2CIAfterJ_LastC(double depart_j, int j, int i, int cusDi, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &cStartS);
  void cal_InsertCusC2CIAfterJ_LastC_LastTrip(double depart_j, int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC);
  void cal_InsertCusC2CIAfterJ_LastC_NextD(double depart_j, int j, int i, int idVeh, int idTrip, int nextSP, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_InsertCusC2CIAfterJ_LastC_NextTripPORC2C(double depart_j,int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP);

  void GoThroughLegC2C_CAP(int curC, int tillC, int &CapVehCus, int &varvioCAP);
  void GoThroughLegC2C(int preC, int curC, int tillC, double &cStartS, int &CapVehCus, int &varvioCAP, double &varvioTWC, bool &contCAP, bool &contTW);
  void UpdateLegC2C_Cus(int preC, int curC, int tillC, int CapVehCus, double cStartS, bool firstC);
  void UpdateLegC2C_Cus_Time(int preC, int curC, int tillC, int CapVehCus, double cStartS, bool firstC);

  bool GoThroughLegC2C(int preC, int curC, double &cStartS, double &varvioTWC, int CapVehCus, int &varvioCAP);
  void SUBcal_InsertCusC2CIAfterJ_NotLastC_LastTrip(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip, double &varcostMIN, double &varvioTWCMIN,  int &varvioCAPMIN, int &insertAfter);
  void SUBcal_InsertCusC2CIAfterJ_NotLastC_LastTrip_delIDIN(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip, double &varcostMIN, double &varvioTWCMIN,  int &varvioCAPMIN, int &insertAfter);
  void SUBcal_InsertCusC2CIAfterJ_NotLastC_LastTrip_iANDdelIDIN(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip, double &varcostMIN, double &varvioTWCMIN,  int &varvioCAPMIN, int &insertAfter);

  void SUBcal_InsertCusC2CIAfterJ_NotLastC_NextD(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip, int nextSP,double &varcostMIN, double &varvioTWCMIN, double &varvioTWSPMIN, int &varvioCAPMIN, int &insertAfter);
  void SUBcal_InsertCusC2CIAfterJ_NotLastC_NextD_COPY(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip, int nextSP,double &varcostMIN, double &varvioTWCMIN, double &varvioTWSPMIN, int &varvioCAPMIN, bool &stopMIN, double &cStartS, int &insertAfter, int update_idTrip);
  void SUBcal_InsertCusC2CIAfterJ_NotLastC_delIDIN_nextD(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip, int nextSP, double &varcostMIN, double &varvioTWCMIN, double &varvioTWSP,  int &varvioCAPMIN, int &insertAfter);

  void SUBcal_InsertCusC2CIAfterJ_NotLastC_NextTripPORC2C(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip,double &varcostMIN, double &varvioTWCMIN, double &varvioTWSPMIN, int &varvioCAPMIN, int &insertAfter);
  void SUBcal_InsertCusC2CIAfterJ_NotLastC_NextTripPORC2C_COPY(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip,double &varcostMIN, double &varvioTWCMIN, double &varvioTWSPMIN, int &varvioCAPMIN, bool &stopMIN, double &cStartS, int &insertAfter, int update_idTrip);
  void SUBcal_InsertCusC2CIAfterJ_NotLastC_delIDIN_NextTripPORC2C(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip, double &varcostMIN, double &varvioTWCMIN, double &varvioTWSPMIN, int &varvioCAPMIN, int &insertAfter);
  void SUBcal_InsertCusC2CIAfterJ_NotLastC_iANDdelIDIN_NextD(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip, int nextSP, double &varcostMIN, double &varvioTWCMIN, double &varvioTWSPMIN, int &varvioCAPMIN, int &insertAfter);
  void SUBcal_InsertCusC2CIAfterJ_NotLastC_iANDdelIDIN_NextTripPORC2C(double depart_j, int CAPCusj, int j, int i, int idVeh, int idTrip, double &varcostMIN, double &varvioTWCMIN, double &varvioTWSPMIN, int &varvioCAPMIN, int &insertAfter);

  /******************************************** NEED TO FINISH **************************************************************/
  /*----------------------------------------------------------------------------------------------------------------------------*/	  
  
  
  //idTripj > 0
  void cal_InsertCusC2CIAfterJ_FromBeginning(int j, int i, int idVeh, int idTripj, int preT_lastC, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, int &insert_After, double cStartS);
  bool cal_InsertCusC2CIAfterJ_LastC_TillTrip(double depart_j,int j, int i, int idVeh, int idTripj, double &varcostj, double &varvioTWCj, double &varvioTWSPj, double &cStartS, int update_idTrip);
  bool cal_InsertCusC2CIAfterJ_NotLastC_TillTrip(double depart_j, int CAPCusj,int j, int i, int idVeh, int idTripj, double &varcostj, double &varvioTWCj, double &varvioTWSPj,int &varvioCAP, double &cStartS, int &insert_After,int update_idTrip);	//THIS PROCEDURE DOESN'T USE ANYWHERE

  bool cal_InsertCusC2CIAfterJ_LastC_NextD_TillTrip(double depart_j, int j, int i, int idVeh, int idTrip, int nextSP, double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int numTrips_update);
  bool cal_InsertCusC2CIAfterJ_LastC_NextTripPORC2C_TillTrip(double depart_j, int j, int i, int idVeh, int idTripj,double &varcost, double &varvioTWC, double &varvioTWSP, double &cStartS, int numTrips_update);

  void cal_InsertCusPIAfterJ_LastC_NextTripPORC2C(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_InsertCusPIAfterJ_LastC_NextTripPORC2C_Start(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_InsertCusPIAfterJ_LastC_NextTripPORC2C_1CusFirstTrip(int j, int i, int idVeh, int idTrip, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP); 
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
  void cal_MoveCusPIAfterJ_oneTrip_NextTripPORC2C(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP); 
  void cal_MoveCusPIAfterJ_oneTrip_NextTripD_SameSP(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP);
  void cal_MoveCusPIAfterJ_oneTrip_NextTripD_DifSP(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP);


  //Move cus i on (idVeh, idTripi) after cus j on (idVeh, idTripj):
  void cal_MoveCusPIAfterJ_oneVeh(int j, int idTripj, int i, int idTripi, int idVeh, double &varcost, double &varcosti, double &varcostj, double &varvioTWC, double &varvioTWCi, double &varvioTWCj, double &varvioTWSP, double &varvioTWSPi, double &varvioTWSPj, int &varvioCAP);
  //idTripj > 0:
  void cal_InsertCusPIAfterJ_FromBeginning(int j, int i, int idVeh, int idTripj,int preT, int &varvioCAP, double &varcost, double &varvioTWC, double &varvioTWSP, double cStartS);


  void SUBcal_MoveCusC2CIAfterJ_oneTrip_LastC(int j, int i, int idVeh, int idTrip, double &varvioTWC, int &varvioCAP, int &CapVehCus, double &cStartS, double &varcost);
  void cal_MoveCusC2CIAfterJ_oneTrip_LastTrip_LastC(int j, int i, int k, int idVeh, int idTrip); //idVeh may have only one trip; j and i are in the same trip; j is the last cus of the trip
  void cal_MoveCusC2CIAfterJ_oneTrip_LastC(int j, int i, int k, int idVeh, int idTrip); //(idVeh, idTrip) is not the last trip of idVeh

  void cal_MoveCusC2CIAfterJ_oneVeh_LastC(int j, int idTripj, int i, int idTripi, int idVeh, int k);
  void cal_MoveCusC2CIAfterJ_oneVeh_NotLastC(int j, int idTripj, int i, int idTripi, int idVeh, int k);


  void cal_MoveCusC2CIAfterJ_oneTrip_LastTrip_NotLastC(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, int &varvioCAP, int &moveAfter);
  void cal_MoveCusC2CIAfterJ_oneTrip_NotLastC_nextD(int j, int i, int idVeh, int idTrip, int nextSP, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP, int &moveAfter);
  void cal_MoveCusC2CIAfterJ_oneTrip_NotLastC_NextTripPORC2C(int j, int i, int idVeh, int idTrip, double &varcost, double &varvioTWC, double &varvioTWSP, int &varvioCAP, int &moveAfter);


  void cal_ExchangeCusDIAndJ(int j, int k);
  void cal_ExchangeCusPIAndJ(int j, int k);
  void cal_ExchangeCusC2CIAndJ(int j, int k);


  void cal_twoOPTstarCusDIAndJ_oneTrip_LastTrip(int j, int k); //connect j with i where j and i are in the same trip
  void cal_twoOPTstarCusDIAndJ_oneTrip_NextTripD(int j, int k); //connect j with i where j and i are in the same trip
  void cal_twoOPTstarCusDIAndJ_oneTrip_NextTripPORC2C(int j, int k); //connect j with i where j and i are in the same trip
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
  void evaluate_moveCusC2C(moveT move);


  int CreateNewVeh_DTrip(int idCus);
  void deleteCusDTrip(int fromVeh, int fromTrip, int idCus);
  void insertCusDTrip(int insert_after, int IDinsert, int idVeh, int idTrip); //insert IDinsert after insert_after: note that insert_after is always a customer, so don't need tocheck if it is startSP or endSP or not
  void moveCusDTrip(int j, int i, int idVeh, int idTrip); //move customer i from its current place to place after j: both i and j are at the same trip
  void exchangeCusDTrip(int j, int i); //exchange customer j and its neighbor i: in 1 trip OR 2 trips
  void SUBexchangeCusDTrip(int idVeh, int idTrip, int prei, int suci, int i, int j, double varcost); //replace i by j
  void twoOPTstarCusDTrip_oneTrip(int j, int i, int idVeh, int idTrip); //2opt* between 2 customer j and its neighbor (i) where j and i are in the same trip
  void twoOPTstarCusDTrip(int j, int idVehj, int idTripj, int i, int idVehi, int idTripi); //2opt* between 2 customer j and its neighbor (i) where j and i belong to different trips
  void moveCusPTrip_OneTrip(int j, int i, int idVeh, int idTrip); //move customer i from its current place to place after j: both i and j are at the same trip
  void UpdateChangeCONNECTAtSP_PickTrip(int idVeh, int idTrip, double varcostpreT, double varvioTWC, int varvioCAP, double costTrip); //for twoOPTstarCusPTrip
  void UpdateChangeCONNECTAtSP_PickTrip(int idVeh, int idTrip, double varcost, double varcostpreT, double varvioTWC, int varvioCAP, int update_idTrip); //for moveCusPTrip_OneTrip, insertCusPTrip_TillTrip and deleteCusPTrip_TillTrip

  void UpdateChangeCONNECTAtSP_C2CTrip(int idVeh, int idTrip, int nextT, int nextSP, double varcost, double varcostpreT, double varvioTWC, int varvioCAP, int update_idTrip); //for deleteCusC2C

  void moveCusPTrip_OneVeh(int j, int toTrip, int i, int fromTrip, int fromVeh);//move cus i on (fromVeh, fromTrip) after cus j on (fromVeh, toTrip) where i and j are on the same vehicle fromVeh

  void deleteCusPTrip(int idVeh, int idTrip, int pre, int idCus, int next); //for the trip >= 2customers
  void deleteCusPTrip_FromBeginning(int idVeh, int idTrip, int pre, int idCus, int next); //for the trip >= 2customers
  void insertCusPTrip_FromBeginning(int insert_after, int IDinsert, int idVeh, int idTrip); //insert IDinsert after insert_after: note that insert_after is always a customer, so don't need tocheck if it is startSP or endSP or not
  void insertCusPTrip(int insert_after, int IDinsert,int idVeh, int idTrip, int update_idTrip);	//insert IDinsert after insert_after on (idVeh, idTrip) and update remaining trips till trip update_idTrip
  void deleteCusPTrip_TillTrip(int idVeh, int idTrip, int idCus, int update_idTrip); //delete cus idCus from (idVeh, idTrip) where idTrip >= 2 customers; and update remaining trips till trip update_idTrip	
  void delete_insertCusPTrip(int i, int idTripi, int j, int idTripj, int idVeh); //empty idTripi which is before idTripj, (NOT NECCESSARY RIGHT BEFORE), then insert i after j on trip idTripj, then DON TOA and update all remaining trips of idVeh

  void replaceCusPTrip_FromBeginning(int pre, int IDReplaced, int suc, int IDinsert, int lastCOLD, int idVeh, int idTrip); //insert IDinsert between pre and suc (delete the cus IDReplaced between pre and suc): if pre < 0 that means insert IDinsert at the beginning of the trip, and then update all remaining trips in the vehicle idVeh
  void replaceCusPTrip(int pre, int IDReplaced, int suc, int IDinsert, int lastCOLD, int idVeh, int idTrip, int update_idTrip); //insert IDinsert between pre and suc (delete the cus IDReplaced between pre and suc): if pre < 0 that means insert IDinsert at the beginning of the trip, and then update all remaining trips in the vehicle idVeh
  void exchangeCusPTrip_OneTrip(int j, int i, int idVeh, int Trip); //exchange cus i and j on (idVeh, idTrip)
  void exchangeCusPTrip_OneVeh(int idCusj, int idTripj, int idCusi, int idTripi, int idVeh); //exchange cus idCusj on (idVeh, idTripj) with cus idCusi on (idVeh, idTripi) of the same vehicle idVeh
  void twoOPTstarCusPTrip_oneTrip(int j, int i, int idVeh, int idTrip); //2opt* between 2 customer j and its neighbor (i) where j and i are in the same trip
  void twoOPTstarCusPTrip(int j, int idVehj, int idTripj, int i, int idVehi, int idTripi); //2opt* between 2 customer j and its neighbor (i) where j and i belong to different trips
 

  void deleteCusC2CTrip(int idVeh, int idTrip, int pre_deletednode, int deletednode, int next_deletednode); //for the trip with > 1 order
  void deleteCusC2CTrip_TillTrip(int idVeh, int idTripi, int pre_deletednode, int deletednode, int next_deletednode, int update_idTrip);
  void deleteCusC2CTrip_FromBeginning(int idVeh, int idTrip, int pre_deletednode, int deletednode, int next_deletednode); //for the trip with > 1 order

  void Update_InsertCusC2CIAfterJ(int j, int i, int idVeh, int idTrip);
  void insertCusC2CTrip(int j, int i, int cusDi_insert_After,int idVeh, int idTrip,int update_idTrip);	//insert i after j and delID of i after cusDi_insert_After on (idVeh, idTrip) and update remaining trips till trip update_idTrip
  void insertCusC2CTrip_FromBeginning(int j, int i, int cusDi_insert_After, int idVeh, int idTripj, int update_idTrip);
  void delete_insertCusC2CTrip(int i, int idTripi, int j, int cusDi_insert_After, int idTripj, int idVeh); //empty idTripi which is before idTripj, (NOT NECCESSARY RIGHT BEFORE), then insert i after j on trip idTripj, then DON TOA and update all remaining trips of idVeh

  void Update_InsertCusC2CTrip_2CusFirstTrip_nextD(int j, int i, int cusDi_insert_After, int idVeh, int idTrip, int nextSP, int update_idTrip);	
  void moveCusC2CTrip_OneTrip(int j, int i, int cusDi_insert_After, int idVeh, int idTrip);
  void moveCusC2CTrip_OneVeh(int j, int toTrip, int i, int fromTrip, int fromVeh, int cusDi_insert_After); //move cus i on (fromVeh, fromTrip) after cus j on (fromVeh, toTrip) where i and j are on the same vehicle fromVeh

  void exchangeCusC2CTrip_OneTrip(int j, int i, int idVeh, int idTrip);
  void exchangeCusC2CTrip_OneVeh(int j, int idTripj, int i, int idTripi, int idVeh);
  void replaceCusC2CTrip(int deleteCus, int idVeh, int idTrip, int insertCus);

  bool Update_EmptyTripP(int idVeh, int idTrip);
  bool Update_EmptyTripD(int idVeh, int idTrip);
  bool Update_EmptyTripPD(int idVeh, int idTrip);
  bool Update_EmptyTripC2C(int idVeh, int idTrip);

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



  void UpdateTripnum(int idVeh, int idTrip);
  void UpdateRouteTripnum(int idVeh, int idTrip);
  void UpdateVehicle(int idVeh, int curT, int preC, double cStartS, int varvioCAP, int update_idTrip);

  void UpdateBvioTWCus(int idVeh, int idTrip, int idCus); 
  void UpdateBvioTW_BvioCAPCus(int idVeh, int idTrip, int idCus); 
  void UpdateFBvioTWCus(int idVeh, int idTrip, int idCus); 
  void UpdateBvioCAPCus(int idVeh, int idTrip, int idCus); //when vioCAPCus[idCus] is changed
  void UpdateFBvioCAPCus(int idVeh, int idTrip, int idCus); //when vioTWCus[idCus] is changed
  

  void UpdateArray(int idVeh);
  void UpdateDeleteArray(int idVeh);
  void UpdateInsertArray(int idVeh);
  void UpdateMoveCusOneTripArray(int idVeh);
  void UpdateMoveCusPOneVehArray(int idVeh);
  void UpdateMoveCusC2COneVehArray(int idVeh);
  void UpdateExchangeArray(int idVeh);
  void UpdatetwoOPTstarArray(int idVeh);

  void checkEndSFirstPT(int idVeh);


 void pushElite(int PROB);
 int popElite();


 //Quangnn++
 void initSolution_Q(void);
 void initAssignment_Q(void);
 bool Create_C2CTrip_Q( int departSPIdx,			// departure SP index: can be DEPOT, SP, last C2C-delivery, last delivery
						double leaveDepartSPIdx,	// departure time at departSPIdx
						int endSPIdx,				// eclude C2C PICKUP/DELIVERY demands
						int inCurrVehCap,			// current vehicle capacity - reserved
						int inCurrVehIdx,			// Current vehicle index
						int inCurrTripIdx);			// Current trip index

//------------------------------------ CREATE INIT SOLUTION-----------------------------//
  void initSol(int typeofseed, int typeofINIT, int typeofAssCusPToSP, int typeofFEE);
  
  
  void CheckAndCreateSingleTrip(void);

  void AssignCusPToSP_Q(void); //Quangnn++ for specific for MTT-PDTWS
  void AssignCusPToSP(int typeofAssCusPToSP);
  bool Create_SequencePickupTripFullCAP(int startSP, int &endSP, int idVeh, int idTrip);
  
  bool CreateC2CTrip_BeforePickupTrip(int idVeh, int idTrip, int assSP, int preC, double depart_preC);
  bool CreateC2CTrip_BeforePickupTrip_Temp(int idVeh, int preT, int preC, double depart_preC, int &idTrip, int value_assign_idTrip, int idTripP, int assSP);
  bool CreateC2CTrip_BeforeSP_Temp(int idVeh, int preT, int preC, double depart_preC, int &idTrip, int value_assign_idTrip, int endSP);
	  
  bool checkCapacity_C2CTrip_InitSol(int fromC, int tillC, int CAPINS);
  bool checkCapacity_C2CTrip_InitSol_Temp(int fromC, int tillC, int CAPINS);
  bool checkC2C_P_FeasibleTrip_InitSol(int idVeh, int idTrip, int preC, double depart_preC, int nextT); //just checking feasible trip
  bool checkC2C_P_FeasibleTrip_InitSol_Temp(int idVeh, int idTrip, int preC, double depart_preC, int nextT); //just checking feasible trip
  bool checkC2C_FeasibleTrip_InitSol_Temp(int idVeh, int idTrip, int preC, double depart_preC, bool Add,int endSP); //just checking feasible trip
  bool checkFeasible_ConnectC2CDToSP(int curC, double depart_curC, double &costT, int endSP, bool &goWS);

  bool checkTripTWC2C_Temp(int curC, double departTime, int nextC, int pos_nextC, double *departTemp);

  void Create_FirstPickupTripLeavingDepot(int assignSP, int &numTrip);
  void CreateVehicle_C2COnly();
  bool checkPickupTripFeasible_AfterInsertC2C_Temp(int idCusD, double departTime, double &costT, int idVeh, int idTrip, bool &changed_idTrip);
  bool checkPickupTripFeasible_AfterInsertC2C(int idCusD, double departTime, double &costT, int idVeh, int idTrip, bool &changed_idTrip);
  bool checkFeasible_ConnectC2CDToSP_AfterInsertC2C(int lastC, double depart_lastC, double &costT, int lastCOLD, int endSP, int goWS, bool &changeWSORGoWS);
  
  
  bool checkC2CAndPTripFeasible_AfterInsertC2C(int curC, double departTime_curC, int nextC, int pos_nextC, double *departTemp, int idVeh, int idTrip);
  bool checkC2CAndPTripFeasible_AfterInsertC2C_Temp(int curC, double departTime_curC, int nextC, int pos_nextC, double *departTemp, int idVeh, int idTrip);
  bool checkC2CTripFeasible_AfterInsertC2C_Temp(int curC, double departTime_curC, int nextC, int pos_nextC, double *departTemp,int endSP);
  bool checkC2CVehicleFeasible(int curC, double departTime, int nextC, int pos_nextC, double *departTemp);

  bool checkC2CTripFeasibleCostChange_AfterInsertC2C_Temp(int curC, double departTime_curC, int nextC, double &costT, int idVeh, int idTrip, int endSP, bool &changeWSORGoWS);

  bool checkC2CAndPTripFeasibleCostChange_AfterInsertC2C_Temp(int curC, double departTime_curC, int nextC, double &costT, int idVeh, int idTrip, bool &changed_idTrip);
  bool checkC2CAndPTripFeasibleCostChange_AfterInsertC2C(int curC, double departTime_curC, int nextC, double &costT, int idVeh, int idTrip, bool &changed_idTrip);
  //return 0: already last delivery trip; 1: nextTrip will be delivery trip; 2: nextTrip will be pickup trip or trip between 2 sp 
  int Create_DeliveryTrip(int startSP, int& idTrip, double leave_startSP, int &nextSP, int typeofseed, int typeofINIT, int valueFEE); 	//Create a delivery trip (idVeh, idTrip) of sp assignSP where the time to leave assignSP is leave_startSP:
  void Init_NewSolomonI1_NewDeliveryTrip(int startSP, int endSP, int idTrip, double leavestartSP, double &feeMIN, int &nextSP, int &DelPick, int typeofseed, int valueFEE, int typeofINIT, int &canRoute);  //always create feasible leg
  
  int Create_PickupTrip(int idTrip_departSP, int departSP, int &numTrip, double leave_departSP, int &nextSP, int typeofseed, int typeofINIT, int valueFEE);
  void Init_NewSolomonI1_NewPickupTrip(int idTrip_departSP,int departSP, int endSP, int idTrip, double leave_departSP, int &nextSP, double &feeMIN, bool &NULLTrip, int typeofseed, int valueFEE, int typeofINIT, int &canRoute);


  void updateStartSP_FEASIBLETRIP_Temp(int idVeh, int idTrip, int preC, int idCus); //update startSP and departP array from idCus on feasible pickup trip(idVeh, idTrip): idCus is not the first customer
  void updateStartSP_FEASIBLETRIP(int idVeh, int idTrip, int preC, int idCus); //update startSP and departP array from idCus on feasible pickup trip(idVeh, idTrip): idCus is not the first customer
  void updateStartS_pos_FCAP_FEASIBLETRIP(int idVeh, int idTrip, int preC, int idCus, int pre_pos); //update startSP and departP array from idCus on feasible pickup trip(idVeh, idTrip): idCus is not the first customer
  void updateStartS_pos_FCAP_FEASIBLETRIP_Temp(int idVeh, int idTrip, int preC, int idCus, int pre_pos); //update startSP and departP array from idCus on feasible pickup trip(idVeh, idTrip): idCus is not the first customer
  void updatePTrip_InitSol(int idVeh, int idTrip, int preC, int idCus, bool changed);
  void updatePTrip_InitSol_Temp(int idVeh, int idTrip, int preC, int idCus, bool changed);
  /********************************************* CALCULATE INFORMATION *****************************************/
  void calCostFromBeginning();
  void CalInfromationTripD(int idVeh, int idTrip);  
  void CalInfromationTripP(int idVeh, int idTrip);  
  void CalInfromationTripC2C(int idVeh, int idTrip);  
  void calFitCostSumTrip();
  void UpdateCostFitnessFeasible();
  void UpdateCostFitnessFeasible1();

  void UpdatePos(int idVeh, int idTrip);
  bool checkCapacity_C2CTrip(int curC, int CAPINS);

  //------------------------------------------- SHOW SOLUTION-------------------------------//
  void showTripCost(int idVeh, int idTrip);
  void showTripCus(int idVeh, int idTrip);
  void showTripCusvioTW_C2C(int idVeh, int idTrip);
  void showTripCusTW(int idVeh, int idTrip);
  void showTripCusCAP(int idVeh, int idTrip);
  void showTripCostTemp(int idVeh, int idTrip);
  void showSolution(); 
  void updateBestSolution(int inSegIdx, int inIterations, double dbCost, char *szTestIdx, double dbBestAtTime, char *szTestOffset);
  void localSearchLeg(int idVeh, int idTrip);
  void localSearchLeg2(int idVeh, int idTrip);
  void postOptimzation(void);
  double getLegCost(int inStartNodeIdx, int inEndNodeIdx, int inWSId, int *arrPath, int pathSz);
  void showSolutionTempToCheck();
  void showSegment(int idVeh);
  void calReport();
  void exportSolution(void);
  //----------------------------------------- CHECK SOLUTION --------------------------------//
  bool checkSolution();
  bool checkVehicle(int idVeh);
  bool checkSolutionBuon();
  bool checkTripD_checkSolution(int idVeh, int idTrip);
  bool checkTripP_checkSolution(int idVeh, int idTrip);
  bool checkTripC2C_checkSolution(int idVeh, int idTrip);
  bool checkTripD_ALLFIELDS(int idVeh, int idTrip); //just checking field of trip; for both FEASIBLE and INFEASIBLE TRIP
  bool checkTripP_ALLFIELDS(int idVeh, int idTrip); //just checking field of trip; for both FEASIBLE and INFEASIBLE TRIP
  bool checkTripC2C_ALLFIELDS(int idVeh, int idTrip); //just checking field of trip; for both FEASIBLE and INFEASIBLE TRIP
  bool IsLIFO(int idVeh, int idTrip);

  
  void freeMEM(int nTypeCus);
protected:
  static Problem* problem; //a pointer to problem data
};

#endif



