#include <iostream>
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


int get_random(int min, int max) // To choose random wall 
{
    return (min + rand() % (max - min + 1)); // it will return a number between 1-4 (inclusive) (+1 is to include upper range)
}

int get_random()
{
    return get_random(1, 4);
}



bool Knock_functions(vector<vector<for_maze_cells>> & maze, STACK<for_maze_cells> & cell_stack ,int num) // This function will check whether the wall can be knocked down or not
{
   
   switch(num)
   {
	  case 1:  // Left_knock

		if (cell_stack.current_node().column>=1 && !maze[cell_stack.current_node().row][cell_stack.current_node().column-1].passed) // check column and the left cell should not be passed
		{
				maze[cell_stack.current_node().row][cell_stack.current_node().column].left_cell=0;
				maze[cell_stack.current_node().row][cell_stack.current_node().column-1].right_cell=0;  // Knock down cell from both sides
				maze[cell_stack.current_node().row][cell_stack.current_node().column-1].passed=true;
				return true;
		}
		else
		{
				return false;
		}

	  case 2:   // Right knock

		if (cell_stack.current_node().column<=maze[0].size()-2 && !maze[cell_stack.current_node().row][cell_stack.current_node().column+1].passed )
		{
				maze[cell_stack.current_node().row][cell_stack.current_node().column].right_cell=0;
				maze[cell_stack.current_node().row][cell_stack.current_node().column+1].left_cell=0;  // Knock down cell from both sides
				maze[cell_stack.current_node().row][cell_stack.current_node().column-1].passed=true;
				return true;
		}
		else
		{
				return false;
		}

	  case 3:   // Down knock

		if(cell_stack.current_node().row>=1 && !maze[cell_stack.current_node().row-1][cell_stack.current_node().column].passed )
		{
			    maze[cell_stack.current_node().row][cell_stack.current_node().column].down_celll=0;
				maze[cell_stack.current_node().row-1][cell_stack.current_node().column].upp_cell=0;  // Knock down cell from both sides
				maze[cell_stack.current_node().row][cell_stack.current_node().column].passed=true;
				return true;
		}	
		else
		{
				return false;
		}

	  case 4:  // Up knock

		if (cell_stack.current_node().row<=maze.size()-2 && !maze[cell_stack.current_node().row+1][cell_stack.current_node().column].passed ) 
		{
				maze[cell_stack.current_node().row][cell_stack.current_node().column].upp_cell=0;
				maze[cell_stack.current_node().row+1][cell_stack.current_node().column].down_cell=0;  // Knock down cell from both sides
				maze[cell_stack.current_node().row][cell_stack.current_node().column].passed=true;
				return true;
		}
		else
		{
				return false;
		}
   }
}




void random_choose_wall(STACK<for_maze_cells> &stack_maze ,vector<vector<for_maze_cells>> & mazematrix)
{
    int num=get_random();



	if( Knock_functions(mazematrix,stack_maze, num) ) // If true then ....
	{

		switch(num)
		{
           case 1: // Left

		      stack_maze.push(mazematrix[][]);


			  break;



		   case 2: // Right

              stack_maze.push(mazematrix[][]);

			  break;

		   case 3: //  Down


              stack_maze.push(mazematrix[][]);

			  break;


           case 4: // Up

		      stack_maze.push(mazematrix[][]);



		      break;
		}
		

		

	}

    
    

}




int main() 
{
    STACK<for_maze_cells> stack_maze;
	vector<vector<vector<for_maze_cells>>> Mazes; // This 3-D vector is to hold multiple mazes
	
	int N, K, M; // K is for the number of mazes

	cout << "Enter the number of mazes: ";
	cin >> K;
	cout << "Enter the number of rows and columns (M and N): ";
	cin >> M >> N;
	cout << "All mazes are generated."<<endl;

	vector<vector<for_maze_cells>> Maze(M,vector<for_maze_cells>(N));// This matrix will hold cells to form a maze (M=#rows N=#columns)
	stack_maze.push(Maze[0][0]);
    Maze[0][0].passed=true;


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

            

            
        }
        
		
	}
	return 0;
}

