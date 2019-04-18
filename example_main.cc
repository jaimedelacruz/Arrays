#include <iostream>
#include "Arrays.hpp"


using namespace std;
using namespace mem;

typedef float fp;

int main()
{

  // --- Example of an Array with normal initialization --- //

  long const nx = 10, ny = 100;
  Array<fp,2> a(ny, nx);
  a = 0.0;

  
  // --- Example of an Array with ranges initialization,
  // --- in this case, the first element in the first dimension is -5 --- //
  
  Array<fp,2> b(-5,-5+ny-1,0,0+nx-1);
  b = 1.0;
  cerr<<"first element of b = b(-5,0) = "<<b(-5,0)<<endl;

  
  // --- add arrays using expression templates --- //

  a = a + b;
  cerr<<"a(0,0) = a(0,0)+b(-5,0) = "<<a(0,0)<<endl;

  
  // --- other operations --- //
  
  a = log(sqrt(exp(a) + cos(b)) + fp(2.0));
  cerr<<"log(sqrt(exp(a) + cos(b)) + 2.0) = "<<a(0,0)<<endl<<endl;;


  // --- Print dimensions --- //

  cerr<<endl<<"a.size() = "<<a.size()<<endl;

  for(int ii=0; ii<2; ii++){
    cerr<<"a.shape["<<ii<<"] ="<< a.shape(ii)<<endl;
    cerr<<"a.offset["<<ii<<"] ="<< a.offset(ii)<<endl;
  }

  cerr<<endl<<"b.size() = "<<b.size()<<endl;

  for(int ii=0; ii<2; ii++){
    cerr<<"b.shape["<<ii<<"] ="<< b.shape(ii)<<endl;
    cerr<<"b.offset["<<ii<<"] ="<< b.offset(ii)<<endl;
  }

  
}
