#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define RESET   "\033[0m"

struct Node {
        int x;
        int y;
        bool visited;
        bool isWall;
        bool inPath;
        bool inFrontier;

        int cost;
        double costSoFar;

        double heurVal;

        double priority;

        struct Node *previousNode;
};

double heuristic(struct Node *curr, struct Node *goal);

int main(void){

    const size_t width = 5;
    const size_t height = 5;

    struct Grid {
        struct Node layout[height][width];
    } realGrid;

    struct Node *goal = &realGrid.layout[0][4];
    goal->x = 4;
    goal->y = 0;
    
    for(int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            realGrid.layout[i][j].x = j;
            realGrid.layout[i][j].y = i;
            realGrid.layout[i][j].visited = false;
            realGrid.layout[i][j].previousNode = NULL;
            realGrid.layout[i][j].isWall = false;
            realGrid.layout[i][j].inPath = false;
            realGrid.layout[i][j].inFrontier = false;
            realGrid.layout[i][j].cost = 1;
            realGrid.layout[i][j].costSoFar = -1;
            realGrid.layout[i][j].heurVal = heuristic(&realGrid.layout[i][j], goal);
            realGrid.layout[i][j].priority = -1;
        }
    }

    //Set the walls to whatever youd like
    realGrid.layout[0][3].isWall = true;
    realGrid.layout[1][3].isWall = true;
    realGrid.layout[2][3].isWall = true;
    realGrid.layout[3][3].isWall = true;

    //Set the unique costs to whatever youd like
    realGrid.layout[0][1].cost = 5;
    //realGrid.layout[3][2].cost = 2;

    //Set the starting positions
    struct Node *start = &realGrid.layout[0][0];
    start->visited = true;
    start->costSoFar = 0;

    struct Node *frontier[width*height];
    frontier[0] = start;
    start->inFrontier = true;

    struct Node *curr;
    int end = 1;

    const int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 }; //up, top-right, right, bottom-right, down, bottom-left, left, top-left
    const int dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 }; 

    while(end > 0){

        //find the index of the highest (lowest number) priority node
        int cheapest = 0;
        for (int i = 1; i < end; i++){
            if(frontier[i]->priority < frontier[cheapest]->priority){
                cheapest = i;
            }
        }

        //move the node to the front
        struct Node *tmp = frontier[0];
        frontier[0] = frontier[cheapest];
        frontier[cheapest] = tmp;

        curr = frontier[0];
        if(curr == goal) break;

        for(int i = 0; i < 8; i++){
            int nx = curr->x + dx[i];
            int ny = curr->y + dy[i];
            //if the neighbour wouldnt exceed the boundaries of the grid, and isnt a wall
            if((nx >= 0 && nx <= width-1 && ny >= 0 && ny <= height-1 && realGrid.layout[ny][nx].isWall == false)){
                if(!((i == 1 || i == 3 || i == 5 || i == 7) && (realGrid.layout[ny][curr->x].isWall == true || realGrid.layout[curr->y][nx].isWall == true))){
                    double nCost = -1;
                    if(i == 1 || i == 3 || i == 5 || i == 7){
                        nCost = curr->costSoFar + realGrid.layout[ny][nx].cost*pow(2, 0.5);
                    }else{
                        nCost = curr->costSoFar + realGrid.layout[ny][nx].cost;
                    }
                    //if the neighbour either hasnt been visited yet, or if it has, if the new costSoFar is cheaper than the one previously assigned to it, take that new cheaper path instead:
                    if(realGrid.layout[ny][nx].costSoFar == -1 || (nCost < realGrid.layout[ny][nx].costSoFar)){
                        //change to the new costSoFar
                        realGrid.layout[ny][nx].costSoFar = nCost;
                        //priority is the new costSoFar + heuristic function
                        realGrid.layout[ny][nx].priority = nCost + realGrid.layout[ny][nx].heurVal;
                        realGrid.layout[ny][nx].previousNode = &realGrid.layout[curr->y][curr->x];
                        realGrid.layout[ny][nx].visited = true;

                        //nodes can be added to the frontier multiple times -> prevents overflow of the array
                        if(!realGrid.layout[ny][nx].inFrontier){
                            frontier[end] = &realGrid.layout[ny][nx];
                            frontier[end]->inFrontier = true;
                            end++;
                        }
                    }
                }
            }
        }
        //removing the frontmost element from the frontier by overwriting it/moving everyone over 1
        for(int i = 0; i < width*height-1; i++){
            frontier[i] = frontier[i + 1];
        }
        curr->inFrontier = false;
        end--;
    }

    curr = goal;
    if(!curr->visited){
        printf("No path");
    }else{
        while(curr != NULL){
            printf("(%d, %d)", curr->y, curr->x);
            realGrid.layout[curr->y][curr->x].inPath = true;
            curr = curr->previousNode;
        }
    }

    printf("\nTotal cost: %f",goal->costSoFar);

    for(int i = 0; i < height; i++){
        printf("\n");
        for (int j = 0; j < width; j++){
            if(realGrid.layout[i][j].x == start->x && realGrid.layout[i][j].y == start->y){
                printf(GREEN "S" RESET);
            }else if (realGrid.layout[i][j].x == goal->x && realGrid.layout[i][j].y == goal->y){
                printf(GREEN "G" RESET);
            }else if(realGrid.layout[i][j].inPath){
                printf(GREEN "%d" RESET,realGrid.layout[i][j].cost);
            }else if(realGrid.layout[i][j].isWall){
                printf(RED "W" RESET);
            }else{
                printf(RED "%d" RESET,realGrid.layout[i][j].cost);
            }
        }
    }
}

//octile distance: does diagonal distance as much as it can before straight steps
//eg dx = 5, dy = 2: 2 diag, 3 straight (diag does both one x and y)
//octile = 2*root2 + 3 = 5.83
double heuristic(struct Node *curr, struct Node *goal){
    int dx = abs(curr->x - goal->x);
    int dy = abs(curr->y - goal->y);
    int max = dx > dy ? dx : dy;
    int min = dx > dy ? dy : dx;
    return (min * sqrt(2)) + (max - min);
}