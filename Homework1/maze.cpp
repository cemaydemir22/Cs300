#include <iostream>
#include <fstream>
#include <ctime> // for generating random numbers
#include "stack.h" // Template based class
#include <vector>
#include <string>
#include "stack.cpp"
#include <cstdlib> // To generate random number
#include <chrono> 


using namespace std;
struct for_maze_cells
{
    int upp_cell = 1; // Upper boundary
    int left_cell = 1; //  Left boundary
    int right_cell = 1; // Right boundary
    int down_cell = 1; //  Down boundary
    bool passed = false; // To track in maze generation part 
    bool passed2 = false; // To track in path finding part 
    int row; // for  coordinate of the cells
    int column; // for coordinate of the cells 
};

bool Check_Other_Cells(vector<vector<for_maze_cells>>, STACK<for_maze_cells>);
// Function to generate a random number between min and max (inclusive)

int get_random(int min, int max)
{
    return (min + rand() % (max - min + 1));
}

//  function to generate a random number between 1 and 4 (inclusive)
int get_random()
{
    return get_random(1, 4);
}

// Function to knock down walls between cells
bool knock_walls(vector<vector<for_maze_cells>>& maze, STACK<for_maze_cells>& cell_stack, int num)
{
    switch (num)
    {
    case 1: // Left
        if (cell_stack.top().column == 0) // To avoid vector out of range error
        {
            return false;
        }
        else if (cell_stack.top().column >= 1 && !maze[cell_stack.top().row][cell_stack.top().column - 1].passed) 
        {
            maze[cell_stack.top().row][cell_stack.top().column].left_cell = 0; // break the wall 
            maze[cell_stack.top().row][cell_stack.top().column - 1].right_cell = 0; // break the wall 
            maze[cell_stack.top().row][cell_stack.top().column - 1].passed = true; // make it passed
            return true;
        }
        else
        {
            return false;
        }

    case 2: // Right
        if (cell_stack.top().column >= maze[0].size() - 1)
        {
            return false;
        }
        else if (cell_stack.top().column <= maze[0].size() - 1 && !maze[cell_stack.top().row][cell_stack.top().column + 1].passed)
        {
            maze[cell_stack.top().row][cell_stack.top().column].right_cell = 0;
            maze[cell_stack.top().row][cell_stack.top().column + 1].left_cell = 0;
            maze[cell_stack.top().row][cell_stack.top().column + 1].passed = true;
            return true;
        }
        else
        {
            return false;
        }

    case 3: // Down
        if (cell_stack.top().row == 0)
        {
            return false;
        }
        else if (cell_stack.top().row >= 1 && !maze[cell_stack.top().row - 1][cell_stack.top().column].passed)
        {
            maze[cell_stack.top().row][cell_stack.top().column].down_cell = 0;
            maze[cell_stack.top().row - 1][cell_stack.top().column].upp_cell = 0;
            maze[cell_stack.top().row - 1][cell_stack.top().column].passed = true;
            return true;
        }
        else
        {
            return false;
        }

    case 4: // Up
        if (cell_stack.top().row >= maze.size() - 1) {

        }
        else  if (cell_stack.top().row <= maze.size() - 1 && !maze[cell_stack.top().row + 1][cell_stack.top().column].passed)
        {
            maze[cell_stack.top().row][cell_stack.top().column].upp_cell = 0;
            maze[cell_stack.top().row + 1][cell_stack.top().column].down_cell = 0;
            maze[cell_stack.top().row + 1][cell_stack.top().column].passed = true;
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

vector<vector<for_maze_cells>> generate_maze(STACK<for_maze_cells> stack_maze, vector<vector<for_maze_cells>> maze) // it returns the generated maze
{
    int num = 0;
    while (!stack_maze.isEmpty()) // Loop until stack is empty
    {
        num = get_random();
        if (knock_walls(maze, stack_maze, num))
        {
            // Add the new cell to the stack
            switch (num)
            {
            case 1: // Left
                stack_maze.push(maze[stack_maze.top().row][stack_maze.top().column - 1]);
                break;
            case 2: // Right
                stack_maze.push(maze[stack_maze.top().row][stack_maze.top().column + 1]);
                break;
            case 3: // Down
                stack_maze.push(maze[stack_maze.top().row - 1][stack_maze.top().column]);
                break;
            case 4: // Up
                stack_maze.push(maze[stack_maze.top().row + 1][stack_maze.top().column]);
                break;
            }
        }
        else
        {
            if (Check_Other_Cells(maze, stack_maze))
            {
                continue; // If other possible directions exists restart
            }
            else
            {
                // if no wall found backtrack
                stack_maze.pop();
            }
        }
    }
    return  maze;
}


bool Check_Other_Cells(vector<vector<for_maze_cells>> maze, STACK<for_maze_cells> stack_maze) // This function checks whether there are other possible directions to go or not
{
    if (knock_walls(maze, stack_maze, 1) || knock_walls(maze, stack_maze, 2) || knock_walls(maze, stack_maze, 3) || knock_walls(maze, stack_maze, 4)) // call knock_walls to check 4 possible directions
    {
        return true;
    }
    else
    {
        return false;
    }
}



STACK<for_maze_cells> function_path_finding(int X, int Y, int X_exit, int Y_exit, vector<vector<for_maze_cells>>& maze, STACK<for_maze_cells> & stack)
{
    stack.push(maze[Y][X]); // Push the entry cell
    maze.at(Y).at(X).passed2 = true; // Makes it passed
    while (!(stack.top().row == Y_exit && stack.top().column == X_exit)) // Continue until the cell is equal to the exit cell
    {
        if (maze[stack.top().row][stack.top().column].left_cell == 0 && !maze[stack.top().row][stack.top().column - 1].passed2) // Check the left wall
        {
            stack.push(maze[stack.top().row][stack.top().column - 1]); 
            maze[stack.top().row][stack.top().column].passed2 = true; 
        }
        else if (maze[stack.top().row][stack.top().column].right_cell == 0 && !maze[stack.top().row][stack.top().column + 1].passed2) // Check the right wall
        {
            stack.push(maze[stack.top().row][stack.top().column + 1]); 
            maze[stack.top().row][stack.top().column].passed2 = true;
        }
        else if (maze[stack.top().row][stack.top().column].down_cell == 0 && !maze[stack.top().row - 1][stack.top().column].passed2) // Check the down wall
        {
            stack.push(maze[stack.top().row - 1][stack.top().column]); 
            maze[stack.top().row][stack.top().column].passed2 = true; 
        }
        else if (maze[stack.top().row][stack.top().column].upp_cell == 0 && !maze[stack.top().row + 1][stack.top().column].passed2) // Check the up wall
        {
            stack.push(maze[stack.top().row + 1][stack.top().column]); 
            maze[stack.top().row][stack.top().column].passed2 = true; 
        }
        else
        {
            stack.pop(); // If no valid direction found backtrack
        }
    }
    return stack;
}

void print_stack(STACK<for_maze_cells> stack, int mazeID, int entryX, int entryY, int exitX, int exitY) // this function writes the path from entry point to exit point
{
    STACK<for_maze_cells> temp_stack;
    while (!stack.isEmpty()) // to reverse the stack
    {
        temp_stack.push(stack.top());
        stack.pop(); 
    }

    ofstream outfile("maze_" + to_string(mazeID) + "_path_" + to_string(entryX) + "_" + to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt");

    if (outfile.is_open())
    {
        while (!temp_stack.isEmpty())
        {
            outfile << temp_stack.top().column << " " << temp_stack.top().row << endl;
            temp_stack.pop();
        }
        outfile.close();
    }
   
}

int main()
{
    int K, M, N;
    int id;
    int X_entry, Y_entry;
    int X_exit, Y_exit;

    cout << "Enter the number of mazes: ";
    cin >> K;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N;
    cout << "All mazes are generated." << endl;
    cout << endl;
    cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path: ";
    cin >> id;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> X_entry >> Y_entry;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> X_exit >> Y_exit;

    vector<vector<vector<for_maze_cells>>> mazes;
    STACK<for_maze_cells> stack_solve;

    // For random generator
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    srand(seconds);

    for (int i = 1; i <= K; i++) // K mazes
    {
        STACK<for_maze_cells> stack_maze;
        vector<vector<for_maze_cells>> maze(M, vector<for_maze_cells>(N));
        for (int j = 0; j < maze.size(); j++) 
        {
            for (int k = 0; k < maze.at(0).size(); k++) 
            {
                maze.at(j).at(k).row = j;
                maze.at(j).at(k).column = k;
            }
        }
        stack_maze.push(maze.at(0).at(0));
        maze.at(0).at(0).passed = true;
        mazes.push_back(generate_maze(stack_maze, maze)); // Push mazes from 1 to K
    }

    for (int i = 0; i < K; i++) // print the generated mazes
    {
        ofstream outfile("maze_" + to_string(i + 1) + ".txt"); 
        if (outfile.is_open())
        {
            outfile << M << " " << N << endl;
            for (int j = 0; j < M; j++)
            {
                for (int k = 0; k < N; k++)
                {
                    outfile << "x=" << k << " y=" << j << " l=" << mazes[i][j][k].left_cell << " r=" << mazes[i][j][k].right_cell << " u=" << mazes[i][j][k].upp_cell << " d=" << mazes[i][j][k].down_cell << endl;
                }
            }
            outfile.close();
        }
    }
    print_stack(function_path_finding(X_entry, Y_entry, X_exit, Y_exit, mazes[id - 1], stack_solve), id, X_entry, Y_entry, X_exit, Y_exit);
    return 0;
}
