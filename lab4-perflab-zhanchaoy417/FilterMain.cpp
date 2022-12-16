#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Filter.h"
#include "omp.h"

using namespace std;

#include "rdtsc.h"
#include "openmp.h"
//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int
main(int argc, char **argv)
{

  if ( argc < 2) {
    fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
  }

  //
  // Convert to C++ strings to simplify manipulation
  //
  string filtername = argv[1];

  //
  // remove any ".filter" in the filtername
  //
  string filterOutputName = filtername;
  string::size_type loc = filterOutputName.find(".filter");
  if (loc != string::npos) {
    //
    // Remove the ".filter" name, which should occur on all the provided filters
    //
    filterOutputName = filtername.substr(0, loc);
  }

  Filter *filter = readFilter(filtername);

  double sum = 0.0;
  int samples = 0;

  for (int inNum = 2; inNum < argc; inNum++) {
    string inputFilename = argv[inNum];
    string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
    struct cs1300bmp *input = new struct cs1300bmp;
    struct cs1300bmp *output = new struct cs1300bmp;
    int ok = cs1300bmp_readfile( (char *) inputFilename.c_str(), input);

    if ( ok ) {
      double sample = applyFilter(filter, input, output);
      sum += sample;
      samples++;
      cs1300bmp_writefile((char *) outputFilename.c_str(), output);
    }
    delete input;
    delete output;
  }
  fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}

class Filter *
readFilter(string filename)
{
  ifstream input(filename.c_str());

  if ( ! input.bad() ) {
    int size = 0;
    input >> size;
    Filter *filter = new Filter(size);
    int div;
    input >> div;
    filter -> setDivisor(div);
    for (int i=0; i < size; i++) {
      for (int j=0; j < size; j++) {
	int value;
	input >> value;
	filter -> set(i,j,value);
      }
    }
    return filter;
  } else {
    cerr << "Bad input in readFilter:" << filename << endl;
    exit(-1);
  }
}
 double
applyFilter(struct Filter *filter, cs1300bmp *input, cs1300bmp *output)
{

  long long cycStart, cycStop;
  cycStart = rdtscll();
  output -> width = input -> width;
  output -> height = input -> height;

 // int filterSize = filter -> getSize();
  short ih = input -> height - 1;
  short iw = input -> width - 1;
float f_div=filter -> divisor;

  #pragma omp parallel for 
  for(int row = 1; row < ih; row++) {
    for(int col = 1; col < iw; col++) {
      PLANE(0);
      PLANE(1);
      PLANE(2);
    }
  }
  cycStop = rdtscll();
  double diff = cycStop - cycStart;
  double diffPerPixel = diff / (output -> width * output -> height);
  fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
	  diff, diff / (output -> width * output -> height));
  return diffPerPixel;
}
/*
double
applyFilter(class Filter *filter, cs1300bmp *input, cs1300bmp *output)
{
  long long cycStart, cycStop;
  cycStart = rdtscll();
  output -> width = input -> width;
  output -> height = input -> height;
	
	short width = (input -> width) - 1;
	short height = (input -> height) - 1;
	short filterdivisor = filter -> divisor;
	short result, result2;
 
short* data=filter->data;
short* input1;
short* input2;
short* input3;
for(short plane = 0; plane < 3; plane++) {
	for(short row = 1; row < height; row+=1) {
        input1=input->color[plane][row-1];
        input2=input->color[plane][row];
        input3=input->color[plane][row+1];
		for(short col = 1; col < width; col+=2) {

	   result = (input1[col-1] * data[0] );
		 result += (input1[col] * data[1] );
		 result += (input1[col+1] * data[2] );
		
		 result += (input2[col-1] * data[3] );
		 result += (input2[col] * data[4] );
		 result += (input2[col+1] * data[5] );
		
		 result += (input3[col-1] * data[6] );
		 result += (input3[col] * data[7] );
		 result +=(input3[col+1] * data[8] );
    
             result2 = (input1[col] *data[0] );
		 result2 += (input1[col+1] * data[1] );
		 result2 += (input1[col+2] * data[2] );
		
		 result2 += (input2[col] * data[3] );
		 result2 += (input2[col+1] * data[4] );
		 result2 += (input2[col+2] * data[5] );
		
		 result2 += (input3[col] * data[6] );
		 result2 += (input3[col+1] * data[7] );
		 result2 +=(input3[col+2] * data[8] );
         
            output -> color[plane][row][col] = result;
		output -> color[plane][row][col+1] = result2;

	if ( filterdivisor > 1){
	output -> color[plane][row][col] = 	
	  output -> color[plane][row][col] / filterdivisor;
        output -> color[plane][row][col+1] = 	
	  output -> color[plane][row][col+1] / filterdivisor;
	}       
	if ( output -> color[plane][row][col]  > 255 )
	  output -> color[plane][row][col] = 255;
	else if ( output -> color[plane][row][col]  < 0 )
	  output -> color[plane][row][col] = 0;
    if ( output -> color[plane][row][col+1]  > 255 )
	  output -> color[plane][row][col+1] = 255;
	else if ( output -> color[plane][row][col+1]  < 0 )
	  output -> color[plane][row][col+1] = 0;
      }
    }
  } 
  cycStop = rdtscll();
  double diff = cycStop - cycStart;
 double diffPerPixel = diff / (output -> width * output -> height);
  fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
	  diff, diff / (output -> width * output -> height));
  return diffPerPixel;
}
*/