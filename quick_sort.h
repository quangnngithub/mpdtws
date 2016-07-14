#ifndef QUICK_SORT
#define QUICK_SORT

namespace quicksort{


  template< class T >
  int pivot( T* t, int *ID, int i, int j )
  {
    T p = t[ID[i]];
    int k = i + 1;
    int l = j;
    int temp;
    while( (t[ID[k]] <= p) && (k < j) ) k++;
    while( t[ID[l]] > p ) l--;
    while( k < l ){
      temp = ID[k];ID[k]=ID[l];ID[l]=temp; //swap(ID, k, l );
      while( t[ID[k]] <= p ) k++;
      while( t[ID[l]] > p ) l--;
    }
    temp = ID[i];ID[i]=ID[l];ID[l]=temp; //swap(ID, i, l );
    return l;
  }

};


template< class T >
void quick_sort( T* t, int *ID, int i, int j )
{
  if( i < j ){
    int l = quicksort::pivot( t, ID, i, j );
    quick_sort( t,ID, i, l-1 );
    quick_sort( t,ID, l+1, j );
  }

}


#endif

