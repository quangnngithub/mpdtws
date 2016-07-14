#include "PDS.h"
#include "Problem.h" //Quangnn++
using namespace std; 

Problem::Problem()
{
}

Problem::Problem(char *filename){
  //   This function has the following (side) effects:
  //   1.reads in a problem instance from file,
  //   2.computes the cost matrix

     ifstream infile;
	 int i, j, k;
	 int TOTALCAPP=0; //Total pickup-demands

#if Q_DEBUG_READ_INPUT
	 cout << "\n***READING DATA..." << endl;
	 cout << "\tFile name = " << filename << endl;
#endif
	 infile.open(filename,ios::in); 
     if (!infile){	 
		 cout << "\n\tInput file not found !!!!!!" << endl; getchar();
		 exit(-1);
	 }
	 else{//got file

		 /*
		  * First line contains the following information:
		  * nOrder: |R|	Number of requests = number of c2c-pickup customer demands =  number of c2c-delivery customer demands
		  * nP:	   |CP| 	Number of pickup customer demands
		  * nD:	   |CD|	Number of delivery customer demands
		  * Q:	  	Vehicle capacity
		  * nSP:	   |S|	Number of supply points  
		  * nWS	   |W|	Number of waiting stations
		  */
		 infile >> nOrder >> nP >> nD >> Q >> nSP >> nWS >> g_inFixedCost;
#if Q_DEBUG_READ_INPUT
		 cout << "\tNumber of C2C requests: "	  << nOrder << endl;
		 cout << "\tNumber of pickup-demands: "   << nP << endl;
		 cout << "\tNumber of delivery-demands: " << nD << endl;
		 cout << "\tVehicle capacity: "			  << Q << endl;
		 cout << "\tNumber of supply points: "    << nSP << endl;
		 cout << "\tNumber of waiting stations: " << nWS << endl;
		 cout << "\Fixed cost: "				  << g_inFixedCost << endl;
#endif

		 //Total number of c2c-customer demands = c2c-pickup-customer demands + c2c-delivery-customer demands
		 nC2C = nOrder * 2; 

		 // Set bound
		 nP1 = nP+1; nD1=nD+1; nSP1=nSP+1; nWS1=nWS+1; nC2C1 = nC2C + 1;
		 //Total nodes = c2c-demands + pickup-demands + delivery demands + supply points + waiting stations
		 int totalNODE = nC2C + nP + nD + nSP + nWS + 2; //TODO: constant value (2) indicates depots?
#if Q_DEBUG_READ_INPUT
		cout << "\tTOTAL NODES: " << totalNODE << endl;
#endif

		 node = new (nothrow) VRPNode[totalNODE];
		 if (node == 0){
			 cout << "\tError node: memory could not be allocated" << endl; getchar();
			 exit(-1);
		 }
		 
		 /*
		  * Second  line contains following information for the DEPOT:
		  * 0    	Representing the depot  
	      * X [0]	X axis for depot   
		  * Y [0]	Y axis for depot
		  */
         infile >> node[0].id >> node[0].x >> node[0].y;

    	 /*
		  * Next  |R| lines contain following information for c2c-pickup-customer demands:
		  *	p: Representing customer demand index
		  * X[p]: X axis for customer demand  
		  * Y[p]: Y axis for customer demand  
		  * q[p]: Demand at customer demand  
		  * e[p]: Ready time for customer demand  
		  * l[p]: Due time for customer demand  
		  * delta[p]: Service Time at customer demand  
		  */			 
		 startidC2C = nSP + 1; endidC2CP = startidC2C + nOrder - 1; // Set startID of C2C and endID of C2C-Pickup
		 for (i = startidC2C; i<=endidC2CP;i++){
              infile >> node[i].id;
			  infile>>node[i].x >> node[i].y;
			  infile >> node[i].capacity;
			  infile>>node[i].sTW>>node[i].eTW;
			  infile>>node[i].duration;

			  node[i].depart = node[i].eTW + node[i].duration;  //The lastest time the vehicle can be at the node TODO: purpose?
			  node[i].delID = i + nOrder;						//The linked nodeId for delivery in case of c2c-demands
			  node[node[i].delID].pickID = i;					//represents c2c-pickup-demand nodeId

			  //node[i].sTW = 0;
			  //node[i].eTW = 9000;
			  //cout << i << " " << node[i].sTW << " " << node[i].eTW << endl;
		 }

	
		 /*
		  * Next  |R| lines contain following information for c2c-delivery-customer demands:
		  *	d: Representing customer demand index
		  * X[d]: X axis for customer demand  
		  * Y[d]: Y axis for customer demand  
		  * q[d]: Demand at customer demand  
		  * e[d]: Ready time for customer demand  
		  * l[d]: Due time for customer demand  
		  * delta[d]: Service Time at customer demand  
		 */
		 startidC2CD = endidC2CP+1; endidC2C = startidC2CD+nOrder-1; // Set startID of C2C-Delivery and endID of C2C
		 for (i = startidC2CD; i<=endidC2C;i++){
              infile >> node[i].id;
			  infile>>node[i].x >> node[i].y;
			  infile >> node[i].capacity;
			  infile>>node[i].sTW>>node[i].eTW;
			  infile>>node[i].duration;

			  node[i].depart = node[i].eTW + node[i].duration; //The lastest time the vehicle can be at the node TODO: purpose?

//			  node[i].sTW = 0;
//			  node[i].eTW = 9000;
			  //cout << i << " " << node[i].sTW << " " << node[i].eTW << endl;
		 }

		 //cout<<"startC2CD = "<<startidC2CD<<"; endC2C = "<<endidC2C<<"; firstid = "<<node[startidC2CD].id<<"; lastid = "<<node[endidC2C].id<<endl;

		 nodeC2C = new (nothrow) NodeC2C[startidC2CD]; //TODO: Review, should be 2*nOrder
		 if (nodeC2C == 0){
			 cout << "\tError nodeC2C: memory could not be allocated"; getchar();
			 exit(-1);
		 }

		 /*
		  * Next  |C^P| lines contain following information for pickup-customer demands:
		  *	p: Representing customer demand index
		  * X[p]: X axis for customer demand  
		  * Y[p]: Y axis for customer demand  
		  * q[p]: Demand at customer demand  
		  * e[p]: Ready time for customer demand  
		  * l[p]: Due time for customer demand  
		  * delta[p]: Service Time at customer demand  
		  * |S_p|: cardinality of the set of admissible supply points for pickup-customer demand p (Number of admissible supply points that pickup-customer demand i can assigned to)
		  * Next |S_p|indexes  of supply points in the set |S_p| numbers
		  */
		 startidP = endidC2C + 1; endidP = startidP + nP-1; // Set start id and end if for pickup
		 int PC2C = endidP + 1;

		 //Allocate memory for pickup (2-dimension array)
		 belong = new bool *[PC2C];
		 for (i = startidC2C; i <= endidP; i++) belong[i] = new bool[nSP1];		 

		 for (i=startidP;i<=endidP;i++){
			 infile>>node[i].id;
			 infile>>node[i].x>>node[i].y;
			 infile>>node[i].capacity;
			 infile>>node[i].sTW>>node[i].eTW;
			 infile>>node[i].duration;
			 node[i].depart = node[i].eTW + node[i].duration;  //The latest time the vehicle can be at the node TODO: purpose?
			 infile>>node[i].numSP;							  // Number of possible supply points to serve pickup i
		
			 //cout << i << " " << node[i].sTW << " " << node[i].eTW << endl;

			 for (j=1;j<=nSP;j++) belong[i][j] = 0; //Initiate 
			 for (j=1;j<=node[i].numSP;j++){
				 infile>>node[i].sp[j];
				 belong[i][node[i].sp[j]] = 1;
			 }
			 TOTALCAPP += node[i].capacity;

//			 node[i].sTW = 0;
//			 node[i].eTW = 9000;
 		 }
         minTripCusP = (int) ceil((double) TOTALCAPP/Q);
#if Q_DEBUG_READ_INPUT
		 cout << "\tTotal PICKUP capacity: " << TOTALCAPP << endl;
		 cout << "\tMinimum trips to serve all PICKUP capacity: " << minTripCusP << endl;
#endif

		 /*
		  * Next  |C^D| lines contain following information for DELIVERY-CUSTOMER DEMANDS:
		  *	d: Representing customer demand index
		  * X[d]: X axis for customer demand  
		  * Y[d]: Y axis for customer demand  
		  * q[d]: Demand at customer demand  
		  * e[d]: Ready time for customer demand  
		  * l[d]: Due time for customer demand  
		  * delta[d]: Service Time at customer demand  
		  * s: index of the supply point to which delivery-customer demand d is assigned
		 */
		 MAXNUMCUSD=0;  //max number of delivery customers in 1 zone
		 int cursp=1;	//current supply point
		 int numCus=0;	//number of customer
		 startidD = endidP + 1; endidD = startidD + nD-1; //Set start id and end id for delivery
		 startCusDSP[cursp]=startidD; 
		 capDSP[cursp]=0;

		 for (i = startidD; i <= endidD; i++){
			 infile>> node[i].id;
			 infile>> node[i].x>>node[i].y;
			 infile>>node[i].capacity;
			 infile>>node[i].sTW>>node[i].eTW;
			 infile>>node[i].duration;
			 node[i].depart = node[i].eTW + node[i].duration;
			 infile>>node[i].zone;
			 //cout<<"node "<<i<<"(id = "<<node[i].id<<"): zone = "<<node[i].zone<<endl;

			 //cout << i << " " << node[i].sTW << " " << node[i].eTW << endl;
		
			 if (cursp == node[i].zone) {
				 numCus++;
				 capDSP[cursp] += node[i].capacity;
			 } else {
				endCusDSP[cursp]= i-1;
			    //cout<<"sp "<<cursp<<": startCus = "<<startCusDSP[cursp]<<"; endCus = "<<endCusDSP[cursp]<<endl;
				numCusDSP[cursp]=numCus; 
				if (numCusDSP[cursp] > MAXNUMCUSD) MAXNUMCUSD = numCusDSP[cursp];
				cursp = node[i].zone; numCus=1; capDSP[cursp] = node[i].capacity;
				startCusDSP[cursp]=i;
			 }

// 			 node[i].sTW = 0;
//			 node[i].eTW = 9000;

		 }
		 numCusDSP[cursp] = numCus; endCusDSP[cursp]=endidD;
		 if (numCusDSP[cursp] > MAXNUMCUSD) MAXNUMCUSD = numCusDSP[cursp];

		 for (i=1;i<=nSP;i++) minTripSP[i] = (int) ceil((double) capDSP[i]/Q);
#if Q_DEBUG_READ_INPUT
		 cout << "\tMinimum trips required for each supply point to delivery (SP | nbr): " << minTripCusP << endl;
		 for (i = 1; i <=nSP; i++){
			 cout << "\t\t" << i << "\t" << minTripSP[i] << endl;
		 }
#endif
		
		 /*
		  * Next |S| lines contain following information for each supply points:
		  * s		: Representing supply point index 
		  * X[s]		: X axis for supply point s  
		  * Y[s]		: Y axis for supply point s
		  * phi'(s)	: Unloading time at supply point s
		  * phi(s)	: loading time at supply point s
		  * t(n)-s	: Ready time at supply point s
		  * t(s)		: Due time at supply point s
		  */
		 nodeSP = new VRPNodeSP[nSP1];
		 for (i=1; i<=nSP; i++)
		 {
			 infile>>nodeSP[i].id;
			 infile>> nodeSP[i].x>>nodeSP[i].y;
			 infile>>nodeSP[i].unload>>nodeSP[i].load;
			 infile>>nodeSP[i].sTW1>>nodeSP[i].eTW1;
		
			 nodeSP[i].sTW2 = nodeSP[i].sTW1; 
			 nodeSP[i].eTW2 = nodeSP[i].eTW1;

// 			 nodeSP[i].sTW1 = 0;
//			 nodeSP[i].eTW1 = 9000;
// 			 nodeSP[i].sTW2 = 0;
//			 nodeSP[i].eTW2 = 9000;

			 //cout << i << " " << nodeSP[i].sTW1 << " " << nodeSP[i].eTW1 << endl;
		 }

		/*
		 * Next |W| lines contain the following information for each waiting stations:
		 * w  	Representing waiting station index  
		 * X[w]	X-Axis for waiting station w 
		 * Y[w]	Y-Axis for waiting station w 
		 */
		 nodeWS = new VRPNodeWS[nWS1];
		 for (i=1; i<=nWS; i++)
		 {
			 infile>>nodeWS[i].id;
             infile>> nodeWS[i].x>>nodeWS[i].y;
		 }

		 /****************************************
		  * COMPUTE THE COST MATRIX
		  ****************************************/
		 //Calculate leaveSP[sp i][j], j=0,1,2,3,4,5:
		 leaveSP = new double*[nSP1];
		 leaveSP[0] = new double [1]; leaveSP[0][0] = -1;
		 for (i=1;i<=nSP;i++) {
			 leaveSP[i] = new double [6];
			 leaveSP[i][0] = nodeSP[i].eTW1 + nodeSP[i].unload; //unload only at ETW
			 leaveSP[i][1] = nodeSP[i].eTW2 + nodeSP[i].load; //load only
			 leaveSP[i][2] = leaveSP[i][0] + nodeSP[i].load; //unload + load
			 leaveSP[i][3] = nodeSP[i].sTW1 + nodeSP[i].unload; //unload only at sTW
			 leaveSP[i][4] = nodeSP[i].sTW2 + nodeSP[i].load; //load only
			 leaveSP[i][5] = leaveSP[i][3] + nodeSP[i].load; //unload + load
		 }

		 int dem = 0; 
		 c = new (nothrow) double *[totalNODE];
		 if (c == 0){
			 cout << "Error c1: memory could not be allocated"; getchar();
			 exit(-1);
		 }
         for (i = 0; i < totalNODE; i++) {
			 c[i] = new double[totalNODE];
			 for (j=0;j<totalNODE;j++) c[i][j] = 0;
		 }

		 //CALCULATE DISTANCE:
		 //1. FOR DEPOT:
		 //1.1. Between DEPOT (0) and SupplyPoint (1 --> nSP)
		 for (i=1;i<=nSP;i++){
			c[0][i] = sqrt(pow((node[0].x - nodeSP[i].x),2) + pow((node[0].y - nodeSP[i].y),2));
            c[i][0] = c[0][i];		
		 }

		 //1.2 Between DEPOT and CUSTOMERS (P+D+C2C):
		 for (i=startidP; i <= endidP; i++){ //for P
			c[0][i] = sqrt(pow((node[0].x - node[i].x),2) + pow((node[0].y - node[i].y),2));
            c[i][0] = PDS_INFINITY;
		 }

		 for (i=startidC2C; i<= endidC2CP;i++){ //for C2CP
			 c[0][i] = sqrt(pow((node[0].x - node[i].x),2) + pow((node[0].y - node[i].y),2));
			 c[i][0] = PDS_INFINITY;
		 }

		 for (i = startidC2CD;i<=endidC2C;i++){ //for C2CD
			 c[0][i] = PDS_INFINITY; //can't from depot to C2CD
			 c[i][0] = sqrt(pow((node[0].x - node[i].x),2) + pow((node[0].y - node[i].y),2));
		 }

		 for (i=startidD; i <= endidD; i++){ //for D
			c[i][0] = sqrt(pow((node[0].x - node[i].x),2) + pow((node[0].y - node[i].y),2));
            c[0][i] = PDS_INFINITY;
		 }

		 //2. FOR CUSTOMERS:
		 //2.1 Distance between PICKUP customers:
		 for (i=startidP; i<endidP;i++)
           for (j=i+1;j<=endidP;j++)
		   {
			   if (sameListSP(i,j))		//has same SPs in possible list of SP
			     c[i][j] = sqrt(pow((node[i].x - node[j].x),2) + pow((node[i].y - node[j].y),2));
               else
			   	   c[i][j] = PDS_INFINITY;
			   c[j][i] = c[i][j];
		   }		

		 //2.2. Distance between DELIVERY customers:
		 for (i = startidD; i < endidD;i++)
           for (j = i+1;j <= endidD;j++)
		   {
			   if (node[i].zone == node[j].zone) // has same assigned SP
			     c[i][j] = sqrt(pow((node[i].x - node[j].x),2) + pow((node[i].y - node[j].y),2));
               else
			   	   c[i][j] = PDS_INFINITY;
			   c[j][i] = c[i][j];
		   }

		  //2.3. Distance between C2C:
		   int P1,P2,D1,D2;
		   for (P1 = startidC2C; P1 < endidC2CP;P1++)
		   {   
			   D1 = node[P1].delID; //linked delivery node of P1
			   for (P2 = P1+1;P2 <= endidC2CP; P2++)
			   {
				   D2 = node[P2].delID;
				   c[P1][P2] = sqrt(pow((node[P1].x - node[P2].x),2) + pow((node[P1].y - node[P2].y),2));
                   c[D1][D2] = sqrt(pow((node[D1].x - node[D2].x),2) + pow((node[D1].y - node[D2].y),2));
				   c[P1][D2] = sqrt(pow((node[P1].x - node[D2].x),2) + pow((node[P1].y - node[D2].y),2));
				   c[P2][D1] = sqrt(pow((node[P2].x - node[D1].x),2) + pow((node[P2].y - node[D1].y),2));
				   /*if (sameListSP(P1,P2)) sai
				   {
					   c[P1][P2] = sqrt(pow((node[P1].x - node[P2].x),2) + pow((node[P1].y - node[P2].y),2));
                       c[D1][D2] = sqrt(pow((node[D1].x - node[D2].x),2) + pow((node[D1].y - node[D2].y),2));
					   c[P1][D2] = sqrt(pow((node[P1].x - node[D2].x),2) + pow((node[P1].y - node[D2].y),2));
					   c[P2][D1] = sqrt(pow((node[P2].x - node[D1].x),2) + pow((node[P2].y - node[D1].y),2));
				   }
				   else
				   {
					   c[P1][P2] = PDS_INFINITY; c[D1][D2] = PDS_INFINITY;
					   c[P1][D2] = PDS_INFINITY; c[P2][D1] = PDS_INFINITY;
				   }*/
				   c[P2][P1] = c[P1][P2]; c[D2][D1] = c[D1][D2]; c[D2][P1] = c[P1][D2]; c[D1][P2] = c[P2][D1];
			   }
			   c[P1][D1] = sqrt(pow((node[P1].x - node[D1].x),2) + pow((node[P1].y - node[D1].y),2));
			   c[D1][P1] = PDS_INFINITY;
			   //c[P1][P1] = 0; c[D1][D1] = 0;
		   }
		   c[endidC2CP][endidC2C] = sqrt(pow((node[endidC2CP].x - node[endidC2C].x),2) + pow((node[endidC2CP].y - node[endidC2C].y),2));
		   c[endidC2C][endidC2CP] = PDS_INFINITY;
		   //c[endidC2CP][endidC2CP] = 0; c[endidC2C][endidC2C] = 0;

		   //2.4 Distance between pickup customers and delivery customers
		   for (i=startidP;i<=endidP;i++)
			   for (j=startidD; j<=endidD; j++)
			   {
				   c[j][i] = sqrt(pow((node[i].x - node[j].x),2) + pow((node[i].y - node[j].y),2));
				   c[i][j] = PDS_INFINITY;
			   }

           //2.5 Distance between C2C and pickup customers:
           for (i=startidP; i<=endidP;i++)
		   {
			   for (j=startidC2C; j <= endidC2CP; j++) //C2CP
			   {
				   c[i][j] = PDS_INFINITY;
				   c[j][i] = PDS_INFINITY;
			   }

			   for (j=startidC2CD; j <= endidC2C; j++) //C2CD
			   {
				   c[i][j] = PDS_INFINITY;
				   c[j][i] = sqrt(pow((node[i].x - node[j].x),2) + pow((node[i].y - node[j].y),2));
			   }
		   }			   

		   //2.6. Distance between C2C and delivery customers:
		   for (i=startidD; i <= endidD; i++)
		   {
			   for (j=startidC2C;j<=endidC2CP;j++) //C2CP
			   {
				   c[i][j] = sqrt(pow((node[i].x - node[j].x),2) + pow((node[i].y - node[j].y),2));
				   c[j][i] = PDS_INFINITY;
			   }
			   for (j = startidC2CD; j<=endidC2C;j++) //C2CD
			   {
				   c[i][j] = PDS_INFINITY;
				   c[j][i] = PDS_INFINITY;
			   }
		   }


         //2.7 Distance between customers and sp:
         //2.7.1 Distance between Pickup customers and sp:
         for (i=startidP; i <= endidP; i++)
		 {
			 for (j=1;j<=nSP;j++)
			 {
				 if (belong[i][j])
				     c[i][j] = sqrt(pow((node[i].x - nodeSP[j].x),2) + pow((node[i].y - nodeSP[j].y),2));
				 else c[i][j] = PDS_INFINITY;
				 c[j][i] = PDS_INFINITY;
			 }
		 }

		 //2.7.2 Distance between delivery customers and sp:
		 for (i = startidD; i <= endidD; i++)
			 for (j = 1; j <= nSP; j++)
			 {
				 if (node[i].zone == j) //delivery customer i belongs to sp j:
				 {
					 c[j][i] = sqrt(pow((node[i].x - nodeSP[j].x),2) + pow((node[i].y - nodeSP[j].y),2));
					 c[i][j] = PDS_INFINITY;
				 }
				 else //delivery customer i doesn't belong to sp j:
				 {
					 c[i][j] = sqrt(pow((node[i].x - nodeSP[j].x),2) + pow((node[i].y - nodeSP[j].y),2));
					 c[j][i] = PDS_INFINITY;
				 }
			 }

         //2.7.3 Distance between C2C and sp:
         int sp;
		 for (P1 = startidC2C; P1 <= endidC2CP; P1++)
		 {
			 D1 = node[P1].delID;
			 for (sp = 1; sp <= nSP; sp++)
			 {
				 c[sp][P1] = sqrt(pow((node[P1].x - nodeSP[sp].x),2) + pow((node[P1].y - nodeSP[sp].y),2));
				 c[D1][sp] = sqrt(pow((node[D1].x - nodeSP[sp].x),2) + pow((node[D1].y - nodeSP[sp].y),2));
				 c[P1][sp] = c[sp][P1];//c[P1][sp] = PDS_INFINITY;
				 c[sp][D1] = c[D1][sp]; //c[sp][D1] = PDS_INFINITY;
			 }
		 }

		 //2.8. Distance between WS and customers:
 		 startidWS = nSP + nP + nD + nC2C + 1; 
		 int wsid = startidWS;
		 for (j=1;j<=nWS;j++)
		 {
			 for (i=startidP; i<=endidP;i++)
			 {
				 c[i][wsid] = sqrt(pow((node[i].x - nodeWS[j].x),2) + pow((node[i].y - nodeWS[j].y),2));
				 c[wsid][i] = c[i][wsid];
			 }
			 for (i = startidD; i <= endidD; i++)
			 {
				 c[i][wsid] = sqrt(pow((node[i].x - nodeWS[j].x),2) + pow((node[i].y - nodeWS[j].y),2));
				 c[wsid][i] = PDS_INFINITY;
			 }
			 for (i = startidC2C; i <= endidC2CP; i++)
			 {
				 c[i][wsid] = PDS_INFINITY;
				 c[wsid][i] = PDS_INFINITY;
			 }
			 for (i = startidC2CD; i <= endidC2C; i++)
			 {
				 c[i][wsid] = sqrt(pow((node[i].x - nodeWS[j].x),2) + pow((node[i].y - nodeWS[j].y),2));
				 c[wsid][i] = PDS_INFINITY;
			 }
			 wsid++;
		 }

         //3. FOR SUPPLY POINTS:
		 //3.1. Distance between supply points:
         for (i=1;i<nSP;i++)
            for (j=i+1;j<=nSP;j++)
			{
				c[i][j] = sqrt(pow((nodeSP[i].x - nodeSP[j].x),2) + pow((nodeSP[i].y - nodeSP[j].y),2));
				c[j][i] = c[i][j];
			}

			 //4. FOR WAITING STATIONS:
			 //4.1 Distance between WS and sp:
			 
			 for (i=1;i<=nSP;i++)
			 {
				 k = startidWS;
				 for (j=1;j<=nWS;j++)
				 {
					 c[i][k] = sqrt(pow((nodeSP[i].x - nodeWS[j].x),2) + pow((nodeSP[i].y - nodeWS[j].y),2));
					 c[k][i] = c[i][k];
					 k++;
				 }
			 }
	}
		
	endidWS = startidWS + nWS - 1;

#if Q_DEBUG_READ_INPUT
	cout << "\n\tIndex:" << endl;
	cout << "\t\t Depot: 0 --> 0" << endl;
	cout << "\t\t Supply points: 1 --> " << nSP << endl;
	cout << "\t\t startidC2C: "<<  startidC2C  << endl;
	cout << "\t\t endidC2CP: "<<  endidC2CP  << endl;
	cout << "\t\t startidC2CD: "<<  startidC2CD  << endl;
	cout << "\t\t endidC2C: "<<  endidC2C  << endl;
	cout << "\t\t Pickup: "<<  startidP << " -->  " << endidP << endl;
	cout << "\t\t Delivery: "<<  startidD << " -->  " << endidD << endl;
	cout << "\t\t Waiting stations: " << startidWS << " -->  " << endidWS << endl;
	cout << "End of reading data" << endl;
#endif
}

//Re-construct Input data file to compare to origin input data
void Problem::ReconstructInputData(char *filename){
     ofstream infile;
	 int i, j, k;
	 int TOTALCAPP=0; //Total pickup-demands

#if Q_DEBUG
	 cout << "\WRITING DATA..." << endl;
	 cout << "\tFile name = " << filename << endl;
#endif

	 infile.open(filename,ios::out); 
     if (!infile){	 
		 cout << "\n\tInput file not found !!!!!!" << endl; 		
		 exit(-1);
	 }
	 else{//got file

		 /*
		  * First line contains the following information:
		  * nOrder: |R|	Number of requests = number of c2c-pickup customer demands =  number of c2c-delivery customer demands
		  * nP:	   |CP| 	Number of pickup customer demands
		  * nD:	   |CD|	Number of delivery customer demands
		  * Q:	  	Vehicle capacity
		  * nSP:	   |S|	Number of supply points  
		  * nWS	   |W|	Number of waiting stations
		  */
		 infile << nOrder << "\t" << nP << "\t" << nD << "\t" << Q << "\t" << nSP << "\t" << nWS <<endl;

		 //Total number of c2c-customer demands = c2c-pickup-customer demands + c2c-delivery-customer demands
		 nC2C = nOrder * 2; 

		 //Total nodes = c2c-demands + pickup-demands + delivery demands + supply points + waiting stations
		 int totalNODE = nC2C + nP + nD + nSP + nWS + 2; //TODO: constant value (2) indicates depots?

		 /*
		  * Second  line contains following information for the DEPOT:
		  * 0    	Representing the depot  
	      * X [0]	X axis for depot   
		  * Y [0]	Y axis for depot
		  */
         infile << node[0].id << "\t" << node[0].x << "\t" << node[0].y << endl;

    	 /*
		  * Next  |R| lines contain following information for c2c-pickup-customer demands:
		  *	p: Representing customer demand index
		  * X[p]: X axis for customer demand  
		  * Y[p]: Y axis for customer demand  
		  * q[p]: Demand at customer demand  
		  * e[p]: Ready time for customer demand  
		  * l[p]: Due time for customer demand  
		  * delta[p]: Service Time at customer demand  
		  */			 
		 startidC2C = nSP + 1; endidC2CP = startidC2C + nOrder - 1; // Set startID of C2C and endID of C2C-Pickup
		 for (i = startidC2C; i<=endidC2CP;i++){
              infile << node[i].id;
			  infile << "\t" <<node[i].x << "\t" << node[i].y;
			  infile << "\t" << node[i].capacity;
			  infile << "\t" <<node[i].sTW << "\t" <<node[i].eTW;
			  infile << "\t" <<node[i].duration << endl;

			  node[i].depart = node[i].eTW + node[i].duration;  //The lastest time the vehicle can be at the node TODO: purpose?
			  node[i].delID = i + nOrder;						//The linked nodeId for delivery in case of c2c-demands
			  node[node[i].delID].pickID = i;					//represents c2c-pickup-demand nodeId
		 }

	
		 /*
		  * Next  |R| lines contain following information for c2c-delivery-customer demands:
		  *	d: Representing customer demand index
		  * X[d]: X axis for customer demand  
		  * Y[d]: Y axis for customer demand  
		  * q[d]: Demand at customer demand  
		  * e[d]: Ready time for customer demand  
		  * l[d]: Due time for customer demand  
		  * delta[d]: Service Time at customer demand  
		 */
		 for (i = startidC2CD; i<=endidC2C;i++){
              infile << node[i].id;
			  infile << "\t" << node[i].x << "\t" << node[i].y;
			  infile << "\t" << node[i].capacity;
			  infile << "\t" << node[i].sTW << "\t" << node[i].eTW;
			  infile << "\t" << node[i].duration << endl;

		 }

		 /*
		  * Next  |C^P| lines contain following information for pickup-customer demands:
		  *	p: Representing customer demand index
		  * X[p]: X axis for customer demand  
		  * Y[p]: Y axis for customer demand  
		  * q[p]: Demand at customer demand  
		  * e[p]: Ready time for customer demand  
		  * l[p]: Due time for customer demand  
		  * delta[p]: Service Time at customer demand  
		  * |S_p|: cardinality of the set of admissible supply points for pickup-customer demand p (Number of admissible supply points that pickup-customer demand i can assigned to)
		  * Next |S_p|indexes  of supply points in the set |S_p| numbers
		  */
		 int PC2C = endidP + 1;

		 for (i=startidP;i<=endidP;i++){
			 infile << node[i].id;
			 infile << "\t" << node[i].x << "\t" << node[i].y;
			 infile << "\t" << node[i].capacity;
			 infile << "\t" << node[i].sTW << "\t" << node[i].eTW;
			 infile << "\t" << node[i].duration;
			 infile << "\t" << node[i].numSP;							  // Number of possible supply points to serve pickup i
		
			 for (j=1;j<=nSP;j++) belong[i][j] = 0; //Initiaze 
			 for (j=1;j<=node[i].numSP;j++){
				 infile << "\t" << node[i].sp[j];
				 //belong[i][node[i].sp[j]] = 1;
			 }
			 infile << endl;

			 //cout << i << " " << node[i].sTW << " " << node[i].eTW << endl;
 		 }

		 /*
		  * Next  |C^D| lines contain following information for DELIVERY-CUSTOMER DEMANDS:
		  *	d: Representing customer demand index
		  * X[d]: X axis for customer demand  
		  * Y[d]: Y axis for customer demand  
		  * q[d]: Demand at customer demand  
		  * e[d]: Ready time for customer demand  
		  * l[d]: Due time for customer demand  
		  * delta[d]: Service Time at customer demand  
		  * s: index of the supply point to which delivery-customer demand d is assigned
		 */
		 int cursp=1;	//current supply point
		 int numCus=0;	//number of customer

		 for (i = startidD; i <= endidD; i++){
			 infile << node[i].id;
			 infile << "\t" << node[i].x << "\t" << node[i].y;
			 infile << "\t" << node[i].capacity;
			 infile << "\t" << node[i].sTW << "\t" << node[i].eTW;
			 infile << "\t" << node[i].duration;
			 infile << "\t" << node[i].zone <<endl;

			 //cout << i << " " << node[i].sTW << " " << node[i].eTW << endl;
		 }
		
		 /*
		  * Next |S| lines contain following information for each supply points:
		  * s		: Representing supply point index 
		  * X[s]		: X axis for supply point s  
		  * Y[s]		: Y axis for supply point s
		  * phi'(s)	: Unloading time at supply point s
		  * phi(s)	: loading time at supply point s
		  * t(n)-s	: Ready time at supply point s
		  * t(s)		: Due time at supply point s
		  */
		 for (i=1; i<=nSP; i++)
		 {
			 infile << nodeSP[i].id;
			 infile << "\t" << nodeSP[i].x << "\t" << nodeSP[i].y;
			 infile << "\t" << nodeSP[i].unload << "\t" << nodeSP[i].load;
			 infile << "\t" << nodeSP[i].sTW1 << "\t" << nodeSP[i].eTW1 << endl;

			 //cout << i << " " << node[i].sTW << " " << node[i].eTW << endl;
		 }

		/*
		 * Next |W| lines contain the following information for each waiting stations:
		 * w  	Representing waiting station index  
		 * X[w]	X-Axis for waiting station w 
		 * Y[w]	Y-Axis for waiting station w 
		 */
		 for (i=1; i<=nWS; i++)
		 {
			 infile << nodeWS[i].id;
             infile << "\t" << nodeWS[i].x << "\t" << nodeWS[i].y <<endl;
		 }

	 }
}

void Problem::CalculateWSAssignment_Q()
{
	double temp,minD; int minWS;

	// Reset
	for (int i = startidC2C; i <= endidD; i++)
	{
		for (int j = 1; j <= nSP; j++)
		{
			CWSSP[i][j] = -1;
			disCWSSP[i][j] = PDS_INFINITY;
		}
	}

	// For pickup
	for (int i = startidP; i <= endidP; i++)
	{
		for (int j = 1; j <= nSP; j++)
		{
			//bool bServed = false;
			//for (int kk = 1; kk <= node[i].numSP; kk++)
			//{
			//	if (belong[i][node[i].sp[kk]] == 1)
			//	{
			//		bServed = true;
			//		break;
			//	}
			//}
			//if (true == bServed) continue;

			minD = PDS_INFINITY; 
			minWS = -1;
 		    for (int k = startidWS; k <= endidWS; k++)
			 {
				  temp = c[i][k] + c[k][j];
				  if (minD > temp)
				  {
					  minD = temp;
					  minWS = k;
				  }
			 }
			  CWSSP[i][j] = minWS; disCWSSP[i][j]=minD;
		}
	}

	// For delivery
	for (int i = startidD; i <= endidD; i++)
	{
		for (int j = 1; j <= nSP; j++)
		{
			if (node[i].zone == j) continue;

			minD = PDS_INFINITY; 
			minWS = -1;
 		    for (int k = startidWS; k <= endidWS; k++)
			 {
				  temp = c[i][k] + c[k][j];
				  if (minD > temp)
				  {
					  minD = temp;
					  minWS = k;
				  }
			 }
			  CWSSP[i][j] = minWS; disCWSSP[i][j]=minD;
		}
	}

	// For C2C-Delivery
	for (int i = startidC2CD; i<= endidC2C; i++)
	{
		for (int j = 1; j <= nSP; j++)
		{
			minD = PDS_INFINITY; 
			minWS = -1;
 		    for (int k = startidWS; k <= endidWS; k++)
			 {
				  temp = c[i][k] + c[k][j];
				  if (minD > temp)
				  {
					  minD = temp;
					  minWS = k;
				  }
			 }
			  CWSSP[i][j] = minWS; disCWSSP[i][j]=minD;
		}
	}



}
void Problem::CalculatePWSSP()
{
    int i,j,k,l;
	double temp,minD; int minWS;
	int inCntTmp = 0;
	for (i=startidP;i<=endidP;i++)
	{
	   for (j=1;j<=nSP;j++) disCWSSP[i][j] = PDS_INFINITY;
	   for (l=1;l<=node[i].numSP;l++)
	   {
			  j = node[i].sp[l];
			  minD = PDS_INFINITY;
			  for (k = startidWS; k <= endidWS;k++)
			  {
				  temp = c[i][k] + c[k][j];
				  if (minD > temp)
				  {
					  minD = temp;
					  minWS = k;
				  }
			  }
			  CWSSP[i][j] = minWS; disCWSSP[i][j]=minD;
	   }
	}

#if Q_DEBUG_CALC_WS_ASSIGN
	cout << "CalculatePWSSP: " << endl;
#endif
	for (i=startidP;i<=endidP;i++)
	{
		inCntTmp = 0;
		for (j=1;j<=nSP;j++)
		{			
			if (0 > CWSSP[i][j])
			{
				inCntTmp++;
			} else
			{
#if Q_DEBUG_CALC_WS_ASSIGN
				cout << i << "->" << j << ": " << CWSSP[i][j] << endl;
#endif
			}
		}
		if (inCntTmp == nSP)
		{
			//cout << i << " can not find parking *ln" << __LINE__ << endl; getchar();
		}

	}
}

void Problem::CalculateDWSSP()
{
    int i,j,k,l,start,end;
	int inCntTmp;
	double temp,minD; int minWS;

	for (i=1; i<nSP; i++)
	{
		start=startCusDSP[i];
		end = endCusDSP[i];//end=start+numCusSP[i]-1;
		for (l=i+1; l<=nSP; l++)
		{
			for (j=start;j<=end;j++)
			{
				minD = PDS_INFINITY;
                for (k=startidWS;k<=endidWS;k++)
		        {
			     	temp=c[j][k]+c[k][l];
			        if (minD >temp)
			        {
				         minD = temp;
                         minWS = k;
			        }
		        }
			    CWSSP[j][l] = minWS; disCWSSP[j][l] = minD;
			}
  		}
	}

#if Q_DEBUG_CALC_WS_ASSIGN
	cout << "CalculateDWSSP: " << endl;
#endif
	for (i=startidD;i<=endidD;i++)
	{
		inCntTmp = 0;
		for (j=1;j<=nSP;j++)
		{			
			if (0 > CWSSP[i][j])
			{
				inCntTmp++;
			} else
			{
#if Q_DEBUG_CALC_WS_ASSIGN
				cout << i << "->" << j << ": " << CWSSP[i][j] << endl;
#endif
			}
		}
		if (inCntTmp == nSP)
		{
			//cout << i << " can not find parking *ln" << __LINE__ << endl; // getchar();
		}

	}

}


void Problem::CalculateSPWSSP()
{
    int i,j,k;
	double temp,minD; int minWS;
    
	for (i=1;i<=nSP;i++)
	  for (j=1;j<=nSP;j++)
	  {
		  minD = PDS_INFINITY;
		  for (k = startidWS; k <= endidWS;k++)
		  {
			  temp = c[k][i] + c[k][j];
			  if (minD > temp)
			  {
				  minD = temp;
				  minWS = k;
			  }
		  }
		  CWSSP[i][j] = minWS; disCWSSP[i][j]=minD;
	  }

}




void Problem::CalculateC2CDWSSP()
{
	int inCntTmp;
    int i,j,k,l, idC2CP;
	double temp,minD; int minWS;

	idC2CP = startidC2C;
	for (i=startidC2CD;i<=endidC2C;i++)
	{
	   for (j=1;j<=nSP;j++) disCWSSP[i][j] = PDS_INFINITY;
	   for (l=1;l<=node[idC2CP].numSP;l++)
	   {
			  j = node[idC2CP].sp[l];
			  minD = PDS_INFINITY; minWS = -15;
			  for (k = startidWS; k <= endidWS;k++)
			  {
				  temp = c[i][k] + c[k][j];
				  if (minD > temp)
				  {
					  minD = temp;
					  minWS = k;
				  }
			  }
			  CWSSP[i][j] = minWS; disCWSSP[i][j]=minD;
	   }
	   idC2CP++;
	}
#if Q_DEBUG_CALC_WS_ASSIGN
	cout << "CalculateC2CDWSSP: " << endl;
#endif
	for (i=startidC2CD;i<=endidC2C;i++)
	{
		inCntTmp = 0;
		for (j=1;j<=nSP;j++)
		{			
			if (0 > CWSSP[i][j])
			{
				inCntTmp++;
			} else
			{
#if Q_DEBUG_CALC_WS_ASSIGN
				cout << i << "->" << j << ": " << CWSSP[i][j] << endl;
#endif
			}
		}
		if (inCntTmp == nSP)
		{
			//cout << i << " can not find parking *ln" << __LINE__ << endl; //getchar();
		}

	}

}





void Problem::CalculateClosestPSP() //for each sp, calculate the list stored its closest pickup customers: closestPSP[sp][#i] = idCus that is ith closest to supply point sp
{
    int i,j,k;
    int id[1000], idT[1000];
	double dis[1000];
	for (i=1;i<=nSP;i++)
	{
		numCusPAvailSP[i] = 0;
		for (j=startidP;j<=endidP;j++)
		  for (k=1;k<=node[j].numSP;k++)
			  if (node[j].sp[k]==i)
			  {
                  numCusPAvailSP[i]++;
				  id[numCusPAvailSP[i]]=j;
				  dis[numCusPAvailSP[i]]=c[j][i];
				  break;
			  }
        for (j=1;j<=numCusPAvailSP[i];j++) idT[j]=j;
		quick_sort(dis, idT, 1,numCusPAvailSP[i]);
		for (j=1;j<=numCusPAvailSP[i];j++)
		{
			closestPSP[i][j] = id[idT[j]];
			if (closestPSP[i][j] < startidP && closestPSP[i][j] > endidP)
				cout<<"ERROR: closestPSP[sp ="<<i<<"]["<<j<<"]  = "<<closestPSP[i][j]<<endl;
		}
	}
}



void Problem::CalculateClosestSPOfP() //for each pickup customer, calculate the list ordering closest sp that it can assigned to & stored it in to array: nodeP[idCus].sp[#i]: = idsp that is ith closest to idCus
{
    int i,j;
    int id[50], idT[50];
	double dis[50];

 	for (i=startidP;i <= endidP;i++)
	{
		for (j=1;j<=node[i].numSP;j++) 
		{
			dis[j] = c[i][node[i].sp[j]];
			id[j] = j;
			idT[j] = node[i].sp[j];
		}
		quick_sort(dis, id, 1, node[i].numSP);
		for (j=1;j<=node[i].numSP;j++)
			node[i].sp[j] = idT[id[j]];
	}
}



void Problem::CalculateSoonestLatestPD() //calculate soonest and latest service time at a customer so that the vehicle can go directly from this customer to a sp
{
	int i,j,sp; double dis,a,b;
	for (i=startidP;i<=endidP;i++)
	{
		for (j=1;j<=node[i].numSP;j++)
		{
			sp = node[i].sp[j];
			dis = node[i].duration + c[i][sp];
		    a = nodeSP[sp].sTW1 - dis;
		    b = nodeSP[sp].eTW1 - dis;
			if (node[i].eTW >=a && node[i].sTW <=b) //customer i can go directly to supply point sp
			{
			   //CSPDirect[i][sp] = 1;
			   //soonestC[i][sp] = PDS_MAX(a,node[i].sTW);
			   //latestC[i][sp] = PDS_MIN(b, node[i].eTW);
			}
		}
	}

	for (i=1;i<nSP;i++)
	{
		for (j=startCusDSP[i]; j<=endCusDSP[i]; j++)
		{
			for (sp=i+1;sp<=nSP;sp++)
			{
				dis = node[j].duration + c[j][sp];
				a = nodeSP[sp].sTW1 - dis;
				b = nodeSP[sp].eTW1 - dis;
				if (node[j].eTW >=a && node[j].sTW <=b) //delivery customer j can go directly to supply point sp
				{
				   //CSPDirect[j][sp] = 1;
				  // soonestC[j][sp] = PDS_MAX(a,node[j].sTW);
				  // latestC[j][sp] = PDS_MIN(b, node[j].eTW);
				}
			}
		}
	}
}



void Problem::calstartSOneP_OR_OneC2C()
{
     int i, j, assSP;
	 double ArrTime, cArrTime, tt, cArrTime1;
	 int totalNODE1 = nSP + nC2C + nP + 1;
     startSOneP = new double *[totalNODE1];
	 waitingSOneP = new bool *[totalNODE1];
	 leaveSPOneP = new double *[totalNODE1];
	 for (i = 0; i< totalNODE1; i++)
	 {
		 startSOneP[i] = new double [nSP1];
		 leaveSPOneP[i] = new double [nSP1];
		 waitingSOneP[i] = new bool [nSP1];
		 for (j = 1; j < nSP1; j++)
		 {
			 startSOneP[i][j] = -1;
			 leaveSPOneP[i][j] = -1;
			 waitingSOneP[i][j] = 0;
		 }
	 }

	 for (i=startidP; i <= endidP; i++)
	 {
		 /*startSOneP[i] = new double [nSP1];
		 leaveSPOneP[i] = new double [nSP1];
		 waitingSOneP[i] = new bool [nSP1];*/
		 for (j=1;j <= node[i].numSP; j++)
		 {
			assSP = node[i].sp[j];
			cArrTime = PDS_MAX(c[0][i], node[i].sTW);
			ArrTime = cArrTime + node[i].duration + c[i][assSP];
			if (!PDSINRANGE(ArrTime, nodeSP[assSP].sTW1, nodeSP[assSP].eTW1))
			{
				 //Arrive here means: ArrTime < sTW1
				 cArrTime1 = nodeSP[assSP].sTW1 - c[i][assSP] - node[i].duration;
				 if (PDSINRANGE(cArrTime1, node[i].sTW, node[i].eTW))
				 {
					 cArrTime = cArrTime1;
					 waitingSOneP[i][assSP] = 0;
					 leaveSPOneP[i][assSP] = leaveSP[assSP][3]; //unload only at sTW1
				 }
				 else
				 {			 
					 ArrTime = cArrTime + node[i].duration + disCWSSP[i][assSP];
				     tt = ArrTime - nodeSP[assSP].eTW1;
				     if (tt > eps) //too soon to sp directly, too late through WS:
					 { 
						 cout<<"EXIT: vioTWCus pickup "<<node[i].id<<" arrive at sp "<<assSP<<endl;
						 cout<<"sTWCus = "<<node[i].sTW<<"; dur = "<<node[i].duration<<endl;
						 cout<<"c[cus][sp] = "<<c[i][assSP]<<"; dis(cus,sp) = "<<disCWSSP[i][assSP]<<endl;
						 cout<<"[stw,etw] of SP = ["<<nodeSP[assSP].sTW1<<", "<<nodeSP[assSP].eTW1<<"]"<<endl;
						 exit(-1);
					 }
					 else
					 {
						 waitingSOneP[i][assSP] = 1;
						 leaveSPOneP[i][assSP] = PDS_MAX(ArrTime, nodeSP[assSP].sTW1) + nodeSP[assSP].unload;
						 //cout<<"through wS: tt = "<<tt<<"; cArrTime = "<<cArrTime<<"; ArrTime = "<<ArrTime<<"; eTW1 = "<<problem->nodeSP[assSP].eTW1<<endl;
	     				 //seg[idVeh].Trip[0].waitingS = problem->CWSSP[curC][assSP];
					 }
				 }
			}
			else 
			{
				waitingSOneP[i][assSP] = 0;
				leaveSPOneP[i][assSP] = ArrTime + nodeSP[assSP].unload;
			}
			startSOneP[i][assSP] = cArrTime;
			//cout<<"waitingSOneP["<<i<<"]["<<assSP<<"] = "<<waitingSOneP[i][assSP]<<endl;
		 } //end j
	 }//end i

	 //2. Calculate for vehicle with first trip is C2C, and this trip has only one Order:
	 int k, delID; double cArrTimeP, cArrTimeD, cArrTime2;
	 for (i=startidC2C; i <= endidC2CP; i++)
	 {
		 /*startSOneP[i] = new double [nSP1];
		 leaveSPOneP[i] = new double [nSP1];
		 waitingSOneP[i] = new bool [nSP1];*/
		 for (assSP=1;assSP <= nSP;assSP++)
		 {
			 bool list = 0;
			 for (k=1;k<=node[i].numSP; k++)
				 if (node[i].sp[k] == assSP) {list = 1; break;}
 			 delID = node[i].delID;
			 //startSOneP[delID] = new double [nSP1];
			 cArrTimeP = PDS_MAX(c[0][i], node[i].sTW);
			 cArrTimeD = PDS_MAX(cArrTimeP + node[i].duration + c[i][delID], node[delID].sTW);
			 if (list)
			 {
				ArrTime = cArrTimeD + node[delID].duration + c[delID][assSP];
				if (!PDSINRANGE(ArrTime, nodeSP[assSP].sTW2, nodeSP[assSP].eTW2))
				{
					 //Arrive here means: ArrTime < sTW1
					 cArrTime1 = nodeSP[assSP].sTW2 - c[delID][assSP] - node[delID].duration;
					 if (PDSINRANGE(cArrTime1, node[delID].sTW, node[delID].eTW))
					 {
						 cArrTime2 = cArrTime1 - c[i][delID]- node[i].duration;
						 if (PDSINRANGE(cArrTime2, node[i].sTW, node[i].eTW))
						 {
						     cArrTimeP = cArrTime2; cArrTimeD = cArrTime1;
						     waitingSOneP[i][assSP] = 0;
						     leaveSPOneP[i][assSP] = leaveSP[assSP][4]; //load only at sTW2
						 }
						 else
						 {
							 ArrTime = cArrTimeD + node[delID].duration + disCWSSP[delID][assSP];
							 tt = ArrTime - nodeSP[assSP].eTW2;
							 if (tt > eps) //too soon to sp directly, too late through WS:
							 { 
								 cout<<"EXIT: vioTWCusC2C "<<i<<" arrive at sp "<<assSP<<endl;
								 exit(-1);
							 }
							 else
							 {
								 waitingSOneP[i][assSP] = 1;
								 leaveSPOneP[i][assSP] = PDS_MAX(ArrTime, nodeSP[assSP].sTW2) + nodeSP[assSP].load;
								 //cout<<"through wS: tt = "<<tt<<"; cArrTime = "<<cArrTime<<"; ArrTime = "<<ArrTime<<"; eTW1 = "<<problem->nodeSP[assSP].eTW1<<endl;
	     						 //seg[idVeh].Trip[0].waitingS = problem->CWSSP[curC][assSP];
							 }
						 }
					 }
					 else
					 {			 
						 ArrTime = cArrTimeD + node[delID].duration + disCWSSP[delID][assSP];
						 tt = ArrTime - nodeSP[assSP].eTW2;
						 if (tt > eps) //too soon to sp directly, too late through WS:
						 { 
							 cout<<"EXIT: vioTWCusC2C "<<i<<" arrive at sp "<<assSP<<endl;
							 exit(-1);
						 }
						 else
						 {
							 waitingSOneP[i][assSP] = 1;
							 leaveSPOneP[i][assSP] = PDS_MAX(ArrTime, nodeSP[assSP].sTW2) + nodeSP[assSP].load;
							 //cout<<"through wS: tt = "<<tt<<"; cArrTime = "<<cArrTime<<"; ArrTime = "<<ArrTime<<"; eTW1 = "<<problem->nodeSP[assSP].eTW1<<endl;
	     					 //seg[idVeh].Trip[0].waitingS = problem->CWSSP[curC][assSP];
						 }
					 }
				}
				else 
				{
					waitingSOneP[i][assSP] = 0;
					leaveSPOneP[i][assSP] = ArrTime + nodeSP[assSP].load;
				}
				startSOneP[i][assSP] = cArrTimeP; startSOneP[delID][assSP] = cArrTimeD;
			 }
			 else //not in the list: it means doesn't exist C2C - SP
			 {
				 startSOneP[i][assSP] = cArrTimeP;
				 startSOneP[delID][assSP] = cArrTimeD;
				 waitingSOneP[i][assSP] = 0;
				 leaveSPOneP[i][assSP] = nodeSP[assSP].eTW2;
			 }//end not in the list: it means doesn't exist C2C - SP
		 }//end for assSP
	 }//end i
}

void Problem::CalculateStartDepart_Depot() //for pickup customers and C2CP customers:
{
	int i; double temp;
	for (i=startidP;i<=endidP;i++)
	{
		temp = node[i].sTW - c[0][i];
		if (temp > eps)
		{
			node[i].startS_depot = node[i].sTW;
			node[i].depart_depot = node[i].sTW + node[i].duration;
		}
		else
		{
			node[i].startS_depot = c[0][i];
			node[i].depart_depot = c[0][i] + node[i].duration;
		}
	}
	//For C2CP customers:
	for (i=startidC2C;i<= endidC2CP;i++)
	{
		temp = node[i].sTW - c[0][i];
		if (temp > eps)
		{
			node[i].startS_depot = node[i].sTW;
			node[i].depart_depot = node[i].sTW + node[i].duration;
		}
		else
		{
			node[i].startS_depot = c[0][i];
			node[i].depart_depot = c[0][i] + node[i].duration;
		}
	}
}


void Problem::CalculateAssignC2CToSP(int &count)
{
   int cusD,cusP,k,sp,countsp;
   double cArrTime, temp, cArrTime1, cArrTime2;
   cusD = startidC2CD; cusP = startidC2C;

   /*return;
   cout << "CALCULATE assignment C2C to supply points: SKIPPED" << endl;
   */

   for (k = 1; k <= nOrder; k++)
   {
	   countsp = 0;
	   //cout<<"ORDER "<<k<<" assign to SP: ";
	   //belong[cusP] = new bool[nSP1];
	   cArrTime1 = node[cusD].sTW + node[cusD].duration;
	   cArrTime2 = node[cusP].sTW + node[cusP].duration;
	   for (sp=1;sp<=nSP;sp++)
	   {
			cArrTime = cArrTime1 + c[cusD][sp];
			temp = cArrTime - nodeSP[sp].eTW2;
			if (temp > eps) //can't assign C2C to sp
			{
				//cout<<"NOT sp "<<sp<<" due to cusD("<<node[cusD].id<<": depart = "<<cArrTime1<<"; c = "<<c[cusD][sp]<<", arrivesp = "<<cArrTime<<"; eTWsp = "<<nodeSP[sp].eTW2<<endl;
				belong[cusP][sp] = 0;
			}
			else //can assign C2C to sp:
			{
				cArrTime = cArrTime2 + c[cusP][sp];
				temp = cArrTime - nodeSP[sp].eTW2;
				if (temp > eps) //can't assign c2c to sp
				{
					//cout<<"NOT sp "<<sp<<" due to cusP("<<node[cusP].id<<"): depart = "<<cArrTime2<<";c = "<<c[cusP][sp]<<"; arrivesp = "<<cArrTime<<"; eTWsp = "<<nodeSP[sp].eTW2<<endl;
					belong[cusP][sp] = 0;
				}
				else
				{
					cArrTime = PDS_MAX(cArrTime2 + c[cusP][cusD], node[cusD].sTW) + node[cusD].duration + c[cusD][sp];
					temp = cArrTime - nodeSP[sp].eTW2;
					if (temp > eps) belong[cusP][sp] = 0;
					else 
					{
						belong[cusP][sp] = 1;
					    //cout<<sp<<", ";
					    countsp++;
					    node[cusP].sp[countsp] = sp;
					}
				}
			}
	   }
	   //cout<<endl;
	   node[cusP].numSP = countsp;
	   bool ASS = 0;
	   for (sp = nSP; sp >= 1; sp--)
	   {
		   temp = node[cusP].eTW - (leaveSP[sp][1] + c[sp][cusP]);
		   if (temp > eps) {node[cusP].zone = sp; ASS = 1; break;}
	   }
	   if (ASS == 0)
	   {
		   //cout<<"ERROR: For C2C "<<node[j].id<<": there doesnt exist any SP such that vehicle can go SP - C2C("<<j<<")"<<endl; 
		   cout<<"ERROR: For C2CP "<<node[cusP].id<<": [sTW, eTW] = ["<<node[cusP].sTW<<", "<<node[cusP].eTW<<"] C2CD "<<node[cusD].id<<": [sTW, eTW] = ["<<node[cusD].sTW<<", "<<node[cusD].eTW<<"]"<<endl;
		   count++;
	   }
	   cusP++;cusD++;
   }
#if Q_DEBUG
 	cout<<"List of C2C cus can assign to SP "<<endl;
	for (sp = 1; sp<= nSP; sp++)
	{
		cout<<"SP "<<sp<<": ";
		for (cusP = startidC2C;cusP <= endidC2CP; cusP ++)
	        if (belong[cusP][sp]) cout<<node[cusP].id<<", ";
		cout<<endl;
	}
#endif
	//for (int i = startidC2C; i <= endidC2CP; i++)
	//{
	//	double dbEarlyTime = node[i].sTW + node[i].duration;
	//	double dbLateTime = node[i].eTW + node[i].duration;

	//	int inDEnode = i + nOrder;

	//	dbEarlyTime += c[i][inDEnode];
	//	dbLateTime += c[i][inDEnode];

	//	if ((dbEarlyTime > node[inDEnode].eTW) || (dbLateTime < node[inDEnode].sTW))
	//	{
	//		cout << "[ERROR] C2CP " << i << "[" << node[i].sTW << "," << node[i].eTW  << "] --> ";
	//		cout << "C2CD " << inDEnode <<  "[" << node[inDEnode].sTW << "," << node[inDEnode].eTW << "]";
	//		cout << " : " << c[i][inDEnode] << endl;
	//	}
	//}
}
/*
 * Assign C2C (pickup/delivery) to SP
 * count: number of can-not-serve C2C
 */
void Problem::CalculateAssignC2CToSP_Q(int &count)
{
   int cusD,cusP,k,sp,countsp;
   double cArrTime, temp, cArrTime1, cArrTime2;
   
   cout << "Calculate assignment C2C to supply points: " << endl;
   
   cusD = startidC2CD; cusP = startidC2C;
   for (k = 1; k <= nOrder; k++) // nOrder: number of c2c-customer-demand
   {
	   countsp = 0;
#if Q_DEBUG
	   cout << "\tconsidering..(" << cusD << "," << cusP << ")" << endl;
#endif
	   cArrTime1 = node[cusD].sTW + node[cusD].duration; // the earliest time to leave after DELIVERY c2c-customer-demand(delivery)
	   cArrTime2 = node[cusP].sTW + node[cusP].duration; // the earliest time to levae after PICKUP c2c-customer-demand(pickup)
	   
	   //
	   for (sp=1;sp<=nSP;sp++)
	   {
			cArrTime = cArrTime1 + c[cusD][sp];
			temp = cArrTime - nodeSP[sp].eTW2;
			if (temp > eps) //can't assign C2C to sp
			{
				//cout<<"NOT sp "<<sp<<" due to cusD("<<node[cusD].id<<": depart = "<<cArrTime1<<"; c = "<<c[cusD][sp]<<", arrivesp = "<<cArrTime<<"; eTWsp = "<<nodeSP[sp].eTW2<<endl;
				belong[cusP][sp] = 0;
			}
			else //can assign C2C to sp:
			{
				cArrTime = cArrTime2 + c[cusP][sp];
				temp = cArrTime - nodeSP[sp].eTW2;
				if (temp > eps) //can't assign c2c to sp
				{
					//cout<<"NOT sp "<<sp<<" due to cusP("<<node[cusP].id<<"): depart = "<<cArrTime2<<";c = "<<c[cusP][sp]<<"; arrivesp = "<<cArrTime<<"; eTWsp = "<<nodeSP[sp].eTW2<<endl;
					belong[cusP][sp] = 0;
				}
				else
				{
					cArrTime = PDS_MAX(cArrTime2 + c[cusP][cusD], node[cusD].sTW) + node[cusD].duration + c[cusD][sp];
					temp = cArrTime - nodeSP[sp].eTW2;
					if (temp > eps) belong[cusP][sp] = 0;
					else 
					{
						belong[cusP][sp] = 1;
					    //cout<<sp<<", ";
					    countsp++;
					    node[cusP].sp[countsp] = sp;
					}
				}
			}
	   }
	   //cout<<endl;
	   node[cusP].numSP = countsp;
	   bool ASS = 0;
	   for (sp = nSP; sp >= 1; sp--)
	   {
		   temp = node[cusP].eTW - (leaveSP[sp][1] + c[sp][cusP]);
		   if (temp > eps) {node[cusP].zone = sp; ASS = 1; break;}
	   }
	   if (ASS == 0)
	   {
		   //cout<<"ERROR: For C2C "<<node[j].id<<": there doesnt exist any SP such that vehicle can go SP - C2C("<<j<<")"<<endl; 
		   cout<<"ERROR: For C2CP "<<node[cusP].id<<": [sTW, eTW] = ["<<node[cusP].sTW<<", "<<node[cusP].eTW<<"] C2CD "<<node[cusD].id<<": [sTW, eTW] = ["<<node[cusD].sTW<<", "<<node[cusD].eTW<<"]"<<endl;
		   count++;
	   }

	   // Next C2C-pickup/delivery-demand
	   cusP++;cusD++;
   }
#if Q_DEBUG
 	cout<<"List of C2C cus can assign to SP "<<endl;
	for (sp = 1; sp<= nSP; sp++)
	{
		cout<<"SP "<<sp<<": ";
		for (cusP = startidC2C;cusP <= endidC2CP; cusP ++)
	        if (belong[cusP][sp]) cout<<node[cusP].id<<", ";
		cout<<endl;
	}
#endif

}



void Problem::initData_Q()
{
	int i,j;

	int totalNODE = nSP + nP + nD + nC2C + 1;
	int totalNODE1 = totalNODE + 2;
	CWSSP = new (nothrow) int *[totalNODE1];
	if (CWSSP == 0)
	{
		 cout << "Error CWSSP: memory could not be allocated";
		 exit(-1);
	}
	for (i = 0; i < totalNODE1; i++) CWSSP[i] = new int[totalNODE1];
	
	disCWSSP = new (nothrow) double *[totalNODE1];
	if (disCWSSP == 0)
	{
		 cout << "Error disCWSSP: memory could not be allocated";
		 exit(-1);
	}
	for (i = 0; i < totalNODE1; i++) 
	{
		disCWSSP[i] = new double[totalNODE1];
		for (j=0;j<totalNODE1;j++) disCWSSP[i][j] = 0;
	}

	closestPSP = new (nothrow) int *[nSP1];
	if (closestPSP == 0)
	{
		 cout << "Error closestPSP: memory could not be allocated";
		 exit(-1);
	}
	int tep = nC2C + nP + 1;
	for (i =0; i <= nSP; i++)
	{
		closestPSP[i] = new int[tep];
		for (j=0;j<tep;j++) closestPSP[i][j] = -1;
	}

	tep = endidC2CP + 1;
    /*sameLeg = new (nothrow) bool *[tep];
	if (sameLeg == 0)
	{
		 cout << "Error sameLeg: memory could not be allocated";
		 exit(-1);
	}
	for (i=startidC2C; i<=endidC2CP;i++)
	{
		sameLeg[i] = new bool[tep];
		for (j=startidC2C; j <= endidC2CP; j++) sameLeg[i][j] = 0;
	}*/


   /*soonestC = new (nothrow) double *[totalNODE1];
   if (soonestC == 0)
   {
		 cout << "Error soonestC: memory could not be allocated";
		 exit(-1);
   }
   for (i=1;i<totalNODE;i++) soonestC[i] = new double[totalNODE1];
   
   latestC = new (nothrow) double *[totalNODE1];
   if (latestC == 0)
   {
		 cout << "Error latestC: memory could not be allocated";
		 exit(-1);
   }
   for (i=1;i<totalNODE;i++) latestC[i] = new double[totalNODE1];
  
  
    CSPDirect = new bool *[totalNODE1];
	for (i = 1; i <= totalNODE; i++) 
	{
		CSPDirect[i] = new bool[nSP1];
	    for (j=1;j<=nSP;j++) CSPDirect[i][j]=0;
	}*/

	int count = 0;
	CalculateAssignC2CToSP(count);
	if (count > 0)
	{
		cout<<"There are "<< count <<"C2C customers that doesn't exist path [SP-C2C]"<<endl;
	}

	CalculateStartDepart_Depot(); //calculate startS for pickup customer/C2C belong to a first trip with only one customer
	CalculatePWSSP();
	CalculateDWSSP();
	CalculateC2CDWSSP();
	CalculateSPWSSP();
	//CalculateWSAssignment_Q(); //TODO:
	calstartSOneP_OR_OneC2C();

	CalculateClosestPSP();
	CalculateClosestSPOfP();
	//CalculateSoonestLatestPD();

		/*for (i=startidP;i<= endidP;i++)
		   for (j=1;j<=node[i].numSP;j++)
		   {
			   int assignSP = node[i].sp[j];
			   double ArrTime = node[i].sTW + node[i].duration + c[i][assignSP];
			   double temp = ArrTime - nodeSP[assignSP].eTW1;
			   if (temp > eps)
			   {
				   cout<<"Pick cus "<<i<<" cannot go to sp "<<assignSP<<" directly"<<endl;
			   }
			   else if (!PDSINRANGE(ArrTime, nodeSP[assignSP].sTW1, nodeSP[assignSP].eTW1))
			   {
				   ArrTime =  node[i].sTW + node[i].duration + disCWSSP[i][assignSP];
				   temp = ArrTime - nodeSP[assignSP].eTW1;
				   if (temp > eps)
					   cout<<"Pickup cus "<<i<<" cannot go to sp "<<assignSP<<" directly because too soon, but cann't go through ws either; numSP = "<<node[i].numSP<<endl;
			   }

		   }*/
	

	findNeighborNodes();
    takenNeighborNodes();
	//CalSameLegC2C();
}
void Problem::CalSameLegC2C()
{
	/*int i,j,delIDi, delIDj;
	double departij, departji, cArrTime,temp;*/
	/*2 orders i and j are incompatible (can't be assigned to the same vehicle) if none of the following six partial routes are feasible:
	(i+,j+,j-,i-), (i+,j+,i-,j-), (j+,i+,i-,j-), (j+,i+,j-,i-),(i+,i-,j+,j-),(j+,j-,i+,i-)*/
	/*for (i = startidC2C; i < endidC2CP; i++)
	{
		delIDi = node[i].delID;
		for (j = i+1; j <= endidC2CP;j++)
		{
			delIDj = node[j].delID;
			cArrTime = node[i].sTW + node[i].duration + c[i][j];
			temp = cArrTime - node[j].eTW;
			if (temp > eps) continue;
			departij = PDS_MAX(cArrTime,node[j].sTW) + node[j].duration;
			//1. (i+,j+,j-,i-):
			cArrTime = departij + c[j][delIDj];
			temp = cArrTime - node[delIDj].eTW;
			if (temp > eps) continue;
			cArrTime = PDS_MAX(cArrTime, node[delIDj].sTW) + node[delIDj].duration + c[delIDj][delIDi];
			temp = cArrTime - node[delIDi].eTW;
			if (temp > eps) continue;
			//2. (i+,j+,i-,j-):
			cArrTime = departij + c[j][delIDi];
			temp = cArrTime - node[delIDi].eTW;
			if (temp > eps) continue;
			cArrTime = PDS_MAX(cArrTime, node[delIDi].sTW) + node[delIDi].duration + c[delIDi][delIDj];
			temp = cArrTime - node[delIDj].eTW;
			if (temp > eps) continue;

			cArrTime = node[j].sTW + node[j].duration + c[j][i];
			temp = cArrTime - node[i].eTW;
			if (temp > eps) continue;
			departji = PDS_MAX(cArrTime,node[i].sTW) + node[i].duration;
			//3. (j+,i+,i-,j-):
			cArrTime = departji + c[i][delIDi];
			temp = cArrTime - node[delIDi].eTW;
			if (temp > eps) continue;
			cArrTime = PDS_MAX(cArrTime, node[delIDi].sTW) + node[delIDi].duration + c[delIDi][delIDj];
			temp = cArrTime - node[delIDj].eTW;
			if (temp > eps) continue;
			//4. (j+,i+,j-,i-):
			cArrTime = departji + c[j][delIDj];
			temp = cArrTime - node[delIDj].eTW;
			if (temp > eps) continue;
			cArrTime = PDS_MAX(cArrTime, node[delIDj].sTW) + node[delIDj].duration + c[delIDj][delIDi];
			temp = cArrTime - node[delIDi].eTW;
			if (temp > eps) continue;

			//5.(i+,i-,j+,j-)
			cArrTime = PDS_MAX(node[i].sTW + node[i].duration + c[i][delIDi], node[delIDi].sTW) + node[delIDi].duration;
			cArrTime += c[delIDi][j];
			temp = cArrTime - node[j].eTW;
			if (temp > eps) continue;
			cArrTime = PDS_MAX(cArrTime, node[j].sTW) + node[j].duration + c[j][delIDj];
			temp = cArrTime - node[delIDj].eTW;
			if (temp > eps) continue;

			//6.(j+,j-,i+,i-)
			cArrTime = PDS_MAX(node[j].sTW + node[j].duration + c[j][delIDj], node[delIDj].sTW) + node[delIDj].duration;
			cArrTime += c[delIDj][i];
			temp = cArrTime - node[i].eTW;
			if (temp > eps) continue;
			cArrTime = PDS_MAX(cArrTime, node[i].sTW) + node[i].duration + c[i][delIDi];
			temp = cArrTime - node[delIDi].eTW;
			if (temp > eps) continue;
			sameLeg[i][j] = 1; sameLeg[j][i] = 1;
		}
	}*/
}



void Problem::createInstance(char *filename1, char *filename2) //Add 1000 to time windows of all customers and supply points
{
     ifstream infile;
	 infile.open(filename1,ios::in); 
     if (!infile)
	 {	 cout << "Input file not found !!!!!!" << endl; exit(-1);}
	 else
	 {
        cout<<"DEPOT"<<endl;

		ofstream myfile;
	    myfile.open (filename2,ios::trunc);

		 int n1, maxVeh, Q1, nSP1, nWS1;
		 infile >> n1 >> maxVeh >> Q1 >>nSP1 >> nWS1; //number of customers, max vechiles, capacity of vehicles, #supply points, #waiting station
         cout<<"D DEPOT"<<endl;

		 int totalNodes = n1 + nSP1 + nWS1;  
		 int i; double temp;
		 int id1[4000], x1[4000], y1[4000], cap1[4000], sTW1[4000], eTW1[4000], dur1[4000], sp1[4000];
         infile>> id1[0] >> x1[0]>>y1[0]>>cap1[0]>>sTW1[0]>>eTW1[0]>>dur1[0]>>sp1[0];
         cout<<"DOC XONG DEPOT"<<endl;
		 for (i=1; i<=n1; i++)
		 {
			 infile>> id1[i];
			 infile>> x1[i]>>y1[i];
			 infile>>cap1[i];
			 infile>>sTW1[i]>>eTW1[i];
			 infile>>dur1[i];
			 infile>>sp1[i];
		 }
		 cout<<"DOC XONG CUSTOMERS"<<endl;
		 
		 //Read data of supply points:
		 int stSP=n1+1; int endSP=n1+nSP1;
	     for (i=stSP; i<=endSP; i++)
		 {
			 infile>>id1[i];
             infile>> x1[i]>>y1[i];
			 infile>> temp;
			 infile>>dur1[i];
			 infile>>sTW1[i]>>eTW1[i];
		 }
		 cout<<"DOC XONG SP"<<endl;
         //Read data of waiting stations:
		 int stWS=endSP+1; int endWS = endSP + nWS1;
         for (i=stWS; i<=endWS; i++)
		 {
			 infile>>id1[i];
             infile>> x1[i]>>y1[i];
		 }

		 cout<<"DOC XONG FILE INPUT"<<endl;
		 myfile << n1<<"\t"<< maxVeh<<"\t"<<Q1<<"\t"<<nSP1<<"\t"<<nWS1<<endl; //number of customers, max vechiles, capacity of vehicles, #supply points, #waiting station
         myfile << id1[0]<<"\t"<< x1[0]<<"\t"<<y1[0]<<"\t"<<cap1[0]<<"\t"<<sTW1[0]<<"\t"<<eTW1[0]<<"\t"<<dur1[0]<<"\t"<<sp1[0]<<endl;


		 for (i=1; i<=n1; i++)
		 {
			 myfile << id1[i]<<"\t";
			 myfile << x1[i]<<"\t"<<y1[i]<<"\t";
			 myfile <<cap1[i]<<"\t";
			 myfile <<sTW1[i]+1000<<"\t"<<eTW1[i]+1000<<"\t";
			 myfile<<dur1[i]<<"\t";
			 myfile<<sp1[i]<<endl;
		 }

		myfile<<id1[stSP]<<"\t";
        myfile<< x1[stSP]<<"\t"<<y1[stSP]<<"\t";
		myfile<< eTW1[stSP]+1000<<"\t";
		myfile<<dur1[stSP]<<"\t";
		myfile<<(sTW1[stSP]+900)<<"\t"<<(eTW1[stSP]+1000)<<endl;

		 for (i=stSP+1; i<=endSP; i++)
		 {
			 myfile<<id1[i]<<"\t";
             myfile<< x1[i]<<"\t"<<y1[i]<<"\t";
			 myfile<< eTW1[i]+1000<<"\t";
			 myfile<<dur1[i]<<"\t";
			 myfile<<(sTW1[i]+1000)<<"\t"<<(eTW1[i]+1000)<<endl;
		 }

		 cout<<"Print XONG SP"<<endl;
         //Read data of waiting stations:
         for (i=stWS; i<=endWS; i++)
		 {
			 myfile<<id1[i]<<"\t";
             myfile<<x1[i]<<"\t"<<y1[i]<<endl;
		 }
	 }
}


void Problem::createInstance1(char *filename1, char *filename2, int num_assSP)
{
     ifstream infile;
	 infile.open(filename1,ios::in); 
     if (!infile)
	 {	 cout << "Input file not found !!!!!!" << endl; exit(-1);}
	 else
	 {
        cout<<"DEPOT"<<endl;

		ofstream myfile;
	    myfile.open (filename2,ios::trunc);

		 int n1, maxVeh, Q1, nSP1, nWS1;
		 infile >> n1 >> maxVeh >> Q1 >>nSP1 >> nWS1; //number of customers, max vechiles, capacity of vehicles, #supply points, #waiting station
         cout<<"D DEPOT"<<endl;

		 int totalNodes = n1 + nSP1 + nWS1;  
		 int i,j; double temp;

		 int *id1, *x1, *y1, *cap1, *sTW1, *eTW1, *dur1, *sp1;
		 id1 = new int[4000]; x1 = new int[4000]; y1 = new int[4000]; cap1 = new int[4000]; sTW1 = new int[4000]; eTW1 = new int[4000]; dur1 = new int[4000]; sp1 = new int[4000];
		
		 int *xP, *yP, *capP, *sTWP, *eTWP, *durP, *numspP, *startSPP;
		 xP = new int[4000]; yP = new int[4000]; capP = new int[4000]; sTWP = new int[4000]; eTWP = new int[4000]; durP = new int[4000]; 
		 numspP = new int[4000]; startSPP = new int[4000];
	
		 int *xD, *yD, *capD, *sTWD, *eTWD, *durD, *spD;
		 xD = new int[4000]; yD = new int[4000]; capD = new int[4000]; sTWD = new int[4000]; eTWD = new int[4000]; durD = new int[4000]; 
		 spD = new int[4000];
		 
		 infile>> id1[0] >> x1[0]>>y1[0]>>cap1[0]>>sTW1[0]>>eTW1[0]>>dur1[0]>>sp1[0];
         cout<<"DOC XONG DEPOT"<<endl;

		 int cursp = 1;
         int numCus = 0;
		 int numCusSP[100], endCusSP[100], startCusSP[100];
		 for (i=1; i<=n1; i++)
		 {
			 infile>> id1[i] >> x1[i]>>y1[i]>>cap1[i]>>sTW1[i]>>eTW1[i]>>dur1[i]>>sp1[i];
			 if (cursp == sp1[i]) {numCus++;}
			 else {endCusSP[cursp]= i-1;
			       //cout<<"sp "<<cursp<<": startCus = "<<startCusSP[cursp]<<"; endCus = "<<endCusSP[cursp]<<endl;
				   numCusSP[cursp]=numCus; 
				   cursp=sp1[i]; numCus=1; startCusSP[cursp]=i;
			       }
		 }
		 numCusSP[cursp] = numCus; endCusSP[cursp]=nD;

    	 cout<<"DOC XONG CUSTOMERS"<<endl;
		 
		 //Read data of supply points:
		 int stSP=n1+1; int endSP=n1+nSP1;
	     for (i=stSP; i<=endSP; i++)
		 {
			 infile>>id1[i];
             infile>> x1[i]>>y1[i];
			 infile>> temp;
			 infile>>dur1[i];
			 infile>>sTW1[i]>>eTW1[i];
		 }
		 cout<<"DOC XONG SP"<<endl;
         //Read data of waiting stations:
		 int stWS=endSP+1; int endWS = endSP + nWS1;
         for (i=stWS; i<=endWS; i++)
		 {
			 infile>>id1[i];
             infile>> x1[i]>>y1[i];
		 }

		 cout<<"DOC XONG FILE INPUT"<<endl;
		 
         
		 int numCusD=0, numCusP=0;
		 for (i=1;i<=n1; i++)
		 {
			 if (i%2 == 1) //selected as delivery cus
			 {
				 numCusD++;
				 xD[numCusD] = x1[i]; yD[numCusD] = y1[i];
				 capD[numCusD] = cap1[i]; sTWD[numCusD] = sTW1[i]; eTWD[numCusD] = eTW1[i];
				 durD[numCusD] = dur1[i]; spD[numCusD] = sp1[i];
			 }
			 else //selected as pickup customers:
			 {
				 numCusP++;
				 xP[numCusP] = x1[i]; yP[numCusP] = y1[i];
				 capP[numCusP] = cap1[i]; 
				 durP[numCusP] = dur1[i]; 
				 numspP[numCusP] = Rand(1,num_assSP);
				 j = sp1[i] + numspP[numCusP]-1;
				 if (j <=nSP1) startSPP[numCusP] = sp1[i];
				 else
				 {
					 int need = j - nSP1; startSPP[numCusP] = sp1[i] - need;
					 //cout<<"need = "<<need<<endl;
				 }
				 //cout<<"nSP = "<<nSP1<<"; j = "<<j<<"; numspP = "<<numspP[numCusP]<<"; startSP = "<<startSPP[numCusP]<<endl;
				 
				 if (numspP[numCusP]==1)
				 {
					 int idSP = sp1[i] + n1;
					 int Lp = eTW1[idSP] - dur1[i] - (int) ceil(sqrt(pow((double)(x1[i] - x1[idSP]),2) + pow((double)(y1[i] - y1[idSP]),2)));

					 eTWP[numCusP] = Rand(Lp-300,Lp); 
					 sTWP[numCusP] = Rand(eTWP[numCusP]-450, eTWP[numCusP]-150);
				 }
				 else
				 {
					 int end = startSPP[numCusP] + numspP[numCusP]-1;
					 int Ep = PDS_INFINITY_INT; int Lp = -PDS_INFINITY_INT;
					 int Ep1, Lp1, idSP;
					 for (int buon = startSPP[numCusP]; buon<=end;buon++)
					 {
						 idSP = buon+n1;
					     Ep1 = eTW1[idSP] - dur1[i] - (int) ceil(sqrt(pow((double)(x1[i] - x1[idSP]),2) + pow((double)(y1[i] - y1[idSP]),2)));
					     Lp1 = eTW1[idSP] - dur1[i] - (int) ceil(sqrt(pow((double)(x1[i] - x1[idSP]),2) + pow((double)(y1[i] - y1[idSP]),2)));
					     if (Ep1 < Ep) Ep = Ep1;
						 if (Lp1 > Lp) Lp = Lp1;
					 }
					 //int idSP = end + n1;
					 //int Lp = eTW1[idSP] - dur1[i] - (int) ceil(sqrt(pow((double)(x1[i] - x1[idSP]),2) + pow((double)(y1[i] - y1[idSP]),2)));
					 eTWP[numCusP] = Rand(Lp-300,Lp);
					 //idSP = startSPP[numCusP]+n1;
					 //int Ep = eTW1[idSP] - dur1[i] - (int) ceil(sqrt(pow((double)(x1[i] - x1[idSP]),2) + pow((double)(y1[i] - y1[idSP]),2)));
					 sTWP[numCusP] = Rand(Ep - 300, Ep);
					 if (eTWP[numCusP] < sTWP[numCusP]) 
					 {
						 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cus "<<numCusP<<endl;
						 cout<<"sTW = "<<sTWP[numCusP]<<"; eTW = "<<eTWP[numCusP]<<endl;
						 cout<<"startSP = "<<startSPP[numCusP]<<", endSP = "<<end<<endl; 
						 cout<<"Ep = "<<Ep<<";Lp = "<<Lp<<endl;
						 
						 sTWP[numCusP] = eTWP[numCusP] - Rand(150,450);
						 cout<<"NEW sTW = "<<sTWP[numCusP]<<"; eTW = "<<eTWP[numCusP]<<endl;
						 //getch();
						 //exit(-1);
					 }
					 if (eTWP[numCusP] < sTWP[numCusP] +100)
					 {
						 cout<<"----------------------- idCus = "<<numCusP<<endl;
						 cout<<"sTW = "<<sTWP[numCusP]<<"; eTW = "<<eTWP[numCusP]<<endl;
						 sTWP[numCusP] = eTWP[numCusP] - Rand(150,200);
						 //getch();
					 }
				 }
			 }
		 }

		 myfile <<"1\t"<<numCusP<<"\t"<<numCusD<<"\t"<<Q1<<"\t"<<nSP1<<"\t"<<nWS1<<endl; //number of customers, max vechiles, capacity of vehicles, #supply points, #waiting station
         myfile << id1[0]<<"\t"<< x1[0]<<"\t"<<y1[0]<<endl;

		 for (i=1; i<=numCusP; i++)
		 {
			 myfile << i<<"\t";
			 myfile << xP[i]<<"\t"<<yP[i]<<"\t";
			 myfile <<capP[i]<<"\t";
			 myfile <<sTWP[i]<<"\t"<<eTWP[i]<<"\t";
			 myfile<<durP[i]<<"\t";
			 myfile<<numspP[i]<<"\t";
			 for (j=startSPP[i];j<startSPP[i] + numspP[i];j++)
				 myfile<<j<<"   ";
			 myfile<<endl;
		 }

		 for (i=1; i<=numCusD; i++)
		 {
			 myfile <<i<<"\t";
			 myfile << xD[i]<<"\t"<<yD[i]<<"\t";
			 myfile <<capD[i]<<"\t";
			 myfile <<sTWD[i]<<"\t"<<eTWD[i]<<"\t";
			 myfile<<durD[i]<<"\t";
			 myfile<<spD[i]<<endl;
		 }


		 j=1;
		 for (i=stSP; i<=endSP; i++)
		 {
			 myfile<<j<<"\t";
             myfile<< x1[i]<<"\t"<<y1[i]<<"\t 30 \t 30\t";
			 myfile<<(sTW1[i])<<"\t"<<(eTW1[i])<<endl;
			 j++;
		 }

		 cout<<"Print XONG SP"<<endl;
         //Read data of waiting stations:
		 j =1;
         for (i=stWS; i<=endWS; i++)
		 {
			 myfile<<j<<"\t";
             myfile<<x1[i]<<"\t"<<y1[i]<<endl;
			 j++;
		 }
	 }
}




void Problem::createInstanceC2C_SP4(char *filename1,char *filename2)
{
     ifstream infile;
	 infile.open(filename1,ios::in); 
     if (!infile)
	 {	 cout << "Input file not found !!!!!!" << endl; exit(-1);}
	 else
	 {
  		 ofstream myfile;
	     myfile.open (filename2,ios::trunc);

		 int type,soonest,latest;
		 infile >> type >> nP >> nD >> Q >> nSP >> nWS;

		 int totalNodes = nP + nD + nSP + nWS;  
		 int i,j; double temp;

		 int *id1, *x1, *y1, *cap1, *sTW1, *eTW1, *dur1, *sp1;
		 id1 = new int[4000]; x1 = new int[4000]; y1 = new int[4000]; cap1 = new int[4000]; sTW1 = new int[4000]; eTW1 = new int[4000]; dur1 = new int[4000]; sp1 = new int[4000];
		

		 int **listSP;
		 listSP = new int*[1000]; 
		 for (i=0;i<1000;i++) listSP[i] = new int[50];
	
		 int *xP, *yP, *capP, *sTWP, *eTWP, *durP;
		 xP = new int[1000]; yP = new int[1000]; capP = new int[1000]; sTWP = new int[1000]; eTWP = new int[1000]; durP = new int[1000]; 

		 int *xD, *yD, *capD, *sTWD, *eTWD, *durD;
		 xD = new int[1000]; yD = new int[1000]; capD = new int[1000]; sTWD = new int[1000]; eTWD = new int[1000]; durD = new int[1000]; 
		
		 
		 infile>> id1[0] >> x1[0] >> y1[0];
         cout<<"DOC XONG DEPOT"<<endl;

		 for (i=1;i<=nP;i++)
		 {
			 infile>> id1[i] >> x1[i]>>y1[i]>>cap1[i]>>sTW1[i]>>eTW1[i]>>dur1[i]>>sp1[i];
			 for (j=1;j<=sp1[i]; j++) infile >>listSP[i][j];
		 }
		 cout<<"DOC XONG PICKUP "<<endl;

		 int count1 = nP + 1; int count2 = nP + nD;
		 int startD = count1;
		 int TOTALCAPD = 0; soonest = PDS_INFINITY_INT; latest = 0;
		 for (i=count1;i<=count2; i++)
		 {
			 infile>> id1[i] >> x1[i]>>y1[i]>>cap1[i]>>sTW1[i]>>eTW1[i]>>dur1[i]>>sp1[i];
			 if (sp1[i] == nSP) 
			 {
				 TOTALCAPD += cap1[i];
				 if (soonest > sTW1[i]) soonest = sTW1[i];
				 if (latest < eTW1[i]) latest = eTW1[i];
			 }
		 }

		 cout<<"DOC XONG delivery: from "<<startD<<" to "<<count2<<endl;
		 
		 //Read data of supply points:
		 count1 = count2 + 1; count2 = count2 + nSP;
		 int startSP = count1; int endSP = count2;
	     for (i=count1; i <= count2; i++)
		 {
			 infile>>id1[i];
             infile>> x1[i]>>y1[i];
			 infile>> temp;
			 infile>>dur1[i];
			 infile>>sTW1[i]>>eTW1[i];
			 //cout<<i<<": "<<id1[i]<<"  "<<x1[i]<<"   "<<y1[i]<<"   "<<temp<<"   "<<dur1[i]<<"   "<<sTW1[i]<<"   "<<eTW1[i]<<endl;
		 }
		 cout<<"DOC XONG SP from "<<startSP<<" to "<<endSP<<endl;

		 //Read data of waiting stations:
		 count1=count2+1; count2 = count2 + nWS;
		 int startWS = count1;
         for (i=count1; i<=count2; i++)
		 {
			 infile>>id1[i];
             infile>> x1[i]>>y1[i];
		 }

		 cout<<"DOC XONG FILE INPUT"<<endl;
		 cout<<"eTW1[sp4 = "<<endSP<<"] = "<<eTW1[endSP]<<endl;

		 cout<<"Create C2C: "<<endl;
		 int nOrder = (nP + nD)/3; int  Ep,Ed,Lp,Ld,Lp1;

		 int capT[400];
		 int nCus = TOTALCAPD/15;
		 DistributeCAP(TOTALCAPD,nCus,capT,5,25);
		 j = 0;
		 cout<<"soonest = "<<soonest<<"; latest = "<<latest<<"; nCus_for_lastSP = "<<nCus<<endl;
		 int capDis = 0;
		 for (i=1;i<=nCus; i++)
		 {
			 j++; 
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = capT[j]; capD[i] = -capP[i];capDis += capP[i];
			 int dis = (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP]),2) + pow((double)(yD[i] - y1[endSP]),2)));
			 Ep = max(soonest,eTW1[endSP]+30) + (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP]),2) + pow((double)(yD[i] - y1[endSP]),2)));
			 Lp = latest - 50;
			 if (Ep > Lp) {int tt = Ep; Ep = Lp; Lp = tt;}
			 //cout<<"dis = "<<dis<<"; Ep = "<<Ep<<"; Lp = "<<Lp<<endl;
			 sTWP[i] = Rand(Ep,Lp);
			 eTWP[i] = sTWP[i] + Rand(150,350);

			 Ed = eTWP[i] + durP[i] + (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 sTWD[i] = Rand(Ed, Ed + 300);
			 eTWD[i] = sTWD[i] + Rand(350,1000);
		 }
		 cout<<"TotalCAPD = "<<TOTALCAPD<<"; capC2C = "<<capDis<<endl;
		 count1 = nCus + 1;
		 int share = (nOrder - nCus)/4;
		 count2 = count1 + share;
		 cout<<"Each vung nCus = "<<share<<endl;

		 for (i=count1;i<=count2; i++)
		 {
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP]),2) + pow((double)(yD[i] - y1[startSP]),2)));
			 int dis2 = (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP]),2) + pow((double)(yD[i] - y1[endSP]),2)));
		     Ed = eTW1[startSP] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP]),2) + pow((double)(yD[i] - y1[startSP]),2)));
		     Ld = eTW1[endSP] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP]),2) + pow((double)(yD[i] - y1[endSP]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);
			 if (sTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": sTWD = "<<sTWD[i]<<endl;
				 cout<<"eTW1[sp1] = "<<eTW1[startSP]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis1<<endl;
				 return;
			 }

			 if (eTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": eTWD = "<<eTWD[i]<<endl;
				 cout<<"eTW1[sp4 = "<<endSP<<"] = "<<eTW1[endSP]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis2<<endl;
				 return;
			 }
			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp1-4: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp1-4:----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }
			 Lp = eTWD[i] - durP[i] - (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp1 = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp1-300,Lp);
			 sTWP[i] = min(eTWP[i] - Rand(150,450), Lp1-Rand(50,100));
			 /*Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);*/
			 if (sTWP[i] < 0) sTWP[i] = 0;
			 if (eTWP[i] < 0)
			 {
				 cout<<"@Sp1-4: ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }

		 int startSP1 = startSP; int endSP1 = startSP1 + 1;
		 count1 = count2+1; count2 = count2+share;
		 for (i=count1;i <= count2; i++)
		 {
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
			 int dis2 = (int)ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
		     Ed = eTW1[startSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
		     Ld = eTW1[endSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);
			 if (sTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": sTWD = "<<sTWD[i]<<endl;
				 cout<<"eTW1[startSP = "<<startSP1<<"] = "<<eTW1[startSP1]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis1<<endl;
				 return;
			 }

			 if (eTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": eTWD = "<<eTWD[i]<<endl;
				 cout<<"eTW1[endSP = "<<endSP1<<"] = "<<eTW1[endSP1]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis2<<endl;
				 return;
			 }

			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp1-2: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp1-2: ----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }
			 Lp = eTWD[i] - durP[i] - (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp1 = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp1-300,Lp);
			 sTWP[i] = min(eTWP[i] - Rand(150,450), Lp1-Rand(50,100));
			 /*Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);*/
			 if (sTWP[i] < 0) 
			 {
				 cout<<"@Sp1-2: sTWP["<<i<<"] = "<<sTWP[i]<<": @ sp 1-2"<<endl;
				 sTWP[i] = 0;
			 }
			 if (eTWP[i] < 0)
			 {
				 cout<<"@Sp1-2: ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }
         
		 startSP1 = startSP + 1; endSP1 = startSP1 + 1;
		 count1 = count2+1; count2 = count2+share;
		 for (i=count1;i<=count2; i++)
		 {
			 //delID = i + nOrder;
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
			 int dis2 = (int)ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
		     Ed = eTW1[startSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
		     Ld = eTW1[endSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);
			 if (sTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": sTWD = "<<sTWD[i]<<endl;
				 cout<<"eTW1[startSP = "<<startSP1<<"] = "<<eTW1[startSP1]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis1<<endl;
				 return;
			 }

			 if (eTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": eTWD = "<<eTWD[i]<<endl;
				 cout<<"eTW1[endSP = "<<endSP1<<"] = "<<eTW1[endSP1]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis2<<endl;
				 return;
			 }
			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp2-3: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp2-3: ----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }
			 Lp = eTWD[i] - durP[i] - (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp1 = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp1-300,Lp);
			 sTWP[i] = min(eTWP[i] - Rand(150,450), Lp1-Rand(50,100));

			 /*Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);*/
			 if (sTWP[i] < 0)
			 {
				 cout<<"sTWP["<<i<<"] = "<<sTWP[i]<<": @ sp 2-3"<<endl;
				 sTWP[i] = 0;
			 }
			 if (eTWP[i] < 0)
			 {
				 cout<<"@Sp2-3: ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }

		 startSP1 = startSP + 2; endSP1 = startSP1+1;
		 count1 = count2 + 1; count2 = count2 + share;
		 for (i=count1;i<=count2; i++)
		 {
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
			 int dis2 = (int)ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
		     Ed = eTW1[startSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
		     Ld = eTW1[endSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);

			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp3-4: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp3-4:----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }
			 //Ep = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);
			 if (sTWP[i] < 0)
			 {
				 cout<<"sTWP["<<i<<"] = "<<sTWP[i]<<": @ sp 3-4"<<endl;
				 sTWP[i] = 0;
			 }
			 if (eTWP[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }
		 nOrder = count2;
		 cout<<"Number of order = "<<nOrder<<endl;

	 
		 myfile <<"1\t"<<nP<<"\t"<<nOrder<<"\t"<<nD<<"\t"<<Q<<"\t"<<nSP<<"\t"<<nWS<<endl; //number of Pcustomers, #Order, nDCustomer, capacity of vehicles, #supply points, #waiting station
         myfile << id1[0]<<"\t"<< x1[0]<<"\t"<<y1[0]<<endl;

		 for (i=1; i<=nP; i++)
		 {
			 myfile << i<<"\t";
			 myfile << x1[i]<<"\t"<<y1[i]<<"\t";
			 myfile <<cap1[i]<<"\t";
			 myfile <<sTW1[i]<<"\t"<<eTW1[i]<<"\t";
			 myfile<<dur1[i]<<"\t";
			 myfile<<sp1[i]<<"\t";
			 for (j=1;j <= sp1[i];j++)  myfile<<listSP[i][j]<<"   ";
			 myfile<<endl;
		 }

		 for (i=1;i<=nOrder;i++)
		 {
			 myfile << i<<"\t";
			 myfile << xP[i]<<"\t"<<yP[i]<<"\t";
			 myfile <<capP[i]<<"\t";
			 myfile <<sTWP[i]<<"\t"<<eTWP[i]<<"\t";
			 myfile<<durP[i]<<"\t";
			 myfile<<endl;
		 }

		 count1 = nOrder + 1; 
		 for (i=1;i<=nOrder;i++)
		 {
			 myfile << count1<<"\t";
			 myfile << xD[i]<<"\t"<<yD[i]<<"\t";
			 myfile <<capD[i]<<"\t";
			 myfile <<sTWD[i]<<"\t"<<eTWD[i]<<"\t";
			 myfile<<durD[i]<<"\t";
			 myfile<<endl;
			 count1++;
		 }

		 count1 = startD;
		 for (i=1; i<=nD; i++)
		 {
			 myfile <<i<<"\t";
			 myfile << x1[count1]<<"\t"<<y1[count1]<<"\t";
			 myfile <<cap1[count1]<<"\t";
			 myfile <<sTW1[count1]<<"\t"<<eTW1[count1]<<"\t";
			 myfile<<dur1[count1]<<"\t";
			 myfile<<sp1[count1]<<"\t";
			 count1++;
 			 myfile<<endl;
		 }

		 count1=startSP;
		 for (i=1; i<=nSP; i++)
		 {
			 myfile<<i<<"\t";
             myfile<< x1[count1]<<"\t"<<y1[count1]<<"\t 30 \t 30\t";
			 myfile<<(sTW1[count1])<<"\t"<<(eTW1[count1])<<endl;
			 count1++;
 			 myfile<<endl;
		 }

		 cout<<"Print XONG SP"<<endl;
         //Read data of waiting stations:
		 count1 = startWS;
         for (i=1; i<=nWS; i++)
		 {
			 myfile<<i<<"\t";
             myfile<<x1[count1]<<"\t"<<y1[count1]<<endl;
			 count1++;
 			 myfile<<endl;
		 }
	 }
}

void Problem::createInstanceC2C_SP8(char *filename1,char *filename2)
{
     ifstream infile;
	 infile.open(filename1,ios::in); 
     if (!infile)
	 {	 cout << "Input file not found !!!!!!" << endl; exit(-1);}
	 else
	 {
  		 ofstream myfile;
	     myfile.open (filename2,ios::trunc);

		 int type,soonest,latest;
		 infile >> type >> nP >> nD >> Q >> nSP >> nWS;

		 int totalNodes = nP + nD + nSP + nWS;  
		 int i,j; double temp;

		 int *id1, *x1, *y1, *cap1, *sTW1, *eTW1, *dur1, *sp1;
		 id1 = new int[4000]; x1 = new int[4000]; y1 = new int[4000]; cap1 = new int[4000]; sTW1 = new int[4000]; eTW1 = new int[4000]; dur1 = new int[4000]; sp1 = new int[4000];
		

		 int **listSP;
		 listSP = new int*[1000]; 
		 for (i=0;i<1000;i++) listSP[i] = new int[50];
	
		 int *xP, *yP, *capP, *sTWP, *eTWP, *durP;
		 xP = new int[1000]; yP = new int[1000]; capP = new int[1000]; sTWP = new int[1000]; eTWP = new int[1000]; durP = new int[1000]; 

		 int *xD, *yD, *capD, *sTWD, *eTWD, *durD;
		 xD = new int[1000]; yD = new int[1000]; capD = new int[1000]; sTWD = new int[1000]; eTWD = new int[1000]; durD = new int[1000]; 
		
		 
		 infile>> id1[0] >> x1[0] >> y1[0];
         cout<<"DOC XONG DEPOT"<<endl;

		 for (i=1;i<=nP;i++)
		 {
			 infile>> id1[i] >> x1[i]>>y1[i]>>cap1[i]>>sTW1[i]>>eTW1[i]>>dur1[i]>>sp1[i];
			 for (j=1;j<=sp1[i]; j++) infile >>listSP[i][j];
		 }
		 cout<<"DOC XONG PICKUP "<<endl;

		 int count1 = nP + 1; int count2 = nP + nD;
		 int startD = count1;
		 int TOTALCAPD = 0; soonest = PDS_INFINITY_INT; latest = 0;
		 for (i=count1;i<=count2; i++)
		 {
			 infile>> id1[i] >> x1[i]>>y1[i]>>cap1[i]>>sTW1[i]>>eTW1[i]>>dur1[i]>>sp1[i];
			 if (sp1[i] == nSP) 
			 {
				 TOTALCAPD += cap1[i];
				 if (soonest > sTW1[i]) soonest = sTW1[i];
				 if (latest < eTW1[i]) latest = eTW1[i];
			 }
		 }

		 cout<<"DOC XONG delivery: from "<<startD<<" to "<<count2<<endl;
		 
		 //Read data of supply points:
		 count1 = count2 + 1; count2 = count2 + nSP;
		 int startSP = count1; int endSP = count2;
	     for (i=count1; i <= count2; i++)
		 {
			 infile>>id1[i];
             infile>> x1[i]>>y1[i];
			 infile>> temp;
			 infile>>dur1[i];
			 infile>>sTW1[i]>>eTW1[i];
			 //cout<<i<<": "<<id1[i]<<"  "<<x1[i]<<"   "<<y1[i]<<"   "<<temp<<"   "<<dur1[i]<<"   "<<sTW1[i]<<"   "<<eTW1[i]<<endl;
		 }
		 cout<<"DOC XONG SP from "<<startSP<<" to "<<endSP<<endl;

		 //Read data of waiting stations:
		 count1=count2+1; count2 = count2 + nWS;
		 int startWS = count1;
         for (i=count1; i<=count2; i++)
		 {
			 infile>>id1[i];
             infile>> x1[i]>>y1[i];
		 }

		 cout<<"DOC XONG FILE INPUT"<<endl;
		 cout<<"eTW1[sp4 = "<<endSP<<"] = "<<eTW1[endSP]<<endl;

		 cout<<"Create C2C: "<<endl;
		 int nOrder = (nP + nD)/3; int  Ep,Ed,Lp,Ld,Lp1;

		 int capT[400];
		 int nCus = TOTALCAPD/15;
		 DistributeCAP(TOTALCAPD,nCus,capT,5,25);
		 j = 0;
		 cout<<"soonest = "<<soonest<<"; latest = "<<latest<<"; nCus_for_lastSP = "<<nCus<<endl;
		 int capDis = 0;
		 for (i=1;i<=nCus; i++)
		 {
			 j++; 
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = capT[j]; capD[i] = -capP[i]; capDis += capP[i];
			 int dis = (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP]),2) + pow((double)(yD[i] - y1[endSP]),2)));
			 Ep = max(soonest,eTW1[endSP]+30) + (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP]),2) + pow((double)(yD[i] - y1[endSP]),2)));
			 Lp = latest - 50;
			 if (Ep > Lp) {int tt = Ep; Ep = Lp; Lp = tt;}
			 //cout<<"dis = "<<dis<<"; Ep = "<<Ep<<"; Lp = "<<Lp<<endl;
			 sTWP[i] = Rand(Ep,Lp);
			 eTWP[i] = sTWP[i] + Rand(150,350);

			 Ed = eTWP[i] + durP[i] + (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 sTWD[i] = Rand(Ed, Ed + 300);
			 eTWD[i] = sTWD[i] + Rand(350,1000);
		 }
		 cout<<"TOTALCAPD = "<<TOTALCAPD<<"; capC2C = "<<capDis<<endl;
		 count1 = nCus + 1;
		 int share = (nOrder - nCus)/7;
		 count2 = count1 + share;
		 cout<<"Each vung nCus = "<<share<<endl;

		 for (i=count1;i<=count2; i++)
		 {
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP]),2) + pow((double)(yD[i] - y1[startSP]),2)));
			 int dis2 = (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP]),2) + pow((double)(yD[i] - y1[endSP]),2)));
		     Ed = eTW1[startSP] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP]),2) + pow((double)(yD[i] - y1[startSP]),2)));
		     Ld = eTW1[endSP] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP]),2) + pow((double)(yD[i] - y1[endSP]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);
			 if (sTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": sTWD = "<<sTWD[i]<<endl;
				 cout<<"eTW1[sp1] = "<<eTW1[startSP]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis1<<endl;
				 return;
			 }

			 if (eTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": eTWD = "<<eTWD[i]<<endl;
				 cout<<"eTW1[sp8 = "<<endSP<<"] = "<<eTW1[endSP]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis2<<endl;
				 return;
			 }
			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp1-8: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp1-8:----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }

			 Lp = eTWD[i] - durP[i] - (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp1 = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp1-300,Lp);
			 sTWP[i] = min(eTWP[i] - Rand(150,450), Lp1-Rand(50,100));

			 /*Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);*/
			 if (sTWP[i] < 0) sTWP[i] = 0;
			 if (eTWP[i] < 0)
			 {
				 cout<<"@Sp1-8: ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }

		 int startSP1 = startSP; int endSP1 = startSP1 + 2;
		 count1 = count2+1; count2 = count2+share;
		 for (i=count1;i <= count2; i++)
		 {
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
			 int dis2 = (int)ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
		     Ed = eTW1[startSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
		     Ld = eTW1[endSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);
			 if (sTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": sTWD = "<<sTWD[i]<<endl;
				 cout<<"eTW1[startSP = "<<startSP1<<"] = "<<eTW1[startSP1]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis1<<endl;
				 return;
			 }

			 if (eTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": eTWD = "<<eTWD[i]<<endl;
				 cout<<"eTW1[endSP = "<<endSP1<<"] = "<<eTW1[endSP1]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis2<<endl;
				 return;
			 }

			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp1-3: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp1-3: ----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }

			 Lp = eTWD[i] - durP[i] - (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp1 = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp1-300,Lp);
			 sTWP[i] = min(eTWP[i] - Rand(150,450), Lp1-Rand(50,100));

			 /*Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);*/
			 if (sTWP[i] < 0) 
			 {
				 cout<<"@Sp1-3: sTWP["<<i<<"] = "<<sTWP[i]<<": @ sp 1-2"<<endl;
				 sTWP[i] = 0;
			 }
			 if (eTWP[i] < 0)
			 {
				 cout<<"@Sp1-3: ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }
         
		 startSP1 = startSP + 1; endSP1 = startSP1 + 2;
		 count1 = count2+1; count2 = count2+share;
		 for (i=count1;i<=count2; i++)
		 {
			 //delID = i + nOrder;
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
			 int dis2 = (int)ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
		     Ed = eTW1[startSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
		     Ld = eTW1[endSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);
			 if (sTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": sTWD = "<<sTWD[i]<<endl;
				 cout<<"eTW1[startSP = "<<startSP1<<"] = "<<eTW1[startSP1]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis1<<endl;
				 return;
			 }

			 if (eTWD[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<": eTWD = "<<eTWD[i]<<endl;
				 cout<<"eTW1[endSP = "<<endSP1<<"] = "<<eTW1[endSP1]<<endl;
				 cout<<"durD = "<<durD[i]<<endl;
				 cout<<"dis = "<<dis2<<endl;
				 return;
			 }
			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp2-4: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp2-4: ----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }
			 Lp = eTWD[i] - durP[i] - (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp1 = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp1-300,Lp);
			 sTWP[i] = min(eTWP[i] - Rand(150,450), Lp1-Rand(50,100));

			 /*Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);*/
			 if (sTWP[i] < 0)
			 {
				 cout<<"sTWP["<<i<<"] = "<<sTWP[i]<<": @ sp 2-3"<<endl;
				 sTWP[i] = 0;
			 }
			 if (eTWP[i] < 0)
			 {
				 cout<<"@Sp2-4: ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }

		 startSP1 = startSP + 2; endSP1 = startSP1+2;
		 count1 = count2 + 1; count2 = count2 + share;
		 for (i=count1;i<=count2; i++)
		 {
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
			 int dis2 = (int)ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
		     Ed = eTW1[startSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
		     Ld = eTW1[endSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);

			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp3-5: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp3-5:----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }

			 Lp = eTWD[i] - durP[i] - (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp1 = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp1-300,Lp);
			 sTWP[i] = min(eTWP[i] - Rand(150,450), Lp1-Rand(50,100));

			 /*Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);*/
			 if (sTWP[i] < 0)
			 {
				 cout<<"sTWP["<<i<<"] = "<<sTWP[i]<<": @ sp 3-5"<<endl;
				 sTWP[i] = 0;
			 }
			 if (eTWP[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }

		 startSP1 = startSP + 2; endSP1 = startSP1+2;
		 count1 = count2 + 1; count2 = count2 + share;
		 for (i=count1;i<=count2; i++)
		 {
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
			 int dis2 = (int)ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
		     Ed = eTW1[startSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
		     Ld = eTW1[endSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);

			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp4-6: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp4-6:----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }

			 Lp = eTWD[i] - durP[i] - (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp1 = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp1-300,Lp);
			 sTWP[i] = min(eTWP[i] - Rand(150,450), Lp1-Rand(50,100));

			 /*Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);*/
			 if (sTWP[i] < 0)
			 {
				 cout<<"sTWP["<<i<<"] = "<<sTWP[i]<<": @ sp 4-6"<<endl;
				 sTWP[i] = 0;
			 }
			 if (eTWP[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }

		 startSP1 = startSP + 2; endSP1 = startSP1+2;
		 count1 = count2 + 1; count2 = count2 + share;
		 for (i=count1;i<=count2; i++)
		 {
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
			 int dis2 = (int)ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
		     Ed = eTW1[startSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
		     Ld = eTW1[endSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);

			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp5-7: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp5-7:----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }

			 Lp = eTWD[i] - durP[i] - (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp1 = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp1-300,Lp);
			 sTWP[i] = min(eTWP[i] - Rand(150,450), Lp1-Rand(50,100));

			 /*Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);*/
			 if (sTWP[i] < 0)
			 {
				 cout<<"sTWP["<<i<<"] = "<<sTWP[i]<<": @ sp 5-7"<<endl;
				 sTWP[i] = 0;
			 }
			 if (eTWP[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }

		 startSP1 = startSP + 2; endSP1 = startSP1+2;
		 count1 = count2 + 1; count2 = count2 + share;
		 for (i=count1;i<=count2; i++)
		 {
			 xP[i] = Rand(0,100); yP[i] = Rand(0,100); durP[i] = 20;
			 xD[i] = Rand(0,100); yD[i] = Rand(0,100); durD[i] = 20;
			 capP[i] = Rand(5,25); capD[i] = -capP[i];
			 int dis1 = (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
			 int dis2 = (int)ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
		     Ed = eTW1[startSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[startSP1]),2) + pow((double)(yD[i] - y1[startSP1]),2)));
		     Ld = eTW1[endSP1] - durD[i] - (int) ceil(sqrt(pow((double)(xD[i] - x1[endSP1]),2) + pow((double)(yD[i] - y1[endSP1]),2)));
			 eTWD[i] = Rand(Ld-300,Ld);
			 sTWD[i] = Rand(Ed - 300, Ed);

			 if (eTWD[i] < sTWD[i]) 
			 {
				 cout<<"@Sp6-8: ERRRORRRRRRRRRRRRRRRRRRRR at cusD "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 cout<<"Ed = "<<Ed<<";Ld = "<<Ld<<endl;
				 
				 sTWD[i] = eTWD[i] - Rand(150,450);
				 cout<<"NEW sTWD = "<<sTWD[i]<<"; eTWD = "<<eTWD[i]<<endl;
			 }
			 if (eTWD[i] < sTWD[i] +100)
			 {
				 cout<<"@Sp6-8:----------------------- idCusD = "<<i<<endl;
				 cout<<"sTW = "<<sTWD[i]<<"; eTW = "<<eTWD[i]<<endl;
				 sTWD[i] = eTWD[i] - Rand(150,200);
				 //getch();
			 }

			 Lp = eTWD[i] - durP[i] - (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 Lp1 = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp1-300,Lp);
			 sTWP[i] = min(eTWP[i] - Rand(150,450), Lp1-Rand(50,100));

			 /*Lp = sTWD[i] - durP[i] -  (int) ceil(sqrt(pow((double)(xP[i] - xD[i]),2) + pow((double)(yP[i] - yD[i]),2)));
			 eTWP[i] = Rand(Lp-300, Lp);
			 sTWP[i] = eTWP[i] - Rand(150, 450);*/
			 if (sTWP[i] < 0)
			 {
				 cout<<"sTWP["<<i<<"] = "<<sTWP[i]<<": @ sp 6-8"<<endl;
				 sTWP[i] = 0;
			 }
			 if (eTWP[i] < 0)
			 {
				 cout<<"ERRRORRRRRRRRRRRRRRRRRRRR at cusP "<<i<<": eTWP = "<<eTWP[i]<<endl;
				 return;
			 }
		 }
		 nOrder = count2;
		 cout<<"Number of order = "<<nOrder<<endl;

	 
		 myfile <<"1\t"<<nP<<"\t"<<nOrder<<"\t"<<nD<<"\t"<<Q<<"\t"<<nSP<<"\t"<<nWS<<endl; //number of Pcustomers, #Order, nDCustomer, capacity of vehicles, #supply points, #waiting station
         myfile << id1[0]<<"\t"<< x1[0]<<"\t"<<y1[0]<<endl;

		 for (i=1; i<=nP; i++)
		 {
			 myfile << i<<"\t";
			 myfile << x1[i]<<"\t"<<y1[i]<<"\t";
			 myfile <<cap1[i]<<"\t";
			 myfile <<sTW1[i]<<"\t"<<eTW1[i]<<"\t";
			 myfile<<dur1[i]<<"\t";
			 myfile<<sp1[i]<<"\t";
			 for (j=1;j <= sp1[i];j++)  myfile<<listSP[i][j]<<"   ";
			 myfile<<endl;
		 }

		 for (i=1;i<=nOrder;i++)
		 {
			 myfile << i<<"\t";
			 myfile << xP[i]<<"\t"<<yP[i]<<"\t";
			 myfile <<capP[i]<<"\t";
			 myfile <<sTWP[i]<<"\t"<<eTWP[i]<<"\t";
			 myfile<<durP[i]<<"\t";
			 myfile<<endl;
		 }

		 count1 = nOrder + 1; 
		 for (i=1;i<=nOrder;i++)
		 {
			 myfile << count1<<"\t";
			 myfile << xD[i]<<"\t"<<yD[i]<<"\t";
			 myfile <<capD[i]<<"\t";
			 myfile <<sTWD[i]<<"\t"<<eTWD[i]<<"\t";
			 myfile<<durD[i]<<"\t";
			 myfile<<endl;
			 count1++;
		 }

		 count1 = startD;
		 for (i=1; i<=nD; i++)
		 {
			 myfile <<i<<"\t";
			 myfile << x1[count1]<<"\t"<<y1[count1]<<"\t";
			 myfile <<cap1[count1]<<"\t";
			 myfile <<sTW1[count1]<<"\t"<<eTW1[count1]<<"\t";
			 myfile<<dur1[count1]<<"\t";
			 myfile<<sp1[count1]<<"\t";
			 count1++;
 			 myfile<<endl;
		 }

		 count1=startSP;
		 for (i=1; i<=nSP; i++)
		 {
			 myfile<<i<<"\t";
             myfile<< x1[count1]<<"\t"<<y1[count1]<<"\t 30 \t 30\t";
			 myfile<<(sTW1[count1])<<"\t"<<(eTW1[count1])<<endl;
			 count1++;
 			 myfile<<endl;
		 }

		 cout<<"Print XONG SP"<<endl;
         //Read data of waiting stations:
		 count1 = startWS;
         for (i=1; i<=nWS; i++)
		 {
			 myfile<<i<<"\t";
             myfile<<x1[count1]<<"\t"<<y1[count1]<<endl;
			 count1++;
 			 myfile<<endl;
		 }
	 }
}


void Problem::DistributeCAP(int TOTALCAP,int numCus, int *capT, int range1, int range2)
{
	int total = 0; int i;
	for (i=1;i<=numCus;i++)
	{
		capT[i] = Rand(range1,range2);
		total += capT[i];
	}
	int TOTAL_ACCEPT = (int) (0.85 * TOTALCAP);
	if (total < TOTAL_ACCEPT)
	{
		int add = TOTALCAP - total;
		int share = (int) (add/numCus);
		if (share <= 1) 
		{
			int count = 0;
			for (i=1;i<=numCus;i++)
			{
				if (capT[i] < range2) {capT[i]++; count++;}
				if (count == add) break;
			}
		}
		else
		{
			for (i=1;i<=numCus;i++)
			{
				capT[i] += share;
				if (capT[i] > range2) capT[i] = range2;
			}
		}
	}
	else if (total > TOTAL_ACCEPT)
	{
		int reduce = total  - TOTAL_ACCEPT;
		int share = (int) (reduce/numCus);
		if (share <= 1) 
		{
			int count = 0;
			for (i=1;i<=numCus;i++)
			{
				if (capT[i] > range1) {capT[i]--; count++;}
				if (count == reduce) break;
			}
		}
		else
		{
			for (i=1;i<=numCus;i++)
			{
				capT[i] -= share;
				if (capT[i] < range1) capT[i] = range1;
			}
		}
	}
}
bool Problem::sameListSP(int idCus1,int idCus2)
{
	//return 1 if cusP idCus1 and idCus2 has at least one available sp
	int i,j, sp1, sp2;
	for (i=1; i<=node[idCus1].numSP; i++)
	{
		sp1 = node[idCus1].sp[i];
		for (j=1;j<=node[idCus2].numSP;j++)
		{
			sp2 = node[idCus2].sp[j];
			if (sp1==sp2) return 1;
			else if (sp1 < sp2) break;
		}
	}
	return 0;
}
void Problem::findNeighborNodes()
{
	int i,j,k,start,end;
	
	VRPNeighborNode1 *neighTemp;
    neighTemp = new VRPNeighborNode1[MAXNUMCUSD];
    int neighSize;
	bool *used; 
	int totalNODE = nSP + nP + nD + nC2C + 1; used = new bool[totalNODE];
	double sTW,eTW,stTW,enTW;

	int MAXNUMCUSD1 = MAXNUMCUSD + 1;
	for (i=startidD;i<=endidD;i++) 
	{
		node[i].neighborSize = 0;
		node[i].neighborNode = new VRPNeighborNode [MAXNUMCUSD1];
	}

	start=startidD;
	for (i=1; i <= nSP; i++)
	{  
		end = endCusDSP[i];//end = start+numCusDSP[i]-1;
        for (j=start; j<=end; j++)
		{
		    neighSize=0;
			stTW = node[j].sTW + node[j].duration;
			enTW = node[j].eTW + node[j].duration;
			//1.overlap TW:
			for (k=start;k<=end;k++) used[k]=0;
			for (k=start; k<=end; k++)
			  if (j!=k)
			  {
				  sTW = stTW + c[j][k]; eTW = enTW + c[j][k];
				  if (PDSINRANGE(sTW, node[k].sTW, node[k].eTW) ||
					  PDSINRANGE(eTW, node[k].sTW, node[k].eTW) ||
					  PDSINRANGE(node[k].sTW,sTW,eTW)||
					  PDSINRANGE(node[k].eTW,sTW,eTW))
				  {
			             neighTemp[neighSize].id = k;
					     neighTemp[neighSize].distance = c[j][k];
					     neighSize++;	used[k]=1;
			 	  }
			  }
		   //outfile<<"Customer "<<j<<" has neighborSize = "<<neighSize<<endl;
			qsort (neighTemp, neighSize, sizeof(VRPNeighborNode1), VRPNeighborNodeCompare);
			for (k=0; k<neighSize;k++)
			{
				 node[j].neighborNode[k].id = neighTemp[k].id;
                 //node[j].neighborNode[k].distance = neighTemp[k].distance;
			}
			//2. feasibile TW:
			int neighSize1=0;
			for (k=start; k<=end; k++)
			  if (j!=k && !used[k])
			  {
				  sTW = stTW + c[j][k]; 
				  if (sTW <= node[k].eTW)
				  {
			             neighTemp[neighSize1].id = k;
					     neighTemp[neighSize1].distance = c[j][k];
					     neighSize1++;	used[k]=1;
				  }
			  }
			qsort (neighTemp, neighSize1, sizeof(VRPNeighborNode1), VRPNeighborNodeCompare);
			int k1;
			for (k=0; k<neighSize1;k++)
			{
				 k1 = k+neighSize;
				 node[j].neighborNode[k1].id = neighTemp[k].id;
                 //node[j].neighborNode[k1].distance = neighTemp[k].distance;
          	}
			node[j].neighborSize = neighSize + neighSize1;
			//3. all remaining customers in this customer zone:
            /*  int neighSize2=0; int curSize = neighSize + neighSize1;
			  for (k=start; k<=end; k++)
			  if (j!=k && !used[k])
			  {
		             neighTemp[neighSize2].id = k;
				     neighTemp[neighSize2].distance = cD[j][k];
				     neighSize2++;	
			  }
			  if (neighSize2 > 0)
			     cout<<"Number neighbors of cusD "<<j<<" in zone "<<i<<": overlapTW = "<<neighSize<<"; feaTW = "<<neighSize1<<"; vioTW = "<<neighSize2<<endl;
			  qsort (neighTemp, neighSize2, sizeof(VRPNeighborNode), VRPNeighborNodeCompare);
		
			for (k=0; k<neighSize2;k++)
			{
				 k1 = k+curSize;
				 nodeD[j].neighborNode[k1].id = neighTemp[k].id;
                 nodeD[j].neighborNode[k1].distance = neighTemp[k].distance;
          	}
			nodeD[j].neighborSizeMAX = curSize + neighSize2;*/
			
		   //cout<<"Customer "<<j<<" has neighborSize = "<<nodes[j].neighborSizeMAX<<" = "<<neighSize<<" + "<<neighSize1<<endl;
		}
		start = end+1; //move to next customer zone
	}
	delete [] neighTemp; 

 
    //FOR PICKUP CUSTOMERS: 
	bool **sameSP;
	int endidP1 = endidP+1;
	sameSP = new bool *[endidP1];
	for (i=startidP; i<=endidP;i++)
	{
		sameSP[i] = new bool[endidP1];
	    for (j=startidP;j<=endidP;j++) 
          if (i != j)
		  {
			  if (sameListSP(i,j)) sameSP[i][j] = 1;
			  else sameSP[i][j]=0;
		  }
		  else sameSP[i][j] = 0;
	}

	neighTemp = new VRPNeighborNode1[nP1];
	for (i=startidP;i<=endidP;i++) 
	{
		node[i].neighborSize = 0;
		node[i].neighborNode = new VRPNeighborNode [nP1];
	}
	
	for (j=startidP; j <= endidP; j++)
	{
		neighSize=0;
		stTW = node[j].sTW + node[j].duration;
		enTW = node[j].eTW + node[j].duration;
		for (k=startidP;k<=endidP;k++) used[k] = 0;
		//1.overlap TW:
		for (k=startidP; k <= endidP; k++)
		   if (sameSP[j][k])
		   {
				sTW = stTW + c[j][k]; eTW = enTW + c[j][k];
				if (PDSINRANGE(sTW, node[k].sTW, node[k].eTW) ||
						  PDSINRANGE(eTW, node[k].sTW, node[k].eTW) ||
						  PDSINRANGE(node[k].sTW,sTW,eTW)||
						  PDSINRANGE(node[k].eTW,sTW,eTW))
					  {
							 neighTemp[neighSize].id = k;
							 neighTemp[neighSize].distance = c[j][k];
							 neighSize++;used[k]=1;
			 		  }
		   }
		qsort (neighTemp, neighSize, sizeof(VRPNeighborNode1), VRPNeighborNodeCompare);
		for (k=0; k<neighSize;k++)
		{
		    node[j].neighborNode[k].id = neighTemp[k].id;
            //node[j].neighborNode[k].distance = neighTemp[k].distance;
		}

		//2. feasibile TW:
		int neighSize1=0;
		for (k=startidP;k<=endidP;k++)
		  if (sameSP[j][k] && !used[k])
		  {
			  sTW = stTW + c[j][k]; 
			  if (sTW <= node[k].eTW)
			  {
		             neighTemp[neighSize1].id = k;
				     neighTemp[neighSize1].distance = c[j][k];
				     neighSize1++;	used[k]=1;
			  }
		  }
		qsort (neighTemp, neighSize1, sizeof(VRPNeighborNode1), VRPNeighborNodeCompare);
		int k1;
		for (k=0; k<neighSize1;k++)
		{
			 k1 = k+neighSize;
			 node[j].neighborNode[k1].id = neighTemp[k].id;
             //node[j].neighborNode[k1].distance = neighTemp[k].distance;
      	}
		node[j].neighborSize = neighSize + neighSize1;
	
		//3. all remaining customers in this customer zone:
        /*int neighSize2=0;
		for (k=1; k<=nP; k++)
		   if (sameSP[j][k] && !used[k]) neighSize2++;	
		if (neighSize2 > 0)
		   cout<<"CusP "<<j<<" has: overlapTW = "<<neighSize<<"; feaTW = "<<neighSize1<<"; vioTW = "<<neighSize2<<endl;
	    */
	}//end for j
	delete [] neighTemp; 

	for (i=startidP; i<=endidP;i++) delete [] sameSP[i];
	delete [] sameSP;

	//FOR C2C CUSTOMERS: 
	//1. Neighbor for insertAfter: cusC2CP insert after any C2C:

	int nC2C1 = nC2C + 1; int cusP1, cusP2, cusD1;
	neighTemp = new VRPNeighborNode1[nC2C1];
	for (i=startidC2C;i<= endidC2C;i++) 
	{
		node[i].neighborSize = 0;
		node[i].neighborNode = new VRPNeighborNode [nC2C1];
	}
	for (cusP1 = startidC2C; cusP1 <= endidC2CP; cusP1++)
	{
		//1. Find neighbors C2CP for C2CP
		neighSize=0;
		stTW = node[cusP1].sTW + node[cusP1].duration;
		enTW = node[cusP1].eTW + node[cusP1].duration;
		for (k=startidC2C;k<=endidC2CP;k++) used[k] = 0;
		//1.1. overlap TW between 2Cus c2cP:
		for (cusP2 = startidC2C; cusP2 <= endidC2CP; cusP2++)
		   if (cusP1 != cusP2)
		   {
				sTW = stTW + c[cusP1][cusP2]; eTW = enTW + c[cusP1][cusP2];
				if (PDSINRANGE(sTW, node[cusP2].sTW, node[cusP2].eTW) ||
						  PDSINRANGE(eTW, node[cusP2].sTW, node[cusP2].eTW) ||
						  PDSINRANGE(node[cusP2].sTW,sTW,eTW)||
						  PDSINRANGE(node[cusP2].eTW,sTW,eTW))
					  {
							 neighTemp[neighSize].id = cusP2;
							 neighTemp[neighSize].distance = c[cusP1][cusP2];
							 neighSize++;used[cusP2]=1;
			 		  }
		   }
		qsort (neighTemp, neighSize, sizeof(VRPNeighborNode1), VRPNeighborNodeCompare);
		for (k=0; k < neighSize;k++)
		{
		    node[cusP1].neighborNode[k].id = neighTemp[k].id;
            //node[j].neighborNode[k].distance = neighTemp[k].distance;
		}
		//1. 2. feasibile TW between 2Cus c2cP:
		int neighSize1=0;
		for (cusP2 = startidC2C; cusP2 <= endidC2CP;cusP2++)
		  if (cusP1 != cusP2 && !used[cusP2])
		  {
			  sTW = stTW + c[cusP1][cusP2]; 
			  if (sTW <= node[cusP2].eTW)
			  {
		             neighTemp[neighSize1].id = cusP2;
				     neighTemp[neighSize1].distance = c[cusP1][cusP2];
				     neighSize1++;	used[cusP2]=1;
			  }
		  }
		qsort (neighTemp, neighSize1, sizeof(VRPNeighborNode1), VRPNeighborNodeCompare);
		int k1;
		for (k=0; k<neighSize1;k++)
		{
			 k1 = k+neighSize;
			 node[cusP1].neighborNode[k1].id = neighTemp[k].id;
             //node[j].neighborNode[k1].distance = neighTemp[k].distance;
      	}
		node[cusP1].neighborSize = neighSize + neighSize1;

		//2. Find neighbors C2CP for C2CD (delCus of CusP1)
		neighSize=0; cusD1 = node[cusP1].delID;
		stTW = node[cusD1].sTW + node[cusD1].duration;
		enTW = node[cusD1].eTW + node[cusD1].duration;
		for (k=startidC2C;k<=endidC2CP;k++) used[k] = 0;
		//2.1. overlap TW between C2CP and C2CD:
		for (cusP2 = startidC2C; cusP2 <= endidC2CP; cusP2++)
		   if (cusP1 != cusP2)
		   {
				sTW = stTW + c[cusD1][cusP2]; eTW = enTW + c[cusD1][cusP2];
				if (PDSINRANGE(sTW, node[cusP2].sTW, node[cusP2].eTW) ||
						  PDSINRANGE(eTW, node[cusP2].sTW, node[cusP2].eTW) ||
						  PDSINRANGE(node[cusP2].sTW,sTW,eTW)||
						  PDSINRANGE(node[cusP2].eTW,sTW,eTW))
					  {
							 neighTemp[neighSize].id = cusP2;
							 neighTemp[neighSize].distance = c[cusP1][cusP2];
							 neighSize++;used[cusP2]=1;
			 		  }
		   }
		qsort (neighTemp, neighSize, sizeof(VRPNeighborNode1), VRPNeighborNodeCompare);
		for (k=0; k < neighSize;k++)
		{
		    node[cusD1].neighborNode[k].id = neighTemp[k].id;
            //node[cusD1].neighborNode[k].distance = neighTemp[k].distance;
		}
		//2.1 feasibile TW between C2CP and C2CD:
		neighSize1=0;
		for (cusP2 = startidC2C; cusP2 <= endidC2CP;cusP2++)
		  if (cusP1 != cusP2 && !used[cusP2])
		  {
			  sTW = stTW + c[cusD1][cusP2]; 
			  if (sTW <= node[cusP2].eTW)
			  {
		             neighTemp[neighSize1].id = cusP2;
				     neighTemp[neighSize1].distance = c[cusD1][cusP2];
				     neighSize1++;	used[cusP2]=1;
			  }
		  }
		qsort (neighTemp, neighSize1, sizeof(VRPNeighborNode1), VRPNeighborNodeCompare);
		for (k=0; k<neighSize1;k++)
		{
			 k1 = k+neighSize;
			 node[cusD1].neighborNode[k1].id = neighTemp[k].id;
             //node[cusD1].neighborNode[k1].distance = neighTemp[k].distance;
      	}
		node[cusD1].neighborSize = neighSize + neighSize1;
	}
	delete [] neighTemp; delete []used;

	//FOR C2C CUSTOMERS: 
	//2. Neighbor for EXCHANGE ORDERS: exchange positions of 2 orders (C2CP1 <--> C2CP2 and C2CD1 <--> C2CD2):
	int nOrder1 = nOrder + 1;
	for (i=startidC2C;i<= endidC2CP;i++) 
	{
		nodeC2C[i].neighborSizeExchange = 0;
		nodeC2C[i].neighborNodeExchange = new VRPNeighborNode [nOrder1];
	}

    double stTWP1, enTWP1, stTWD1, enTWD1,stTWP2, enTWP2, stTWD2, enTWD2, sTW1, eTW1, sTW2, eTW2; int cusD2;
	for (cusP1 = startidC2C; cusP1 <= endidC2CP; cusP1 ++)
	{
		cusD1 = node[cusP1].delID;
		neighSize=0;
		stTWP1 = node[cusP1].sTW + node[cusP1].duration;
		enTWP1 = node[cusP1].eTW + node[cusP1].duration;
		stTWD1 = node[cusD1].sTW + node[cusD1].duration;
		enTWD1 = node[cusD1].eTW + node[cusD1].duration;
		for (cusP2 = startidC2C; cusP2 <= endidC2CP; cusP2++)
		{
			if (cusP1 != cusP2)
		    {
			     cusD2 = node[cusP2].delID;
			     if (node[cusD1].eTW < node[cusP2].sTW || node[cusD2].eTW < node[cusP1].sTW) continue;
		 		 //1.1. overlap TW between 2Cus c2cP:
				 sTW1 = stTWP1 +  c[cusP1][cusP2]; eTW1 = enTWP1 + c[cusP1][cusP2];
				 stTWP2 = node[cusP2].sTW + node[cusP2].duration;
				 enTWP2 = node[cusP2].eTW + node[cusP2].duration;
				 stTWD2 = node[cusD2].sTW + node[cusD2].duration;
				 enTWD2 = node[cusD2].eTW + node[cusD2].duration;
				 sTW2 = stTWP2 +  c[cusP2][cusP1]; eTW2 = enTWP2 + c[cusP2][cusP1];
				 
				 if (PDSINRANGE(sTW1, node[cusP2].sTW, node[cusP2].eTW) ||
						  PDSINRANGE(eTW1, node[cusP2].sTW, node[cusP2].eTW) ||
						  PDSINRANGE(node[cusP2].sTW,sTW1,eTW1)||
						  PDSINRANGE(node[cusP2].eTW,sTW1,eTW1)||
					  PDSINRANGE(sTW2, node[cusP1].sTW, node[cusP1].eTW) ||
						  PDSINRANGE(eTW2, node[cusP1].sTW, node[cusP1].eTW) ||
					      PDSINRANGE(node[cusP1].sTW,sTW2,eTW2)||
					      PDSINRANGE(node[cusP1].eTW,sTW2,eTW2)) 
				 {
			 		 //1.2. overlap TW between 2Cus c2cD:
					 sTW1 = stTWD1 + c[cusD1][cusD2]; eTW1 = enTWD1 + c[cusD1][cusD2];
					 sTW2 = stTWD2 +  c[cusD2][cusD1]; eTW2 = enTWD2 + c[cusD2][cusD1];

					 if (PDSINRANGE(sTW1, node[cusD2].sTW, node[cusD2].eTW) ||
							  PDSINRANGE(eTW1, node[cusD2].sTW, node[cusD2].eTW) ||
							  PDSINRANGE(node[cusD2].sTW,sTW1,eTW1)||
							  PDSINRANGE(node[cusD2].eTW,sTW1,eTW1)||
						 PDSINRANGE(sTW2, node[cusD1].sTW, node[cusD1].eTW) ||
							  PDSINRANGE(eTW2, node[cusD1].sTW, node[cusD1].eTW) ||
							  PDSINRANGE(node[cusD1].sTW,sTW2,eTW2)||
							  PDSINRANGE(node[cusD1].eTW,sTW2,eTW2))  
					 {
						 nodeC2C[cusP1].neighborNodeExchange[neighSize].id = cusP2;
						 neighSize++;
					 }
				 }
			}//end if
		}//end cusP2
		nodeC2C[cusP1].neighborSizeExchange = neighSize;
	}//end cusP1
}


void Problem::takenNeighborNodes()
{
	int i,k,id;

	int totalNODE = nP + nD + nSP + nC2C + 2;
	int MAXNUMCUSD1 = MAXNUMCUSD + 1;

	for (i=startidD;i<=endidD;i++) 
	{
		node[i].beNeighborSize=0;
		node[i].neighborOF = new int [totalNODE];
		node[i].beNeighbor = new VRPBeNeighborNode[MAXNUMCUSD1];
		for (k=0;k<totalNODE;k++) node[i].neighborOF[k] = -1;
		//for (k=startidD;k<=endidD;k++) node[i].neighborOF[k] = -1;
	}

	for (i=startidD;i<=endidD;i++) 
	   for (k=0;k < node[i].neighborSize;k++)
	   {
		   id = node[i].neighborNode[k].id;
		   node[id].beNeighbor[node[id].beNeighborSize].id=i;
		   node[id].beNeighbor[node[id].beNeighborSize].at=k;
		   node[id].beNeighborSize++;
		   node[id].neighborOF[i] = k; //node id is neighbor kth of i 
	   }

	//FOR PICKUP CUSTOMERS:
	for (i=startidP;i<=endidP;i++) 
	{
		node[i].beNeighborSize=0;
		node[i].beNeighbor = new VRPBeNeighborNode[nP1];
		node[i].neighborOF = new int [totalNODE];
		for (k=0;k<totalNODE;k++) node[i].neighborOF[k] = -1;
	    //for (k=startidP;k<=endidP;k++) node[i].neighborOF[k] = -1;
	}
	for (i=startidP;i<=endidP;i++) 
	   for (k=0;k<node[i].neighborSize;k++)
	   {
		   id = node[i].neighborNode[k].id;
		   //cout<<"id = "<<id<<"; i = "<<i<<", k = "<<k<<", nodeP[id].beNeighborSize = "<<nodeP[id].beNeighborSize<<", MAX_NODEP = "<<MAX_NODEP<<endl;
		   node[id].beNeighbor[node[id].beNeighborSize].id = i;
		   node[id].beNeighbor[node[id].beNeighborSize].at = k;
		   node[id].beNeighborSize++;
		   node[id].neighborOF[i] = k; //node id is neighbor kth of i
	   }

	//FOR C2C CUSTOMERS: 
	//1. Neighbor for insertAfter: cusC2CP insert after any C2C:
	for (i=startidC2C;i <= endidC2C;i++) 
	{
		node[i].beNeighborSize=0;
		node[i].beNeighbor = new VRPBeNeighborNode[nC2C1];
		node[i].neighborOF = new int [totalNODE];
		for (k=0;k<totalNODE;k++) node[i].neighborOF[k] = -1;
	}


	for (i=startidC2C;i <= endidC2C;i++) 
	   for (k=0;k<node[i].neighborSize;k++)
	   {
		   id = node[i].neighborNode[k].id;
		   node[id].beNeighbor[node[id].beNeighborSize].id = i;
		   node[id].beNeighbor[node[id].beNeighborSize].at = k;
		   node[id].beNeighborSize++;
		   node[id].neighborOF[i] = k; //node id is neighbor kth of i
	   }

	//FOR C2C CUSTOMERS: 
	//2. Neighbor for EXCHANGE ORDERS: exchange positions of 2 orders (C2CP1 <--> C2CP2 and C2CD1 <--> C2CD2):
	int nOrder1 = nOrder + 1;
	for (i=startidC2C;i <= endidC2CP;i++) 
	{
		nodeC2C[i].beNeighborSizeExchange = 0;
		nodeC2C[i].beNeighborExchange = new VRPBeNeighborNode[nOrder1];
		//nodeC2C[i].neighborOF = new int [nOrder1]; SAI
		nodeC2C[i].neighborOF = new int [startidC2CD]; //Updated 17/1/2015
		for (k=1;k<=nOrder;k++) nodeC2C[i].neighborOF[k] = -1;
	}

	for (i=startidC2C;i <= endidC2CP;i++) 
	{
		//cout<<"C2CP "<<i<<" has "<<nodeC2C[i].neighborSizeExchange<<" neighbors: ";
	    for (k=0;k<nodeC2C[i].neighborSizeExchange;k++)
	   {
		   id = nodeC2C[i].neighborNodeExchange[k].id;
		   //if (id > endidC2CP){cout<<"ERROR: id = "<<id<<" > endidC2CP = "<<endidC2CP<<endl;exit(-1);}
		   //cout<<"id = "<<id<<endl;
		   nodeC2C[id].beNeighborExchange[nodeC2C[id].beNeighborSizeExchange].id = i;
		   nodeC2C[id].beNeighborExchange[nodeC2C[id].beNeighborSizeExchange].at = k;
		   nodeC2C[id].beNeighborSizeExchange++;
		   nodeC2C[id].neighborOF[i] = k;  //node id is neighbor kth of i
	   }
		//cout<<endl;
	}
}


void Problem::checkingData()
{
	int i,j; double temp;
	for (i=startidC2C; i<= endidC2CP; i++)
	{
		j = node[i].delID;
		temp = node[i].eTW + node[i].duration + c[i][j];
		temp -= node[j].eTW;
		if (temp > eps)
		{
			cout<<"ERROR: C2C("<<node[i].id<<", "<<node[j].id<<")"<<endl;
		}
	}
}
Problem::~Problem()
{
	 int i;
	 int totalNODES = nC2C + nP + nD + nSP + nWS + 2;
	 for (i = 0; i < totalNODES; i++) delete [] c[i];
	 delete [] c;
	 //cout<<"OK1 "<<endl;


	int totalNODE1 = nSP + nP + nD + nC2C + 3;
	for (i=0;i<totalNODE1;i++)
	{
		delete [] CWSSP[i]; 
		delete [] disCWSSP[i];
	}
	delete [] CWSSP; 
	delete [] disCWSSP;
	//cout<<"OK2 "<<endl;

	
	for (i =0; i <= nSP; i++)
	{
		delete [] closestPSP[i];
		delete [] leaveSP[i];
	}
	delete [] closestPSP;
    delete [] leaveSP;
	//cout<<"OK3 "<<endl;


	 for (i = startidC2C; i <= endidD; i++)
	 {
		delete [] node[i].neighborNode;
		delete [] node[i].neighborOF;
		delete [] node[i].beNeighbor;
	 }
	//cout<<"OK4 "<<endl;

	delete [] nodeWS;
    delete [] node;
	delete [] nodeSP;
	//cout<<"OK5 "<<endl;

    /*int PC2C = endidP + 1;
	for (i=startidP;i <= endidP;i++)
	{
		delete [] belong[i];
	}
	delete [] belong; */
	//cout<<"OK6 "<<endl;

    int PC2C = endidP + 1;
	for (i=startidC2C;i <= endidP;i++)
	{
		delete [] belong[i];
	}
	delete [] belong; 


	for (i=0;i < PC2C;i++)
	{
		delete [] waitingSOneP[i];
		delete [] startSOneP[i];
		delete [] leaveSPOneP[i];
	}
	delete [] waitingSOneP;	delete [] startSOneP;
	delete [] leaveSPOneP;
	//cout<<"OK7 "<<endl;


    for (i=startidC2C;i<= endidC2CP;i++)  
	{
		delete [] nodeC2C[i].neighborNodeExchange;
		delete [] nodeC2C[i].neighborOF;
		delete [] nodeC2C[i].beNeighborExchange;
	}
	
	/*for (i=startidC2C;i<= endidC2CP;i++)  
	{
		delete [] nodeC2C[i].neighborNodeExchange;
	}*/
	//cout<<"OK8 "<<endl;
	delete [] nodeC2C;
    //cout<<"FINISH free "<<endl;
}

