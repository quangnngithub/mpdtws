#ifndef QUICK_SORT_ARR
#define QUICK_SORT_ARR

namespace quicksortarr{


  template< class T >
  int pivot( T* t, int i, int j )
  {
    T p = t[i];
    int k = i + 1;
    int l = j;
    int temp;
    while( (t[k] <= p) && (k < j) ) k++;
    while( t[l] > p ) l--;
    while( k < l ){
      temp = t[k];t[k]=t[l];t[l]=temp; //swap(ID, k, l );
      while( t[k] <= p ) k++;
      while( t[l] > p ) l--;
    }
    temp = t[i];t[i]=t[l];t[l]=temp; //swap(ID, i, l );
    return l;
  }

};


template< class T >
void quick_sort_arr( T* t, int i, int j )
{
  if( i < j ){
    int l = quicksortarr::pivot( t, i, j );
    quick_sort_arr( t, i, l-1 );
    quick_sort_arr( t,l+1, j );
  }

}


#endif

