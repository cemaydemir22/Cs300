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
    bool passed = false;
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

void generate_maze(int M, int N, int maze_id)
{
    STACK<for_maze_cells> stack_maze;
    vector<vector<for_maze_cells>> maze(M, vector<for_maze_cells>(N));

    auto now = std::chrono::high_resolution_clock::now();
    // Convert current time to a duration since epoch
    auto duration = now.time_since_epoch();
    // Convert duration to seconds
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    // Seed the random number generator with current seconds
    srand(seconds);

    int num = 0;
    maze.at(0).at(0).passed = true;
    stack_maze.push(maze.at(0).at(0));
    for (int j = 0; j < maze.size(); j++) {
        for (int k = 0; k < maze.at(0).size(); k++) {
            maze.at(j).at(k).row = j;
            maze.at(j).at(k).column = k;
        }
    }


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
                continue; // Exit the current iteration to restart the loop
            }
            else
            {
                // Backtrack if no wall can be knocked down
                stack_maze.pop();
            }
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



void fuction_path_finding(int X,int Y,int X_exit,int Y_exit)
{

   STACK<for_maze_cells> stack;
   vector<vector<for_maze_cells>> maze(M, vector<for_maze_cells>(N));
   stack.push(maze[Y][X]); // push the entry cell

    




}

int main()
{
    int K, M, N;
    int id;
    int X_entry,Y_entry;
    int X_exit,Y_exit;
    cout << "Enter the number of mazes: ";
    cin >> K;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N;
    cout << "All mazes are generated." << endl;
    cout<<endl<<endl;
    cout<<"Enter a maze ID between 1 to "<<K<<" inclusive to find a path: ";
    cin>>id;
    cout<<"Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin>>X_entry>>Y_entry;
    cout<<"Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin>>X_exit>>Y_exit;

    for (int i = 1; i <= K; i++)
    {
        generate_maze(M, N, i);
    }
 
    fuction_path_finding(X_entry,Y_entry,X_exit,Y_exit);
    return 0;
}
