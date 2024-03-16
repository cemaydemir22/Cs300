#include <iostream>
#include <fstream>
#include <ctime> // for generating random numbers
#include "stack.h" // Template based class
#include <vector>
#include <string>
#include "stack.cpp"
#include <cstdlib> // To generate random number
#include <ctime>
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

// Overloaded function to generate a random number between 1 and 4 (inclusive)
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
        if (cell_stack.current_node().column == 0)
        {
            return false;
        }
        else if (cell_stack.current_node().column >= 1 && !maze[cell_stack.current_node().row][cell_stack.current_node().column - 1].passed) // vector 0 için -1 oluyor
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
        if (cell_stack.current_node().column >= maze[0].size() - 1)
        {
            return false;
        }
        else if (cell_stack.current_node().column <= maze[0].size() - 1 && !maze[cell_stack.current_node().row][cell_stack.current_node().column + 1].passed)
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
        if (cell_stack.current_node().row == 0)
        {
            return false;
        }
        else if (cell_stack.current_node().row >= 1 && !maze[cell_stack.current_node().row - 1][cell_stack.current_node().column].passed)
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
        if (cell_stack.current_node().row >= maze.size() - 1) {

        }
        else  if (cell_stack.current_node().row <= maze.size() - 1 && !maze[cell_stack.current_node().row + 1][cell_stack.current_node().column].passed)
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

vector<vector<for_maze_cells>> generate_maze(STACK<for_maze_cells> stack_maze, vector<vector<for_maze_cells>> maze)
{
    int num = 0;
    while (!stack_maze.Is_empty()) // Loop until stack is empty
    {
        num = get_random();
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
            if (Check_Other_Cells(maze, stack_maze))
            {
                continue; // Restart loop
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


bool Check_Other_Cells(vector<vector<for_maze_cells>> maze, STACK<for_maze_cells> stack_maze)
{
    if (knock_walls(maze, stack_maze, 1) || knock_walls(maze, stack_maze, 2) || knock_walls(maze, stack_maze, 3) || knock_walls(maze, stack_maze, 4))
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
    maze.at(Y).at(X).passed = true;
    while (!(stack.current_node().row == Y_exit && stack.current_node().column == X_exit)) // Continue until the cell is equal to the exit cell
    {
        if (maze[stack.current_node().row][stack.current_node().column].left_cell == 0 && !maze[stack.current_node().row][stack.current_node().column - 1].passed2) // Check if there is no left wall and the left cell has not been passed
        {
            stack.push(maze[stack.current_node().row][stack.current_node().column - 1]); // Move to the left cell
            maze[stack.current_node().row][stack.current_node().column].passed2 = true; // Mark the left cell as passed
        }
        else if (maze[stack.current_node().row][stack.current_node().column].right_cell == 0 && !maze[stack.current_node().row][stack.current_node().column + 1].passed2) // Check if there is no right wall and the right cell has not been passed
        {
            stack.push(maze[stack.current_node().row][stack.current_node().column + 1]); // Move to the right cell
            maze[stack.current_node().row][stack.current_node().column].passed2 = true; // Mark the right cell as passed
        }
        else if (maze[stack.current_node().row][stack.current_node().column].down_cell == 0 && !maze[stack.current_node().row - 1][stack.current_node().column].passed2) // Check if there is no down wall and the down cell has not been passed
        {
            stack.push(maze[stack.current_node().row - 1][stack.current_node().column]); // Move to the down cell
            maze[stack.current_node().row][stack.current_node().column].passed2 = true; // Mark the down cell as passed
        }
        else if (maze[stack.current_node().row][stack.current_node().column].upp_cell == 0 && !maze[stack.current_node().row + 1][stack.current_node().column].passed2) // Check if there is no up wall and the up cell has not been passed
        {
            stack.push(maze[stack.current_node().row + 1][stack.current_node().column]); // Move to the up cell
            maze[stack.current_node().row][stack.current_node().column].passed2 = true; // Mark the up cell as passed
        }
        else
        {
            stack.pop(); // If no valid direction is found, backtrack
        }
    }
    return stack;
}

void write_path_to_file(STACK<for_maze_cells> stack, int mazeID, int entryX, int entryY, int exitX, int exitY)
{
    ofstream outfile("maze_" + to_string(mazeID) + "_path_" + to_string(entryX) + "_" + to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt");

    if (outfile.is_open())
    {
        while (!stack.Is_empty())
        {
            outfile << stack.current_node().column << " " << stack.current_node().row << endl;
            stack.pop();
        }
        outfile.close();
        cout << "Path written to file successfully." << endl;
    }
    else
    {
        cout << "Unable to open file for writing." << endl;
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
    cout << endl << endl;
    cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path: ";
    cin >> id;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> X_entry >> Y_entry;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> X_exit >> Y_exit;

    vector<vector<vector<for_maze_cells>>> mazes;
    STACK<for_maze_cells> stack_solve;

    // Seed the random number generator
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    srand(seconds);

    for (int i = 1; i <= K; i++)
    {
        STACK<for_maze_cells> stack_maze;
        vector<vector<for_maze_cells>> maze(M, vector<for_maze_cells>(N));
        for (int j = 0; j < maze.size(); j++) {
            for (int k = 0; k < maze.at(0).size(); k++) {
                maze.at(j).at(k).row = j;
                maze.at(j).at(k).column = k;
            }
        }
        stack_maze.push(maze.at(0).at(0));
        maze.at(0).at(0).passed = true;
        mazes.push_back(generate_maze(stack_maze, maze));
    }

    for (int i = 0; i < K; i++)
    {
        ofstream outfile("maze_" + to_string(i + 1) + ".txt"); // Maze IDs start from 1
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
    write_path_to_file(function_path_finding(X_entry, Y_entry, X_exit, Y_exit, mazes[id - 1], stack_solve), id, X_entry, Y_entry, X_exit, Y_exit);
    return 0;
}
