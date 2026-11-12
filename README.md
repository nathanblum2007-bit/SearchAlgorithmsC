# Console implementations of pathfinding algorithms in C using only standard libraries (stdlib, stdio, math)

Each implementation prints the list of nodes visited in the path from the goal -> start in format (y, x). 

Then prints the total cost, followed by a 2d-grid representation of the map. 


Each node is represented by one of the following characters depending on its role: 

S - Starting node 

G - Goal node 

W - A Wall node 

int x - a node which can be accessed where x = its cost 


Each node is printed in one of the following colours: 

Red - A node not visited 

    - All walls will always be printed red 


Green - A node visited 

      - S will always be green 

      - G will be green in any situation where there is a possible path 


# Grid configurating

Note: the top left corner is [0][0]. The bottom right corner is [height-1][width-1] 


The Grid is initialized to a size of 5x5. 

Start is initialized to [0][0] (Except in GreedyBestFirst.c to showcase its unique pathfinding - it is [2][0]). 

Goal is initialized to [0][4] 

[0][3], [1][3], [2][3], and [3][3] are initialized as walls. 



Setting the starting location - struct Node *start = &realGrid.layout[y][x]; 

Setting the goal location - struct Node *goal = &realGrid.layout[y][x]; 


Setting a wall - realGrid.layout[y][x].isWall = true; 

Setting a nodes cost - realGrid.layout[y][x].cost = int; 


Note: All nodes are initialized with a cost of 1. 


# Example output

(0, 4)(1, 4)(2, 4)(3, 4)(4, 4)(4, 3)(4, 2)(3, 2)(2, 1)(1, 1)(0, 0) 

Total cost: 10.828427 

S51WG      

111W1       (This grid would be coloured in the console) 

111W1       

111W1       

11111       

# Algorithms

BreadthFirstSearch.c - Finds the shortest path to the goal node from the start node 

                     - Does not take node costs into consideration 

                     - Explores the whole map 


Dijkstras.c          - Finds the cheapest path to the goal node from the start node 

                     - Does consider node costs when finding the path 

                     - Explores the whole map 


GreedyBestFirst.c    - Does not always find the shortest path to the goal node from the start node 

                     - Does not take node costs into consideration 

                     - Explores only nodes in the direction of the goal -> faster algorithm 

                     - Uses a heuristic function which calculates manhattan distance 


AStar.c              - Finds the cheapest path to the goal node from the start node 

                     - Does consider node costs when finding the path 

                     - Explores only nodes in the direction of the goal -> faster algorithm 

                     - Uses a heuristic function which calculates manhatten distance 


AStarDiag.c          - Finds the cheapest path to the goal from the start node 

                     - Allows diagonal movements when they would not cut corners through walls 

                     - Does consider node costs when finding the path 

                     - Diagonal movement cost = node cost * sqrt(2) 

                     - Explores only nodes in the direction of the goal -> faster algorithm 

                     - Uses a heuristic function which calculates octile distance 





