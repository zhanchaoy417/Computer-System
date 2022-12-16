//-*-c++-*-
#ifndef _Filter_h_
#define _Filter_h_

using namespace std;

class Filter {
  //c
    public:
   
   char divisor;
  char dim;
  char *data;
   
 /*short divisor;
  short dim;
  short *data;  */


  Filter(int _dim);
  int get(int r, int c);
  void set(int r, int c, int value);

  int getDivisor();
  void setDivisor(int value);

  int getSize();
  void info();
};

#endif
