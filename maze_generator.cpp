#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <queue>

#define WALL '#'
#define START 'S'
#define EXIT 'E'
#define PATH '.'
#define TREASURE '@'

struct cords{
    cords(int x = 0, int y = 0) : x(x), y(y) {}
    int x, y;
};

enum Sides { UP, RIGHT, DOWN, LEFT, SIDES_AMOUNT };

using Matrix = std::vector<std::vector<char>>;

// ================================================================================================================== //

// O(1)
void read_size(int *rows, int *cols){
    std::cin >> *rows >> *cols;
    if (*rows < 1 || *cols < 1 || (*rows == 1 && *cols == 1)){
        std::cout << "It`s impossible to build the labyrinth this size. The size of the labyrinth must consists of positive numbers and give an area greater then 1"
                    << std:: endl;
        exit(0);
    }
}

// O(n * m)
Matrix matrix_initialization(int xl, int yl){
    return Matrix(yl, std::vector<char>(xl, WALL));
}

// O(n * m)
void print_matrix(Matrix& matrix){
    for (auto& row:matrix) {
        for (auto cell: row)
            std::cout << cell << ' ';
        std::cout << std::endl;
    }
}

// O(1)
cords get_cords_for_exit(int rows, int cols, std::mt19937 &gen){
    int side = gen() % SIDES_AMOUNT;
    cords result;

    switch (side) {
        case UP:
            result.x = gen() % cols;
            return result;
        case RIGHT:
            result.y = gen() % rows;
            result.x = cols - 1;
            return result;
        case DOWN:
            result.y = rows - 1;
            result.x = gen() % cols;
            return result;
        case LEFT:
            result.y = gen() % rows;
            return result;
    }
    return result;
}

// O(1)
void set_exits(Matrix &matrix, cords *start, cords *end){
    std::random_device rd;
    std::mt19937 gen(rd());

    *start = get_cords_for_exit(matrix.size(), matrix[0].size(), gen);
    end->x = start->x;
    end->y = start->y;

    // making sure that start and end have different cords
    while(start->x == end->x && start->y == end->y)
        *end = get_cords_for_exit(matrix.size(), matrix[0].size(), gen);

    matrix[start->y][start->x] = START;
    matrix[end->y][end->x] = EXIT;
}

// O(n + m)
std::vector<cords> BFS (cords start, cords end){
    // relative displacements of directions for moving
    const int dirX[] = {0, 1, 0, -1};
    const int dirY[] = {-1, 0, 1, 0};

    // queue for BFS
    std::queue<cords> q;
    q.push(start);

    // parent map to reconstruct the path
    std::vector<std::vector<cords>> parent(101, std::vector<cords>(101, cords(-1, -1)));
    parent[start.x][start.y] = start;

    // BFS
    while (!q.empty()) {
        cords current = q.front();
        q.pop();

        // if we reach the goal, reconstruct and return the path
        if (current.x == end.x && current.y == end.y) {
            std::vector<cords> path;
            cords p = end;
            while (!(p.x == start.x && p.y == start.y)) {
                path.push_back(p);
                p = parent[p.x][p.y];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        // neighbors
        for (int d = 0; d < SIDES_AMOUNT; ++d) {
            int newX = current.x + dirX[d];
            int newY = current.y + dirY[d];

            // check bounds
            if (newX >= 0 && newX <= 100 && newY >= 0 && newY <= 100 && parent[newX][newY].x == -1) {
                parent[newX][newY] = current;
                q.push(cords(newX, newY));
            }
        }
    }

    // if no path found, return an empty path
    return std::vector<cords>();
}

// O(n * m)
cords set_key_point(Matrix &matrix, std::mt19937 gen){

    cords point;
    bool valid = false;

    int attempts = 0;
    int max_attempts = matrix.size() * matrix[0].size();

    while (!valid && attempts++ < max_attempts) {
        point = {int((gen() % matrix[0].size())), int(gen() % matrix.size())};
        valid = true;

        // check neighbors
        for (int i = -1; i <= 1 && valid; ++i) {
            for (int j = -1; j <= 1 && valid; ++j) {
                int newX = point.x + i;
                int newY = point.y + j;

                if (newX >= 0 && newX < matrix[0].size() && newY >= 0 && newY < matrix.size()) {
                    if (matrix[newY][newX] != WALL) valid = false;
                }
            }
        }
    }

    if (!valid) return {-1, -1};
    return point;
}

// O(n * m * √mn)
void set_path(Matrix& matrix, cords start, cords exit) {

    cords treasure = {-1, -1};

    // get amount of intermediate points
    double scale_factor = std::sqrt(matrix.size() * matrix[0].size());
    int points_amount = int(matrix.size() * matrix[0].size() / scale_factor);

    cords current = {start.x, start.y};

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < points_amount; ++i) {
        // set point
        cords key_point = set_key_point(matrix, gen);
        if (key_point.x == -1 && key_point.y == -1) break;

        // get path to the point
        std::vector<cords> path = BFS(current, key_point);
        for (cords cords: path) matrix[cords.y][cords.x] = PATH;

        // set treasure
        if (treasure.x == -1 && treasure.y == -1 && gen() % points_amount == 1) {
            treasure.x = key_point.x;
            treasure.y = key_point.y;
        }
        // set current point to the point
        current = key_point;
    }

    std::vector<cords> path = BFS(current, exit);
    for (cords cords: path)
        matrix[cords.y][cords.x] = PATH;

    matrix[start.y][start.x] = START;
    matrix[exit.y][exit.x] = EXIT;
    if (!(treasure.x == -1 && treasure.y == -1)) matrix[treasure.y][treasure.x] = TREASURE;
}

int main() {
    int rows, cols;
    read_size(&rows, &cols);

    Matrix matrix = matrix_initialization(cols, rows);

    cords start, exit;

    set_exits(matrix, &start, &exit);

    set_path(matrix, start, exit);

    print_matrix(matrix);
    return 0;
}
