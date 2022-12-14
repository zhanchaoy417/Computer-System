/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


See README-longer.md for more details on debugging & development.
#endif
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    //x xor y= (x or y) and (not x or not y)
    //not(x and y) and not(not x and not y)
    return   ~(x&y)& ~(~x&~y);
}
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
    //Tmax is 011111111
  int temp=(x+1);
    int case1=~(temp)^x;//~temp is max, using ^ check if x is Tmax
    int case2=!(temp);//special case is -1= 1111, if -1 return 1,otherwise 0
  return !(case1 |case2 );//using ! to adjust ans

}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
    //0xAA=10101010, so the odd positions of binary is odd, then creat the odd number bit in the 32-bits
     int temp=(0xAA<<24)+(0xAA<<16)+(0xAA<<8)+(0xAA);
    //if odd numbers bits,then odd number bits of x&temp is 1,we want the outcome is 000000 by using ^, it will easily get 0 because all bits are 0.
  return !(x&temp^temp);
}
/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
   //frist left shift 31 keep the lowest bits then right shift 31 replace
  return x<<0xff>>0xff;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
    //left shift 32-n,then right shift 32-n, if the data are some mean they are fine with given bits
    int y=(33+~n);//32-n
    int z=x<<y>>y;//check
    return !(z^x);//using ^ find if they are same
}
/* 
 * isNegative - return 1 if x < 0, return 0 otherwise 
 *   Example: isNegative(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNegative(int x) {
      //if the highest bit is 1,then it is negative,if it is 0 then it is positive
  return   (x>>0xff)&0x01;
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
    // ex 1000 1000 1000 return 0 //1000 0111 1111 return 1
    //right shift x,y,x or y to check they overflow or not, if overflow then return 1,using &, because
    //only 1&1 return 1
    int a=(x>>0xff) &0x01;
    int b=(y>>0xff)&0x01;
    int c=((x+y)>>0xff) &0x01;
    //case1 (a,b cannot overflow),so (!a^b)always be 1,
    //case 2,one of them a,b compare with c cannot overflow
    int case1=(!a^b);
    int case2=(b^c);  //(a^c)) also works
    //using &, both case happen return 1,then reverse to 0
    
  return !(case1&case2); 
      
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
    //2 case, 1:the sign of x,y are same but x>y, 2:the sign of x,y are different and x>0,y<0
    //frist find the sign of x,y
     int a=(x>>0xff);
    int b=(y>>0xff) ;
// case1,assume x,y>0 or x,y<0, use a^b find it they are equal, and x>y mean y-x<0 so that (~x+y+1) return 1(negative)
    int case1=(~x+y+1)>>0xff & (!(a^b));
    //case2,means y=0,x=1,only 1 probablity return 1
    int case2=(!a&b);

    return (case1|case2);
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
    //ex. 1bytes=4 bit, x to  32bits binary,then left shift 4
    //using << left shift, unsign 64+1=n+(~n), Tmin=-32 Tmax=31
    //2 steps: 1: leftshift ,2 :number that exceed  need to add in the end after shifting
    //step1
    int a=x<<n;
      //step2 left shif n,right shift 32-n 
    int b= (  ~(~(0x00)<<n) & (x>>(33+(~n)))  ) ;
  return (a+b);
}
/* 
 * greatestBitPos - return a mask that marks the position of the
 *               most significant 1 bit. If x == 0, return 0
 *   Example: greatestBitPos(96) = 0x40
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4 
 */
int greatestBitPos(int x) {
    //96 is 11000000 turn mask 01000000, reversethe highest bits ,0x40 is 01000000
    // | , ~
//(divide half) to check the greatest position, use OR condition(0|1 is 1)to find the greatest postition contain 1
//recitation talk about!(!(x>>16))<<4;
    x=x>>1 |x;
    x=x>>2 |x;
    x=x>>4 |x;
    x=x>>8 |x;
    x=x>>16 |x;
    //mask of positive number and negative number
    //different is ~x&x find the mask in the end,(~x>>1)mean we right shift first,so that mark most significant 1 bit
    //left side is general find the mask, right side is find the negative and 0,(0x01<<0xff) is 0x80000000,so we 
    //always keep the 1 if negative, and always return 0 if 0 because & operator say only both 1 be kepp
    return  ((~x>>1)&x) |(0x01<<0xff)&x;
    
  

}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) { 
    //two's complement 12 is 01100,298 is 0100101010,5 is 1100,0 is 0,-1 is 1
    //Use dichotomy (divide half)to be more efficient，a is frist 16 bits,b is next 8 bits, c is next 4 bits,d
    int a,b,c,d,e,result;
    int sign=x>>0xff;
   //find two's complement number
    //if x is negative then the sign is not change and other bits get inverse correspond left side
    //if x is positive then all bits are not change
    x=(sign&~x)|(~sign&x);
    
    //we want to check if the bit contain 1 or not, using double !! make sure return 1 if bits contain 1
   a= !(!(x>>16))<<4;
    x=x>>a; //give 1111 then check next
   b=!(!(x>>8))<<3;
    x=x>>b;   //give 111 then check next
    c=!(!(x>>4))<<2;
    x=x>>c;  //give 11 then check next
    d=!(!(x>>2))<<1;
    x=x>>d;  //give 1 then check next
    e=!(!(x>>1))<<0;
    x=(x>>e)+1 ;  // give addition 1 for sign because(positive number)
   result=a+b+c+d+e+x;
  return result;


     
}
/*
 * Extra credit
 */
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
   // define NaN,32bits,1st is sign,8bit is 11111111,last 23 bits has at least one "1"
    //then minNaN=0(positive)11111111(22bits are 0)last bit is 1
    //maxNaN=0(positive)11111111(23bits are 1)
  int  max_NaN=0x7fffffff;
   int min_NaN=0x7f800001;
   //using &max_NaN get mask,a nagtive float will become positive,since the higherst bit of 
   //max is 0,then the highest bit of mask is 0 too.
    
   //negative,compare other bits except highest bit, if still less than min,mean orginal float
   //has some 0s,so it make it more negative
    int obj=(uf&max_NaN);
      if( obj <min_NaN)
      {
          return obj; 
      }
    //positve
      else
    {
        return uf;
    }

 
}
/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
    //IEEE said the 1st bit is the sign,next 8 bit represent exponement part, the 23 bits are fraction
//first find the sign, E, exp and frac parts
    int ss;
    int s=uf>>0xff; //sign
    int E=((uf&0x7f800000)>>23); //e
    int exp=E-127; //8 bit expoent
    //frac part, left side is find the last 23 bits using &, right side is add the sign of exponement part
    int f=(uf&0x007fffff)|0x00800000; 
 
    //overflow
    if(exp>31){return 0x80000000u; }
    //underflow
    if(exp<0){return 0;}
    //exp is 23 bits now is more than 23 bits, then substract and left shif
    if(exp>23)
    {
        f=f<<(exp-23);
    }
    //other right shift and add the missing bits
    else{
        f=f>>(23-exp);
    }
    
    ss=f>>0xff;// find the sign of the frac as "ss"
    //first case is not overflow and positvie, second case is overflow because sign is different
    //third case is not overflow and negative, find two's complement number
    if(!(s^ss))
   {return f;}
   else if(ss)  
   {
       return 0x80000000u; 
   }
    else
    {
        return ~f+1;
    }
}
