//Cory Press

#include<vector>
#include<iostream>
#include<random>
#include<algorithm>
#include <time.h>

using namespace std;

const int n = 50;
const int m = 50;

int custrand(int r) {
	return rand() % r;
}

struct block {
	bool bot;
	bool right;
	bool visited;
	block() {
		bot = right = true;
		visited = false;
	}

};

void createMaze(int r, int c, vector<vector<block>> &maze) {
	maze[r][c].visited = true;

	vector<char> order(0);
	order.reserve(4);
	if (maze[r][c + 1].visited == false)
		order.push_back('r');
	if (maze[r + 1][c].visited == false)
		order.push_back('b');
	if (maze[r][c - 1].visited == false)
		order.push_back('l');
	if (maze[r - 1][c].visited == false)
		order.push_back('t');

	if(order.size() > 0)
		random_shuffle(order.begin(), order.end(), custrand);

	for (int i = 0; i < order.size(); i++){
		switch (order[i]) {
		case 'r':
			if (maze[r][c + 1].visited == false) {
				maze[r][c].right = false;
				createMaze(r, c + 1, maze);
			}
			break;
		case 'b':
			if (maze[r + 1][c].visited == false) {
				maze[r][c].bot = false;
				createMaze(r + 1, c, maze);
			}
			break;
		case 'l':
			if (maze[r][c - 1].visited == false) {
				maze[r][c - 1].right = false;
				createMaze(r, c - 1, maze);
			}
			break;
		case 't':
			if (maze[r - 1][c].visited == false) {
				maze[r - 1][c].bot = false;
				createMaze(r - 1, c, maze);
			}
			break;
		}
	}

	return;
}

int main()
{
	srand(time(NULL));


	vector<vector<block>> maze(n+2, vector<block>(m+2));
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

	int start = rand() % n + 1;
	maze[start][0].right = false;
	int end = rand() % n + 1;
	maze[end][m].right = false;

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

