#ifndef __PATH_GEN_H__
#define __PATH_GEN_H__

#include "moveGen.h"
#include "maze.h"
#include "robotState.h"

#define LEFT90 'L'
#define LEFTTIGHT90 '('
#define LEFT45 '<'
#define LEFT135 '/'

#define RIGHT90 'R'
#define RIGHTTIGHT90 ')'
#define RIGHT45 '>'
#define RIGHT135 '\\'

#define STRAIGHT 'F'
#define DIAG_STRAIGHT '|'
#define NONE '_'


void PathGenOrtho(Maze* maze, RobotState* state, char* ortho);

#endif
