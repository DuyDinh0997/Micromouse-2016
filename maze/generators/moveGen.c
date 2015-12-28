#include "maze.h"

int isOptimal(MazeCell* targetCell, MazeCell* compareToCell)
{
    if (targetCell == 0) return 0;
    if (compareToCell == 0) return 1;

    return (targetCell->distance <= compareToCell->distance) ? 1 : 0;
}

int isValidMove(MazeCell* cell1)
{
    if (cell1 == 0) return 0;

    return cell1->distance != 255 ? 1 : 0 ;
}

int isExplored(MazeCell* cell)
{
    if (cell == 0) return 2;
    
    return cell->explored ? 1 : 0;
}

int isNotEqualDistance(MazeCell* cell1, MazeCell* cell2)
{
    if (cell1 == 0 || cell2 == 0) return 0;

    return cell1->distance != cell2->distance ? 1 : 0;
}
#include "processor.h"
char MoveGenGetNextMove(Maze* this, int x, int y, Compass forwardDir)
{
	Processor* proc = SingletonProcessor();

    int isWallFront = MazeHasAWallRelative(this, x, y, forwardDir, FORWARD);
    int isWallLeft = MazeHasAWallRelative(this, x, y, forwardDir, LEFT);
    int isWallRight = MazeHasAWallRelative(this, x, y, forwardDir, RIGHT);
    int isWallBack = MazeHasAWallRelative(this, x, y, forwardDir, BACK);

    MazeCell* forwardCell;
    MazeCell* backCell;
    MazeCell* leftCell;
    MazeCell* rightCell;
    
    forwardCell = MazeGetRelativeAdjacentCell(this, x, y, forwardDir, FORWARD);
    leftCell = MazeGetRelativeAdjacentCell(this, x, y, forwardDir, LEFT);
    rightCell = MazeGetRelativeAdjacentCell(this, x, y, forwardDir, RIGHT);
    backCell = MazeGetRelativeAdjacentCell(this, x, y, forwardDir, BACK);

    char isExploredStraight = isExplored(forwardCell);
    char isExploredLeft = isExplored(leftCell);
    char isExploredRight = isExplored(rightCell);


    if (isWallLeft && isWallFront && isWallRight && !isWallBack)
    {
        return 'B';
    }
    // Move Forward
    // - If there are no wall in front of the mouse
    // - The forward cell distance is less then the left cell's (assuming no //   wall present)
    // - The forward cell distance is less then the right cell's (assuming no //   wall present)
    // - Prefer to go straight if straight is unexplored and still as short as //   distance as another route
    else if ( (!isWallFront && forwardCell != 0) 
        && (isOptimal(forwardCell, leftCell) || isWallLeft)
        && (isOptimal(forwardCell, rightCell) || isWallRight)   
        && (isExploredRight >= isExploredStraight || isWallRight 
            || isNotEqualDistance(forwardCell, rightCell))
        && (isExploredLeft >= isExploredStraight || isWallLeft 
            || isNotEqualDistance(forwardCell, leftCell))
        && isValidMove(forwardCell))
    {
        if (forwardCell->explored)
            return 'F';
        else
            return 'f';
    }
    // Rotate Left
    // - If there is no wall to the left of the mouse
    // - The left cell's distance is less then the forward cell's (assuming no //   wall present)
    // - The left cell's distance is less then the right cell's (assuming no   //   wall present)
    // - Prefer to go straight if straight is unexplored and still as short as //   distance as another route   
    else if( (!isWallLeft && leftCell != 0) 
        && (isOptimal(leftCell, forwardCell) || isWallFront)
        && (isOptimal(leftCell, rightCell) || isWallRight)
        && (isExploredStraight >= isExploredLeft || isWallLeft 
            || isNotEqualDistance(leftCell, forwardCell))
        && (isExploredRight >= isExploredLeft || isWallRight 
            || isNotEqualDistance(leftCell, rightCell))
        && isValidMove(leftCell))
    {
        if (leftCell->explored == 1)
            return 'L';
        else
            return 'l'; 
    }
    // Rotate Right
    // - If there is no wall to the right of the mouse
    // - The right cell's distance is less then the forward cell's (assuming //   no wall present)
    // - The right cell's distance is less then the left cell's (assuming no //   wall present)
    // - Prefer to go straight if straight is unexplored and still as short as //   distance as another route
    else if( (!isWallRight && rightCell != 0)
        && (isOptimal(rightCell, forwardCell) || isWallFront)
        && (isOptimal(rightCell, leftCell) || isWallLeft)
        && (isExploredStraight >= isExploredRight || isWallFront 
            || isNotEqualDistance(rightCell, forwardCell))
        && (isExploredLeft >= isExploredRight || isWallLeft 
            || isNotEqualDistance(rightCell, leftCell))
        && isValidMove(rightCell))
    {
        if(rightCell->explored == 1)
            return 'R';
        else
            return 'r';
    }
    else if (!isWallBack
        &&(isOptimal(backCell, forwardCell) || isWallFront)
        && (isOptimal(backCell, leftCell) || isWallLeft)
        && (isOptimal(backCell, rightCell) || isWallRight))
    {
    	//printf("backCell = %d, frontCell = %d\n", forwardCell->distance, backCell->distance);
    	//printf("isWall = %d %d %d\n", isWallFront, isWallLeft, isWallRight);
    	//printf("Back 2\n");
        return 'b';
    }
    else
    {
        // An error occured!!
        return 0;
    } 

}
