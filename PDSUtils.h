#ifndef _PDS_UTIL_H
#define _PDS_UTIL_H


// Misc
#define TOTALNODE1_LM 3710 //=nSP+nP+nD+nWS+1
#define TOTALNODE2_LM 3680 //=nSP+nP+nD+1
#define TOTALNODE3_LM 3610 //=nP+nD+1

#define PDSABS(a) (((a)<0) ? -(a): (a))
#define NUM_ELITE_SOLUTIONS		200
#define MAX_NUM_COLS			10000
#define NUM_ENTRIES				8
#define MAX_VRPH_TABU_LIST_SIZE 50


#define MAX_NEIGHBORING_ROUTES		5
#define DUPLICATE_TRIP				0
#define OVERWRITTEN_TRIP			1
#define ADDED_TRIP					2
#define BETTER_TRIP					3

#define HASH_TABLE_SIZE			(1<<18)
#define SALT_1					0
#define SALT_2					11

#define eps                     0.000999999999999
#define eps1                    1.0e-3
#define eps2                    -1.0e-3

#define MAX_SP 30;
#define MAX_NBPICK 100;
#define MAX_NBDEL 100;


//#define TMMINRANGE(x,a,b) ((x-a >1.0e-3 && b-x > 1.0e-3) ? 1:0)

extern double epsilon; //to find best next supply point (for initial solution)
extern double VTWCUS, VTWSP, VCAP; //penalty
extern double alpha1, alpha2;

extern char *middle;


struct htable_entry
{
	///
	/// Each entry in the hash table will contain
	/// an array of num_vals valid entries in hval[].
	/// Each entry is produced by hashing using
	/// SALT_2.  The length array contains the lengths
	/// of the routes in this position in the hash table.
	///

	int num_vals;
	int hash_val_2[NUM_ENTRIES];
	int tot;
	double cost[NUM_ENTRIES]; //cost of each trip having the same value of has_val, but different hash_val_2

};
struct int_int
{
	int i;
	int j;
};
struct double_int
{
	double d;
	int k;
};


struct VRPNeighborNode
{
public:
	int id;
	double distance; 	//Quangnn++ for compiling
};

class VRPNeighborNode1
{
public:
	int id;
	double distance;
};

struct VRPBeNeighborNode
{
public:
	int id;
	int at;
};

class VRPViolation
{
public:
	double length_violation;
	int    capacity_violation;
};




typedef struct variation *Variationptr; //for deteteCus, insertCus, twoOPTCus of UNITABU
struct variation
{
  double var_cost, var_fitness;
  int var_vioCAP;
  double var_vioTWC;

  variation()
  {
      var_cost = 0; var_fitness=0; 
      var_vioCAP = 0;
      var_vioTWC = 0;
  }

  variation & operator=( const variation& v)
  {
    if (&v == this) return *this;
    var_cost = v.var_cost;
	var_fitness = v.var_fitness;
    var_vioCAP = v.var_vioCAP;
    var_vioTWC = v.var_vioTWC;
    return *this;
  }
    
    ~variation(){}
  
};





int VRPNeighborNodeCompare (const void *a, const void *b);

// Create a random number in the range [i,j]
int Rand(int i, int j);
void RandPermutation(int n, int *source, int * per);

bool PDSDIFINT(int a, int b); //a != b

bool PDSDIF(double a, double b); //a !=b
bool PDSEQUAL(double a, double b); //a == b

bool PDSINRANGE(double x, double a, double b);

bool PDSGREATER(double a, double b);//a>b
bool PDSGREATEROREQUAL(double a, double b); //a>=b
bool PDSLESSOREQUAL(double a, double b); //a<=b

#endif


