#include "maze.h"
#include <stdio.h>

/*
 * Filename: graphics.c
 * Description: Contains functions that print the grid to stdout.
 */

/* 
 * Function name: printGrid()
 * Description: Prints the encoded wall information starting with 
 *              the topmost row and working its way down.
 */
void printGrid() {
  for (int i = SIZE - 1; i >= 0; i--) {
    for (int j = 0; j < SIZE; j++) {
      printf("%3d", block[i][j]);
    }
		printf("\n\r");
  }
}

/* 
 * Function name: visualizeGrid()
 * Description: Visualize the grid by decoding the value of
 *              each block into a specific wall configuration.
 *              Then prints the wall layout.
 */
void visualizeGrid() {
  for (int i = SIZE - 1; i >= 0; i--) {
    

    // Print north cell wall 
    for (int j = 0; j < SIZE; j++) {
      if ( hasNorth(block[i][j]) )
        if (HIDEEAST)
          printf("+---");
        else 
          printf("+---+");
      else
        if (HIDEEAST)
          printf("+   ");
        else
          printf("+   +");
    }
    
    if (HIDEEAST) {
      printf("+");
    }
    
    printf("\n\r");
  
    /*
     * Print west and east wall, object, and traces
     */
    for (int j = 0; j < SIZE; j++) {
      
      // Print west wall
      if ( hasWest(block[i][j]) )
        printf("|");
      else
        printf(" ");
      
      // Print if object present
      if ( i == yPos && j == xPos ) {
        if (orientation == 'N')
          printf(" ^ ");
        else if (orientation == 'E')
          printf(" > ");
        else if (orientation == 'S')
          printf(" v ");
        else if (orientation == 'W')
          printf(" < ");
      }
      
      // Print markers
      else if ( isDeadEnd(block[i][j]) )
        printf(" x ");
      else if ( hasTrace(block[i][j]) ) {
        printf(" . ");
      }
      else {
        printf("%3d", distance[i][j]);
      }
      
      // Opt to print east wall
      if (!HIDEEAST) {
        if ( hasEast(block[i][j]) )
          printf("|");
        else
          printf(" ");
      }
    }
    
    // Print east boundary if necessary
    if (HIDEEAST){
      printf("|");
    }
    
    printf("\n\r");
    
    // Opt to print south wall
    if (!HIDESOUTH) {
      for (int j = 0; j < SIZE; j++) {
        if ( hasSouth(block[i][j]) ) {
          if (HIDEEAST) 
            printf("+---");
          else
            printf("+---+");
        }
        else {
          if (HIDEEAST)
            printf("+   ");
          else 
            printf("+   +");
        }
      }
      
      if (HIDEEAST) {
        printf("+");
      }
      
      printf("\n\r");
    }
  }
  
    // Print south boundary if necessary 
    if(HIDESOUTH) {
      for(int i = 0; i < SIZE; i++) {
        if(HIDEEAST)
          printf("+---");
        else 
          printf("+---+");     
      }
      
      if (HIDEEAST) {
        printf("+");
      }
      
      printf("\n\r");
    }
  
}
