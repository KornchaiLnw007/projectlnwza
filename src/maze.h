#include <vector>
#include <stack>
#include <random>

using namespace std;

class Maze {
public:
    vector<vector<int>> maze;
    int rows;
    int cols;
    Maze(int rows, int cols);
    void generateMaze();
    bool isWall(int i, int j);
};

const int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // Directions for maze generation

// Constructor: Initializes the maze and generates it
Maze::Maze(int rows, int cols) : rows(rows), cols(cols) {
    maze = vector<vector<int>>(rows, vector<int>(cols, 1)); // Initialize maze with walls
    generateMaze(); // Call generateMaze to fill in the path
}

// Maze generation algorithm (recursive backtracking)
void Maze::generateMaze() {
    stack<pair<int, int>> st;
    st.push({1, 1}); // Start from (1,1)
    maze[1][1] = 0;  // Starting point is a path

    random_device rd;
    mt19937 rng(rd()); // Random number generator

    while (!st.empty()) {
        pair<int, int> current = st.top();
        st.pop();

        vector<int> dirs = {0, 1, 2, 3}; // Directions for possible moves
        shuffle(dirs.begin(), dirs.end(), rng); // Shuffle directions randomly

        for (int dir : dirs) {
            int ni = current.first + directions[dir][0] * 2; // New row
            int nj = current.second + directions[dir][1] * 2; // New column

            // Ensure the new cell is within bounds and is a wall
            if (ni >= 1 && ni < rows && nj >= 1 && nj < cols && maze[ni][nj] == 1) {
                maze[ni][nj] = 0; // Make the new cell a path
                maze[current.first + directions[dir][0]][current.second + directions[dir][1]] = 0; // Remove wall between
                st.push({ni, nj}); // Push new cell onto stack
            }
        }
    }
}

// Method to check if a specific cell is a wall
bool Maze::isWall(int i, int j) {
    return maze[i][j] == 1;
}
