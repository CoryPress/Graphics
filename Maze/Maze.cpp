#include<vector>
#include<iostream>
#include<random>
#include<algorithm>
#include<time.h>

using namespace std;

const int n = 50;
const int m = 50;

int custrand(int r) {
	return rand() % r;
}

//struct that store information for a single
// block in a maze
struct block {
	bool bot;//signal if wall under
	bool right;//signals if wall to the right
	bool visited;//signal if block has been visited by
				 //  cutting algorithm
	block() {//default constructor
		bot = right = true;
		visited = false;
	}

};

//Recursive Function that goes through randomly and cuts a
// path into a maze one block at a time
void createMaze(int r, int c, vector<vector<block>> &maze) {
	maze[r][c].visited = true;

	vector<char> order(0);
	order.reserve(4);
	//check neighbors to see whose visited
	if (maze[r][c + 1].visited == false)
		order.push_back('r');//right
	if (maze[r + 1][c].visited == false)
		order.push_back('b');//bottom
	if (maze[r][c - 1].visited == false)
		order.push_back('l');//left
	if (maze[r - 1][c].visited == false)
		order.push_back('t');//top

	if (order.size() > 0) {
		//randomly shuffle order there visited in
		random_shuffle(order.begin(), order.end(), custrand);

		//loop through a visited all unvisited nieghbors
		for (int i = 0; i < order.size(); i++) {
			switch (order[i]) {
			case 'r'://right
				//check to make sure wasn't visited by another path
				if (maze[r][c + 1].visited == false) {
					maze[r][c].right = false;//cut path
					createMaze(r, c + 1, maze);//go to next block
				}
				break;
			case 'b'://bottom
				if (maze[r + 1][c].visited == false) {
					maze[r][c].bot = false;
					createMaze(r + 1, c, maze);
				}
				break;
			case 'l'://left
				if (maze[r][c - 1].visited == false) {
					maze[r][c - 1].right = false;
					createMaze(r, c - 1, maze);
				}
				break;
			case 't'://top
				if (maze[r - 1][c].visited == false) {
					maze[r - 1][c].bot = false;
					createMaze(r - 1, c, maze);
				}
				break;
			}
		}


	}

	return;
}

int main()
{
	srand(time(NULL));

	vector<vector<block>> maze(n+2, vector<block>(m+2));
	//set outside of bounds blocks to display properly
	// and set them to visited to eliminate having to
	// check bounds repeatidly
	for (int r = 0; r < n + 2; r++) {
		maze[r][0].bot = false;
		maze[r][0].visited = true;
		maze[r][m + 1].bot = false;
		maze[r][m + 1].right = false;
		maze[r][m + 1].visited = true;
	}
	for (int c = 0; c < m + 2; c++) {
		maze[0][c].right = false;
		maze[0][c].visited = true;
		maze[n+1][c].bot = false;
		maze[n+1][c].right = false;
		maze[n + 1][c].visited = true;
	}

	//set random start position of right side and
	// end on the left
	int start = rand() % n + 1;
	maze[start][0].right = false;
	int end = rand() % n + 1;
	maze[end][m].right = false;

	//cut maze starting at the start
	createMaze(start, 1, maze);

	
	for (int r = 0; r < n + 2; r++) {
		for (int c = 0; c < m + 2; c++) {
			if (maze[r][c].bot == true) {
				if (maze[r][c].right == true) {
					char c = (217);
					cout << c;
				}
				else
					cout << '_';
			}
			else {
				if (maze[r][c].right == true)
					cout << '|';
				else
					cout << ' ';
			}
		}
		cout << "\n";
	}



    return 0;
}

