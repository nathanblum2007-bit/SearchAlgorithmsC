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
            realGrid.layout[i][j].previousNode = NULL;
            realGrid.layout[i][j].isWall = false;
            realGrid.layout[i][j].inPath = false;
        }
    }

    //Set the walls to whatever youd like
    realGrid.layout[0][3].isWall = true;
    realGrid.layout[1][3].isWall = true;
    realGrid.layout[2][3].isWall = true;
    realGrid.layout[3][3].isWall = true;

    //Set the starting and goal positions
    struct Node *start = &realGrid.layout[0][0];
    start->visited = true;
   
    struct Node *goal = &realGrid.layout[0][4];

    struct Node *frontier[width*height];
    frontier[0] = start;

    struct Node *curr;
    int end = 1;
    int front = 0;

    const int dx[] = { 0, 1, 0, -1 }; //up right down left
    const int dy[] = { -1, 0, 1, 0 }; 

    while(front < end){
        curr = frontier[front];
        if(curr == goal) break;

        for(int i = 0; i < 4; i++){
            int nx = curr->x + dx[i];
            int ny = curr->y + dy[i];

            //If the neighbour wouldnt leave the boundaries of the grid, hasnt been visited, and isnt a wall
            if(nx >= 0 && nx <= width-1 && ny >= 0 && ny <= height-1 && realGrid.layout[ny][nx].visited == false 
                    && realGrid.layout[ny][nx].isWall == false){
                //add it to the end of the frontier
                frontier[end] = &realGrid.layout[ny][nx];
                frontier[end]->previousNode = &realGrid.layout[curr->y][curr->x];
                realGrid.layout[ny][nx].visited = true;
                end++;
            }
        }
        front++;
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
                printf(GREEN "1" RESET);
            }else if(realGrid.layout[i][j].isWall){
                printf(RED "W" RESET);
            }else{
                printf(RED "1" RESET);
            }
        }
    }
}