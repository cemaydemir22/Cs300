#include <iostream>
#include "stack.h" // Template based class
#include <vector>
#include <string>
#include "stack.cpp"
// #include <ctime>    // For time 
// #include <cstdlib>  // For rand

using namespace std;
struct for_maze_cells 
{

	int upp_cell = 1; // Upper boundary
	int left_cell = 1; //  Left boundary
	int right_cell = 1; // Right boundary
	int down_cell = 1; //  Down boundary
	bool visit = false;
	int row; // for  coordinate of the cells
	int column; // for coordinate of the cells

	for_maze_cells(int x, int y) : column(x), row(y), upp_cell(1), down_cell(1), right_cell(1), left_cell(1), visit(false){}
    for_maze_cells(){  // default constructor
		int upp_cell = 1; // Upper boundary
		int left_cell = 1; //  Left boundary
		int right_cell = 1; // Right boundary
		int down_cell = 1; //  Down boundary
		bool visit = false;
		int row; // for  coordinate of the cells
		int column; // for coordinate of the cells

	}
};


bool Left_knock(vector<vector<for_maze_cells>> maze,STACK<for_maze_cells> & cell_stack)
{
    // if (/* condition */)
	// {
	// 	/* code */
	// }
	
}


bool Right_knock(vector<vector<for_maze_cells>> maze,STACK<for_maze_cells> & cell_stack)
{

}


bool Down_knock(vector<vector<for_maze_cells>> maze,STACK<for_maze_cells> & cell_stack)
{

}


bool Up_knock(vector<vector<for_maze_cells>> maze,STACK<for_maze_cells> & cell_stack)
{

}



int get_random(int min, int max) // To choose random wall 
{
    return (min + rand() % (max - min + 1)); // it will return a number between 1-4 (inclusive)
}

int get_random()
{
    return get_random(1, 4);
}


void random_choose_wall(STACK<for_maze_cells> &stack_maze ,for_maze_cells & cell)
{
    int num=get_random();

    switch(num)
    {

        case 1: // Upp wall

           if(Up_knock())
           {

           }

          
          
        
        case 2: // Left wall

         if(Left_knock())
           {
            
           }

        case 3:  // Right wall
           if(Right_knock())
           {
            
           }
         

        case 4: // Down wall
          if(Down_knock())
          {
            
           
          }

    }

}




int main() 
{
    STACK<for_maze_cells> stack_maze;
	vector<vector<vector<for_maze_cells>>> Mazes; // This 3-D vector is to hold multiplep mazes
	
	int N, K, M; // K is for the number of mazes

	cout << "Enter the number of mazes: ";
	cin >> K;
	cout << "Enter the number of rows and columns (M and N): ";
	cin >> M >> N;
	cout << "All mazes are generated."<<endl;

	vector<vector<for_maze_cells>> Maze(M,vector<for_maze_cells>(N));// This matrix will hold cells to form a maze (M=#rows N=#columns)
	stack_maze.push(Maze[0][0]);
    Maze[0][0].visit=true;


	for (int i = 1; i < K; i++) // This loop is to create K mazes
	{
		for (int j = 0; j < Maze.size(); j++)
		{
			for (int k = 0; k < Maze[0].size(); k++)
			{
				Maze[j][k].row=j;
				Maze[j][k].column=k; // Maze içerisindeki  hücrelere konum yazıldı
			}
			
		}
		
        while (!stack_maze.Is_empty())
        {

            for_maze_cells current_cell=stack_maze.current_node();

            
        }
        
		
	}
	return 0;
}

