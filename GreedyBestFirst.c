#include <stdlib.h>
#include <stdio.h>

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define RESET   "\033[0m"

struct Node {
        int x;
        int y;
        bool visited;
        bool isWall;
        bool inPath;

        int cost;
        int heurVal;

        struct Node *previousNode;
    };

    //prototype for heuristic
int heuristic(struct Node *curr, struct Node *goal);

int main(void){

    const size_t width = 5;
    const size_t height = 5;

    struct Grid {
        struct Node layout[height][width];
    } realGrid;
    
    //Set the goal position
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
            realGrid.layout[i][j].cost = 1;
            realGrid.layout[i][j].heurVal = heuristic(&realGrid.layout[i][j], goal);
        }
    }

    //Set the walls to whatever youd like
    realGrid.layout[0][3].isWall = true;
    realGrid.layout[1][3].isWall = true;
    realGrid.layout[2][3].isWall = true;
    realGrid.layout[3][3].isWall = true;

    //Set the starting position
    struct Node *start = &realGrid.layout[2][0];
    start->visited = true;
   
    struct Node *frontier[width*height];
    frontier[0] = start;

    struct Node *curr;
    int end = 1;

    const int dx[] = { 0, 1, 0, -1 }; //up right down left
    const int dy[] = { -1, 0, 1, 0 }; 

    while(end > 0){

        //find the index of the node in the frontier with the lowest heurVal
        int lowest = 0;
        for (int i = 1; i < end; i++){
            if(frontier[i]->heurVal < frontier[lowest]->heurVal){
                lowest = i;
            }
        }

        //move the node to the front
        struct Node *tmp = frontier[0];
        frontier[0] = frontier[lowest];
        frontier[lowest] = tmp;

        curr = frontier[0];
        if(curr == goal) break;

        for(int i = 0; i < 4; i++){
            int nx = curr->x + dx[i];
            int ny = curr->y + dy[i];
            //if the neighbour wouldnt exceed the boundaries of the grid, hasnt been visited, and isnt a wall
            if((nx >= 0 && nx <= width-1 && ny >= 0 && ny <= height-1 && realGrid.layout[ny][nx].isWall == false) && realGrid.layout[ny][nx].visited == false ){
                    frontier[end] = &realGrid.layout[ny][nx];
                    frontier[end]->previousNode = &realGrid.layout[curr->y][curr->x];
                    realGrid.layout[ny][nx].visited = true;
                    end++;
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

int heuristic(struct Node *curr, struct Node *goal){
    return abs(curr->x - goal->x) + abs(curr->y - goal->y);
}