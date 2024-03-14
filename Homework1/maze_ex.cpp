#include <iostream>
#include <fstream>
#include <ctime> // for generating random numbers
#include "stack.h" // Template based class
#include <vector>
#include <string>
#include "stack.cpp"

using namespace std;

struct for_maze_cells
{
    int upp_cell = 1; // Upper boundary
    int left_cell = 1; //  Left boundary
    int right_cell = 1; // Right boundary
    int down_cell = 1; //  Down boundary
    bool passed = false;
    int row; // for  coordinate of the cells
    int column; // for coordinate of the cells
};

// Function to generate a random number between min and max (inclusive)
int get_random(int min, int max)
{
    return (min + rand() % (max - min + 1));
}

// Overloaded function to generate a random number between 1 and 4 (inclusive)
int get_random()
{
    return get_random(1, 4);
}

// Function to knock down walls between cells
bool knock_walls(vector<vector<for_maze_cells>>& maze, STACK<for_maze_cells>& cell_stack,int num)
{
    

    switch (num)
    {
    case 1: // Left
        if (cell_stack.current_node().column >= 1 && !maze[cell_stack.current_node().row][cell_stack.current_node().column - 1].passed)
        {
            maze[cell_stack.current_node().row][cell_stack.current_node().column].left_cell = 0;
            maze[cell_stack.current_node().row][cell_stack.current_node().column - 1].right_cell = 0;
            maze[cell_stack.current_node().row][cell_stack.current_node().column - 1].passed = true;
            return true;
        }
        else
        {
            return false;
        }

    case 2: // Right
        if (cell_stack.current_node().column <= maze[0].size() - 1 && !maze[cell_stack.current_node().row][cell_stack.current_node().column + 1].passed)
        {
            maze[cell_stack.current_node().row][cell_stack.current_node().column].right_cell = 0;
            maze[cell_stack.current_node().row][cell_stack.current_node().column + 1].left_cell = 0;
            maze[cell_stack.current_node().row][cell_stack.current_node().column + 1].passed = true;
            return true;
        }
        else
        {
            return false;
        }

    case 3: // Down
        if (cell_stack.current_node().row >= 1 && !maze[cell_stack.current_node().row - 1][cell_stack.current_node().column].passed)
        {
            maze[cell_stack.current_node().row][cell_stack.current_node().column].down_cell = 0;
            maze[cell_stack.current_node().row - 1][cell_stack.current_node().column].upp_cell = 0;
            maze[cell_stack.current_node().row - 1][cell_stack.current_node().column].passed = true;
            return true;
        }
        else
        {
            return false;
        }

    case 4: // Up
        if (cell_stack.current_node().row <= maze.size() - 1 && !maze[cell_stack.current_node().row + 1][cell_stack.current_node().column].passed)
        {
            maze[cell_stack.current_node().row][cell_stack.current_node().column].upp_cell = 0;
            maze[cell_stack.current_node().row + 1][cell_stack.current_node().column].down_cell = 0;
            maze[cell_stack.current_node().row + 1][cell_stack.current_node().column].passed = true;
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

// Function to generate maze
void generate_maze(int M, int N, int maze_id)
{
    STACK<for_maze_cells> stack_maze;
    vector<vector<for_maze_cells>> maze(M, vector<for_maze_cells>(N));

    // Set the seed for random number generation
    int num=get_random;

    stack_maze.push(maze[0][0]);
    maze[0][0].passed = true;

    // Maze generation loop
    while (!stack_maze.Is_empty())
    {
        if (knock_walls(maze, stack_maze, num))
        {
            // Add the new cell to the stack
            switch (num)
            {
            case 1: // Left
                stack_maze.push(maze[stack_maze.current_node().row][stack_maze.current_node().column - 1]);
                break;
            case 2: // Right
                stack_maze.push(maze[stack_maze.current_node().row][stack_maze.current_node().column + 1]);
                break;
            case 3: // Down
                stack_maze.push(maze[stack_maze.current_node().row - 1][stack_maze.current_node().column]);
                break;
            case 4: // Up
                stack_maze.push(maze[stack_maze.current_node().row + 1][stack_maze.current_node().column]);
                break;
            }
        }
        else
        {
            // Backtrack if no wall can be knocked down
            stack_maze.pop();
        }
    }

    // Write the maze to a file
    ofstream outfile("maze_" + to_string(maze_id) + ".txt");
    if (outfile.is_open())
    {
        outfile << M << " " << N << endl;
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                outfile << "x=" << j << " y=" << i << " l=" << maze[i][j].left_cell << " r=" << maze[i][j].right_cell << " u=" << maze[i][j].upp_cell << " d=" << maze[i][j].down_cell << endl;
            }
        }
        outfile.close();
    }
   
}

int main()
{
    int K, M, N;

    cout << "Enter the number of mazes: ";
    cin >> K;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N;
    cout << "All mazes are generated." << endl;

    for (int i = 1; i <= K; i++)
    {
        generate_maze(M, N, i);
    }

    return 0;
}
