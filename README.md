# Breadcrumbs

This console application generates a labyrinth with a random path from a starting point 'S' to an exit 'E', optionally including a treasure '@'.

## How to Run the Code

### Prerequisites

- A C++ compiler such as `g++`
- C++ standard library support (C++11 or later)

### Compilation

To compile the code, use the following command in your terminal:

```sh
g++ -o maze_generator maze_generator.cpp
```

To run the compiled program, use:

```sh
./maze_generator
```
You will be prompted to enter the number of rows and columns for the labyrinth. The program will then generate and display the labyrinth

## Example
```sh
$ ./maze_generator
5 5
# . # # #
# . . . E
# # # . #
# S . . #
# # # # #
```

## Explanation of Symbols

```#``` Wall - Represents the boundaries or obstacles within the labyrinth that cannot be passed through.

```.``` Path - Represents the navigable path from the start to the exit or between key points within the labyrinth.

```S``` Start - The starting point of the labyrinth where the pathfinding begins.

```E``` Exit - The endpoint of the labyrinth where the pathfinding concludes.

```@``` Treasure - An optional point in the labyrinth representing a treasure.

## Explanation of Efficiency

### Time Complexity

read_size() - O(1): This function reads the input size and validates it. The complexity is constant as it performs a fixed amount of operations.

matrix_initialization() - O(n * m): This function initializes the matrix with walls. It iterates over all elements in the matrix, where n is the number of rows and m is the number of columns.

print_matrix() - O(n * m): This function prints the matrix. It iterates over all elements in the matrix to print them.

get_cords_for_exit() - O(1): This function generates random coordinates for the exit point. The complexity is constant as it only generates a single coordinate pair.

set_exits() - O(1): This function sets the start and exit points in the matrix. It calls get_cords_for_exit() twice and performs a constant number of operations.

BFS() - O(n + m): This function performs a breadth-first search to find a path from the start to a given point. In the worst case, it explores all cells in the matrix once.

set_key_point() - O(n * m): This function sets a key point in the maze ensuring it is surrounded by walls. It performs a constant number of operations for each cell in the matrix.

set_path() - O(n * m * √mn): This function generates the path in the maze. It uses BFS to find paths between key points. The BFS complexity is multiplied by the number of key points, which is approximately the square root of the total number of cells.

Overall, the solution is efficient for typical use cases, with the time complexity dominated by the BFS operations.
