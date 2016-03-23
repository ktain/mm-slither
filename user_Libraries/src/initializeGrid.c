#include "maze.h"

/*
 * Function name: initializeGrid()
 * Description: Initializes a 16 by 16 square grid. 
 *              Lower left square is (0, 0).
 *              Initializes distances.
 */
void initializeGrid() {
	
  int i, j, k;
  
  // Map known walls
  block[0][0] |= 14;  // Starting walls
  block[0][1] |= 12;

  for (int j = 0; j < SIZE; j++) {  // Map north wall
    block[SIZE-1][j] |= 1;
  }

  for (int i = 0; i < SIZE; i++) {  // Map east wall
    block[i][SIZE-1] |= 2;
  }

  for (int j = 0; j < SIZE; j++) {  // Map south wall
    block[0][j] |= 4;
  }

  for (int i = 0; i < SIZE; i++) {  // Map west wall
    block[i][0] |= 8;
  }
  
  // Initialize distances for flood fill
  if (SIZE%2 == 1) {    // Odd size
    k = SIZE - 1;
    for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
      distance[i][j] = k;
      if (j < SIZE/2)
        k--;
      else
        k++;
      }
    }
    if (i < SIZE/2)
      k--;
    else 
      k++;
  }
  else if (SIZE%2 == 0) {   // Even size
    k = SIZE - 2;
    for ( i = 0; i < SIZE; i++ ) {
      for ( j = 0; j < SIZE; j++) {
        distance[i][j] = k;
        if (j < (SIZE/2 - 1) )
          k--;
        else if ( (j > SIZE/2 - 1) && (j < SIZE-1) )
          k++;
      }
      if (i < SIZE/2 - 1)
        k--;
      else if ( (i > SIZE/2 - 1) && (i < SIZE-1) )
        k++;
    }
  }
}
