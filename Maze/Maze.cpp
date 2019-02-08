// Cory Press
// Maze
// Creates a Maze of size n by first cutting the maze using a depth
//	first search than drawing it by drawing each wall of each cell


#include<vector>
#include<iostream>
#include<random>
#include<algorithm>
#include<time.h>
#include "Angel.h"
#pragma comment( lib, "glew32" )



using namespace std;

const int n = 11;
vector<vec2> points;

//random function
int custrand(int r) {
	return rand() % r;
}

//struct that store information for a single
// block in a maze
struct cell {
	bool bot;//signal if wall under
	bool right;//signals if wall to the right
	bool visited;//signal if block has been visited by
				 //  cutting algorithm
	cell() {//default constructor
		bot = right = true;
		visited = false;
	}

};

//Recursive Function that goes through randomly and cuts a
// path into a maze one block at a time
void createMaze(int r, int c, vector<vector<cell>> &maze) {
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

//----------------------------------------------------------------------------

void
init(void)
{
	points.reserve(n*n*2*2);

	srand(time(NULL));

	vector<vector<cell>> maze(n + 2, vector<cell>(n + 2));
	//set outside of bounds blocks to display properly
	// and set them to visited to eliminate having to
	// check bounds repeatidly
	for (int r = 0; r < n + 2; r++) {
		maze[r][0].bot = false;
		maze[r][0].visited = true;
		maze[r][n + 1].bot = false;
		maze[r][n + 1].right = false;
		maze[r][n + 1].visited = true;
	}
	for (int c = 0; c < n + 2; c++) {
		maze[0][c].right = false;
		maze[0][c].visited = true;
		maze[n + 1][c].bot = false;
		maze[n + 1][c].right = false;
		maze[n + 1][c].visited = true;
	}

	//set random start position of right side and
	// end on the left
	int start = rand() % n + 1;
	maze[start][0].right = false;
	int end = rand() % n + 1;
	maze[end][n].right = false;

	//cut maze starting at the start
	createMaze(start, 1, maze);


	double seg = 1.9 / (n+2);//length of side of cell
	double offset = .95;//how far maze extends from the center
	//go through each cell and if there is a wall create two points for each corner of the wall
	for (int r = 0; r < n + 2; r++) {
		for (int c = 0; c < n + 2; c++) {
			if (maze[r][c].bot == true) {
				points.push_back(vec2((c*seg) - offset, (r+1)*seg - offset));
				points.push_back(vec2((c + 1)*seg - offset, (r+1)*seg - offset));
			}

			if (maze[r][c].right == true) {
				points.push_back(vec2((c + 1)*seg - offset, (r+1)*seg - offset));
				points.push_back(vec2((c + 1)*seg - offset, r*seg - offset));
			}
		}
	}
	

	
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(vec2), &points[0], GL_STATIC_DRAW);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader_a2.glsl", "fshader_a2.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 0.0); // white background
}

//----------------------------------------------------------------------------

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);     // clear the window
	glDrawArrays(GL_LINES, 0, points.size());    // draw the points
	glFlush();
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(780, 780);
	// If you are using freeglut, the next two lines will check if 
	// the code is truly 3.2. Otherwise, comment them out
	glutInitContextVersion(3, 1);// Does not need glewExperimental = GL_TRUE
	// 3,2 does.  Discovered by Dr. C
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Maze");
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	;
	glutMainLoop();
	return 0;
}
