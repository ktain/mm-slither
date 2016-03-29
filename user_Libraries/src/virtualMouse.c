#include <stdio.h>
#include "maze.h"

void mouser(MAZE * maze, const COORD goal, MOUSE * mouse)
{
	isWaiting = 0;
	isSearching = 1;
	isSpeedRunning = 0;

	bool beginCellFlag = 0;
	bool quarterCellFlag = 0;
	bool halfCellFlag = 0;
	bool threeQuarterCellFlag = 0;
	bool fullCellFlag = 0;

	char temp;
	//  0 0 0 0       0 0 0 0
	//    D V T       W S E N
	// [row] [col]
	//  N S   W E
	initDist(maze);
  //Set goal locations 
	COORD goal1 = { .row = 7, .col = 7 };
	COORD goal2 = { .row = 8, .col = 7 };
	COORD goal3 = { .row = 7, .col = 8 };
	COORD goal4 = { .row = 8, .col = 8 };
  maze->dist[goal1.row][goal1.col] = 0;
  maze->dist[goal2.row][goal2.col] = 0;
  maze->dist[goal3.row][goal3.col] = 0;
  maze->dist[goal4.row][goal4.col] = 0;
  updateDist(maze, goal1, WWALL);

	while(maze->dist[mouse->location.row][mouse->location.col] != 0)	
	{
		mouse->orientation = floodfill(maze, mouse);

		switch(mouse->orientation)
		{
			case 'N':
				mouse->location.row++;
				break;
			case 'E':
				mouse->location.col++;
				break;
			case 'W':
				mouse->location.col--;
				break;
			case 'S':
				mouse->location.row--;
				break;
			case 'D':
				printf("FOUND GOAL\n");
				break;
			default:
				printf("IDK \n");
				break;
		}
    mouse->moveCount++;
		visualizeGrid(maze, *mouse);    // Print after
		printf("CURRENT MOUSE LOCATION: row: %d col: %d\n", (int) mouse->location.row, (int) mouse->location.col );
		printf("CURR DIST TO CENTER: %d\n", (int) maze->dist[mouse->location.row][mouse->location.col]);
		printf("Total Moves: %d\n", mouse->moveCount);
		scanf("%c", &temp); // Wait for keypress
	}

	initDist(maze);
  COORD retGoal = { .row = 0, .col = 0 };
  COORD prev = { .row = 1, .col = 0 };
  maze->dist[retGoal.row][retGoal.col] = 0;
  updateDist(maze, prev, 0);
  visualizeGrid(maze, *mouse);
  mouse->moveCount = 0;

  while(maze->dist[mouse->location.row][mouse->location.col] != 0)
  {
    mouse->orientation = floodfill(maze, mouse);

		switch(mouse->orientation)
		{
			case 'N':
				mouse->location.row++;
				break;
			case 'E':
				mouse->location.col++;
				break;
			case 'W':
				mouse->location.col--;
				break;
			case 'S':
				mouse->location.row--;
				break;
			case 'D':
				printf("FOUND GOAL\n");
				break;
			default:
				break;
		}
    mouse->moveCount++;
		printf("CURRENT MOUSE LOCATION: row: %d col: %d\n", (int) mouse->location.row, (int) mouse->location.col );
		printf("CURR DIST TO CENTER: %d\n", (int) maze->dist[mouse->location.row][mouse->location.col]);
		printf("Total Moves: %d\n", mouse->moveCount);
    visualizeGrid(maze, *mouse);
		scanf("%c", &temp); // Wait for keypress
  }

  mouse->moveCount = 0;
	initDist(maze);
  //Set goal locations 
  maze->dist[goal1.row][goal1.col] = 0;
  maze->dist[goal2.row][goal2.col] = 0;
  maze->dist[goal3.row][goal3.col] = 0;
  maze->dist[goal4.row][goal4.col] = 0;
  updateDist(maze, goal1, WWALL);

  while(maze->dist[mouse->location.row][mouse->location.col] != 0)
  {
    mouse->orientation = floodfill(maze, mouse);

		switch(mouse->orientation)
		{
			case 'N':
				mouse->location.row++;
				break;
			case 'E':
				mouse->location.col++;
				break;
			case 'W':
				mouse->location.col--;
				break;
			case 'S':
				mouse->location.row--;
				break;
			case 'D':
				printf("FOUND GOAL\n");
				break;
			default:
				break;
		}
    mouse->moveCount++;
		printf("CURRENT MOUSE LOCATION: row: %d col: %d\n", (int) mouse->location.row, (int) mouse->location.col );
		printf("CURR DIST TO CENTER: %d\n", (int) maze->dist[mouse->location.row][mouse->location.col]);
		printf("Total Moves: %d\n", mouse->moveCount);
    visualizeGrid(maze, *mouse);
		scanf("%c", &temp); // Wait for keypress
  }

	printf("done\n");
}

