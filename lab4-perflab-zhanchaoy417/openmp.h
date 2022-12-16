#ifndef OPENMP_H
#define OPENMP_H

#define \
LOOP(plane, row, col, i, j) { \
  output -> color[plane][row][col] += (input -> color[plane][row + i - 1][col + j - 1] * filter -> data[i * 3 + j]); \
};

#define \
GETFILETER(plane, row, col) { \
  LOOP(plane, row, col, 0, 0); \
  LOOP(plane, row, col, 0, 1); \
  LOOP(plane, row, col, 0, 2); \
  LOOP(plane, row, col, 1, 0); \
  LOOP(plane, row, col, 1, 1); \
  LOOP(plane, row, col, 1, 2); \
 LOOP(plane, row, col, 2, 0); \
  LOOP(plane, row, col, 2, 1); \
  LOOP(plane, row, col, 2, 2); \
};


#define \
PLANE(plane) { \
  output -> color[plane][row][col] = 0;   \
  GETFILETER(plane, row, col);   \
  output -> color[plane][row][col] /= f_div; \
  if ( output -> color[plane][row][col]  < 0 ) {   \
    output -> color[plane][row][col] = 0;   \
  } \
  if ( output -> color[plane][row][col]  > 255 ) {  \
    output -> color[plane][row][col] = 255;  \
  } \
};

#endif