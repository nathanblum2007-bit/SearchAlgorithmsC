#include <stdlib.h>
#include <stdio.h>

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define RESET   "\033[0m"

int main(void){

    const size_t width = 5;
    const size_t height = 5;

    struct Node {
        int x;
        int y;
        bool visited;
        bool isWall;
        bool inPath;

        int cost;
        int costSoFar;

        struct Node *previousNode;
    };

    struct Grid {
        struct Node layout[height][width];
    } realGrid;
    
    for(int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            realGrid.layout[i][j].x = j;
            realGrid.layout[i][j].y = i;
            realGrid.layout[i][j].visited = false;
            realGrid.layout[i][j].previousNode = nullptr;
            realGrid.layout[i][j].isWall = false;
            realGrid.layout[i][j].inPath = false;
            realGrid.layout[i][j].cost = 1;
            realGrid.layout[i][j].costSoFar = -1;
        }
    }

    //Set the walls to whatever youd like
    realGrid.layout[0][3].isWall = true;
    realGrid.layout[1][3].isWall = true;
    realGrid.layout[2][3].isWall = true;
    realGrid.layout[3][3].isWall = true;

    //Set the unique costs to whatever youd like
    //realGrid.layout[0][1].cost = 5;

    //Set the starting and goal positions
    struct Node *start = &realGrid.layout[0][0];
    start->visited = true;
    start->cost = 0;
    start->costSoFar = 0;
   

    struct Node *goal = &realGrid.layout[0][4];
    goal->cost = 0;

    struct Node *frontier[width*height];
    frontier[0] = start;

    struct Node *curr;
    int end = 1;

    const int dx[] = { 0, 1, 0, -1 }; //up right down left
    const int dy[] = { -1, 0, 1, 0 }; 

    while(end > 0){

        //find the index of the cheapest node in the frontier
        int cheapest = 0;
        for (int i = 1; i < end; i++){
            if(frontier[i]->costSoFar < frontier[cheapest]->costSoFar){
                cheapest = i;
            }
        }

        //move the cheapest node to the front
        struct Node *tmp = frontier[0];
        frontier[0] = frontier[cheapest];
        frontier[cheapest] = tmp;

        curr = frontier[0];
        if(curr == goal) break;

        for(int i = 0; i < 4; i++){
            int nx = curr->x + dx[i];
            int ny = curr->y + dy[i];
            int nCost = curr->costSoFar + curr->cost;
            //if the neighbour wouldnt exceed the boundaries of the grid, and isnt a wall
            if((nx >= 0 && nx <= width-1 && ny >= 0 && ny <= height-1 && realGrid.layout[ny][nx].isWall == false)){
                //if the neighbour either hasnt been visited yet, or if it has, if the new costSoFar is cheaper than the one previously assigned to it, take that new cheaper path instead:
                if(realGrid.layout[ny][nx].costSoFar == -1 || (nCost < realGrid.layout[ny][nx].costSoFar)){
                    //change to the new costSoFar
                    realGrid.layout[ny][nx].costSoFar = nCost;
                    frontier[end] = &realGrid.layout[ny][nx];
                    frontier[end]->previousNode = &realGrid.layout[curr->y][curr->x];
                    realGrid.layout[ny][nx].visited = true;
                    end++;
                }
            }
        }
        //removing the frontmost element from the frontier by overwriting it/moving everyone over 1
        for(int i = 0; i < width*height-1; i++){
            frontier[i] = frontier[i + 1];
        }
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