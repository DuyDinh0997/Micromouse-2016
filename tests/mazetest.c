#include <stdio.h>
#include "maze.h"
#include "floodFill.h"
#include "robotState.h"
#include "moveGen.h"
#include "pathGen.h"

// Imports a maze represented in ascii
void importMaze(const char* fileName, Maze* maze)
{
    FILE* pFile;
    pFile = fopen(fileName, "r");

    if (pFile == 0) return;

    char line[255];
    int lc = 0;
 
    while (1)
    {
        if (fgets(line, 255, pFile) == NULL) break;

        // Check for North facing walls        
        if (lc % 2 == 0)
        {
            int x;
            for (x = 0; x < 16; x++)
            {
                if (line[x*4+1] != ' ')
                {
                    maze->maze[x][(int)lc/2].northWall = 1;
                }       
            } 
        }
        // Check for west facing walls
        else
        {
            int x;
            for (x = 0; x < 17; x++)
            {
                if (line[x*4] != ' ')
                {
                    maze->maze[x][(int)lc/2].westWall = 1;
                }
            }
        }

        lc++;
    }
    
 
   /* int lc = 0;
    if (myfile.is_open())
    {
        while( getline(myfile, line))
        {           
            if (lc % 2 == 0)
            {
                for (int x = 0; x < 16; x++)
                {
                    if (line.at(x*4+1) != ' ')
                    {
                        maze->setWall(x, (int)lc/2, true, NORTH);
                    }
                }
            }
            else
            {
                for (int x = 0; x < 17; x++)
                {
                    if (line.at(x*4) != ' ')
                    {
                        maze->setWall(x, (int)lc/2, true, WEST);
                    }
                }
            }

            lc++;
            
        }
        myfile.close();
    }
    else
    {
        std::cout << "Could not open maze" << std::endl;
    }

    int x, y;
    for (x = 0; x < 17; x++)
    {
        for (y = 0; y < 17; y++)
        {
            maze->getCell(x, y)->explored = true;
        }
    }*/
}

int main()
{
    printf("Maze Test\n");

    // Maze Init
    Maze maze;
    MazeReset(&maze);
    importMaze("mazes/CAMM2015.txt", &maze);
    MazePrint(&maze, 0, 0);

    // Robot State Init 
    RobotState state;
    MousePositionInit(&state.pos, NORTH, 0, 15);
    state.targetXmin = 7;
    state.targetYmin = 7;
    state.targetXmax = 8;
    state.targetYmax = 8;    

    // Execute Flood Fill on current maze
    FloodFill floodFill;
    floodFill.state = &state;
    floodFill.maze = &maze;
    SimpleFloodFillSolve(&floodFill);

    char ortho[255]; 
    PathGenOrtho(&maze, &state, ortho);

    printf("Ortho = %s\n", ortho);

    MazePrint(&maze, state.pos.x, state.pos.y);
   
    printf("Done!\n");
    return 0;
}
