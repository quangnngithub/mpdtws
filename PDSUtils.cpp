#include "PDS.h"
using namespace std;

double epsilon; //to find best next supply point (for initial solution)
double alpha1, alpha2;
double VTWCUS, VTWSP, VCAP; //penalty
char *middle;


int VRPNeighborNodeCompare (const void *a, const void *b)
{
	/// 
	/// Compares two VRPNeighborNode using the distance field.
	///

	VRPNeighborNode1 *s1;
	VRPNeighborNode1 *s2;

	s1= (VRPNeighborNode1 *)a;
	s2= (VRPNeighborNode1 *)b;

	if( s1->distance > s2->distance)
		return 1;
	else
	{
		if( s1->distance <= s2->distance)
			return -1;
		else
			return 0;

	}
}

int VRPIntCompare(const void *a, const void *b)
{
	///
	/// Compares two int's.
	///

	return ( *(int*)a - *(int*)b );

}

// Create a random number in the range [i,j]
int Rand(int i, int j)
{
 // srand(time(NULL));
  int iInterval = j - i + 1;
  return rand() % iInterval + i;
}

void RandPermutation(int n, int *source, int * per)
{
  int i,j;
  per[1] = source[1];
  for (i=2;i<=n;i++)
  {
	  j = Rand(1,i);
	  if (j!=i) per[i] = per[j];
	  per[j] = source[i];
  }
}


bool PDSINRANGE(double x, double a, double b)
{
	if ((x >= a || abs(x-a) < 0.000999999999999) && 
		(x<=b || abs(b-x)<0.000999999999999)) return 1;
	else return 0;
}

bool PDSGREATER(double a, double b)//a>b
{
	double temp = a-b;
	if (temp > eps) return 1;
	return 0;
}

bool PDSDIF(double a, double b) //a !=b
{
	if (fabs(a-b) < eps) return 0;
	else return 1;
}

bool PDSEQUAL(double a, double b) //a ==b
{
	if (fabs(a-b) > eps1) return 0;
	else return 1;
}

bool PDSGREATEROREQUAL(double a, double b) //a>=b
{
	if (a>=b || fabs(a-b) < eps) return 1;
	else return 0;
}

bool PDSLESSOREQUAL(double a, double b) //a<=b
{
	if (a<=b || fabs(a-b) < eps) return 1;
	else return 0;
}


