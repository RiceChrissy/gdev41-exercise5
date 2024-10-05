//  g++ Main.cpp -o Main -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm
//  ./Main.exe
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
// const int numberOfCircles = 5;
const float FPS = 60;
const float TIMESTEP = 1 / FPS; // Sets the timestep to 1 / FPS. But timestep can be any very small value.
int cellSize = 50;

struct Ball
{
    Vector2 position;
    float radius;
    Color color;

    float mass;
    float inverse_mass; // A variable for 1 / mass. Used in the calculation for acceleration = sum of forces / mass
    Vector2 velocity;
};

struct cell
{
    Vector2 position;
    Color color;
    Vector2 max;
    Vector2 min;

    std::vector<Ball> ballsInCell;

    bool operator==(const cell &cell)
    {
        return (this->position.x == cell.position.x && this->position.y == cell.position.y);
    }

    bool operator==(const Vector2 &position)
    {
        return (this->position.x == position.x && this->position.y == position.y);
    }
    void addBall(Ball ball)
    {
        this->ballsInCell.push_back(ball);
    }
    void clearBalls()
    {
        this->ballsInCell.clear();
    }
};

Vector2 getNearestIndexAtPoint(Vector2 position)
{
    return Vector2{std::floor(position.x / cellSize), std::floor(position.y / cellSize)};
}

void initializeCell(cell &testCell, Vector2 &pos, Color &color)
{
    testCell.position = pos;
    // testCell.cellSize = cellSize;
    testCell.max = Vector2{testCell.position.x + cellSize, testCell.position.y};
    testCell.min = Vector2{testCell.position.x, testCell.position.y + cellSize};
    testCell.color = color;
}

void initializeAllCells(std::vector<std::vector<cell>> &Cells)
{
    // get array of columns, store it in a row.
    int numberOFRows = std::ceil((float)WINDOW_HEIGHT / (float)cellSize);
    int numberOfColumns = std::ceil((float)WINDOW_WIDTH / (float)cellSize);
    std::cout << numberOFRows << std::endl;
    std::cout << numberOfColumns << std::endl;
    for (int i = 0; i < numberOFRows; i++)
    {
        float iTemp = (float)i;
        std::vector<cell> row;
        for (int j = 0; j < numberOfColumns; j++)
        {
            cell x;
            Color red = RED;
            Vector2 newVector = {(float)(j * cellSize), (float)(i * cellSize)};
            std::cout << newVector.x << " " << newVector.y << std::endl;
            initializeCell(x, newVector, red);
            row.push_back(x);
        }
        Cells.push_back(row);
    }
}

void addBallToCell(std::vector<std::vector<cell>> &grid, Ball ball)
{
    Vector2 max = Vector2{ball.position.x + ball.radius, ball.position.y + ball.radius};
    Vector2 min = Vector2{ball.position.x - ball.radius, ball.position.y - ball.radius};

    Vector2 indexAtMin = getNearestIndexAtPoint(min);
    Vector2 indexAtCenter = getNearestIndexAtPoint(ball.position);
    Vector2 indexAtMax = getNearestIndexAtPoint(max);

    // std::cout << "Index At center" << indexAtCenter.x << " " << indexAtCenter.y << std::endl;
    /*
    grid[indexAtMin.x][indexAtMin.y].addBall(ball);

    grid[indexAtMax.x][indexAtMax.y].addBall(ball);
    */
    // grid[indexAtCenter.x][indexAtCenter.y].color = BLUE;
}

void updateCellContents(std::vector<std::vector<cell>> &grid, std::vector<Ball> &balls)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            grid[i][j].clearBalls();
            if (grid[i][j].ballsInCell.size() > 0)
            {
                grid[i][j].color = BLUE;
            }
            else
            {
                grid[i][j].color = RED;
            }
        }
    }
    for (int k = 0; k < balls.size(); k++)
    {
        addBallToCell(grid, balls[k]);
    }
}

void updateCellVisuals(std::vector<std::vector<cell>> &grid)
{
}

float getDistance(Ball b1, Ball b2)
{
    Vector2 dist = Vector2Subtract(b1.position, b2.position);
    return std::abs(Vector2Length(dist));
}

float getDistanceToPoint(Ball b1, Vector2 pos)
{
    Vector2 dist = Vector2Subtract(b1.position, pos);
    return std::abs(Vector2Length(dist));
}

bool isCirclesColliding(Ball b1, Ball b2)
{
    float sumOfRadii = b1.radius + b2.radius;
    float distance = getDistance(b1, b2);
    if (distance <= sumOfRadii)
    {
        // std::cout << "colliding" << std::endl;
        return true;
    }
    return false;
}

// Deprecated atm
// bool isCircleCollidingWithBorder(Ball b1, Border br1)
// {
//     float nearestX = fmaxf(br1.position.x, fminf(b1.position.x, br1.position.x + br1.width));
//     float nearestY = fmaxf(br1.position.y, fminf(b1.position.y, br1.position.y + br1.height));

//     float distance = getDistanceToPoint(b1, {nearestX, nearestY});
//     if (distance <= b1.radius)
//     {
//         return true;
//     }
//     return false;
// }

float RandomDirection()
{
    float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    // Make it [-1, 1]
    return x * 2.0f - 1.0f;
}

void InitializeBall(std::vector<Ball> &array, int arraySize, bool isLarge)
{
    for (size_t i = 0; i < arraySize; i++)
    {
        Ball ball;
        Color randomColor = {
            GetRandomValue(0, 255),
            GetRandomValue(0, 255),
            GetRandomValue(0, 255),
            255};
        ball.position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
        if (isLarge)
        {
            ball.radius = 25.0f;
            ball.mass = 10.0f;
        }
        else
        {
            ball.radius = (float)GetRandomValue(5, 10);
            ball.mass = 1.0f;
        }
        ball.inverse_mass = 1.0f / ball.mass;
        ball.color = randomColor;
        ball.velocity = {500.0f * RandomDirection(), 500.0f * RandomDirection()};
        array.push_back(ball);
    }
}

// void UpdateParticle(Particle *particle, float deltaTime)
// {
//     if (particle->lifeTime <= 0)
//     {
//         particle->isActive = false;
//     }
//     else
//     {
//         particle->position.x += (particle->speed * particle->direction.x) * deltaTime;
//         particle->position.y += (particle->speed * particle->direction.y) * deltaTime;
//         particle->lifeTime -= deltaTime;
//         particle->color.a = 255 * (particle->lifeTime / particle->totalTime);
//     }
// }

int main()
{
    // Ball ball;
    // ball.position = {200, WINDOW_HEIGHT / 2};
    // ball.radius = 30.0f;
    // ball.color = WHITE;
    // ball.mass = 1.0f;
    // ball.inverse_mass = 1 / ball.mass;
    // ball.velocity = Vector2Zero();

    int elasticityCoefficient = 1.0f;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OlivaresTamano - Homework 3");

    SetTargetFPS(FPS);

    float accumulator = 0;

    std::vector<Ball> ballArray;
    int spawnInstance = 0;

    std::vector<std::vector<cell>> grid;
    initializeAllCells(grid);

    bool drawGrid = false;

    while (!WindowShouldClose())
    {

        float delta_time = GetFrameTime();
        Vector2 forces = Vector2Zero();
        Vector2 mouseIndexLocation = getNearestIndexAtPoint(GetMousePosition());
        if (IsMouseButtonPressed(0))
        {
            std::cout << "MOUSE INDEX: " << mouseIndexLocation.x << " " << mouseIndexLocation.y << std::endl;
            std::cout << "GRID RETURNS: " << grid[mouseIndexLocation.y][mouseIndexLocation.x].position.x << " " << grid[mouseIndexLocation.y][mouseIndexLocation.x].position.y << std::endl;
        }

        // std::cout << mouseIndexLocation.x << " " <<  mouseIndexLocation.y << std::endl;
        updateCellContents(grid, ballArray);
        if (IsKeyPressed(KEY_TAB))
        {
            drawGrid = !drawGrid;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            if (spawnInstance == 9)
            {
                InitializeBall(ballArray, 1, true);
                spawnInstance = 0;
            }
            else
            {
                InitializeBall(ballArray, 25, false);
                spawnInstance++;
            }
            // std::cout << ballArray.size() << std::endl;
        }

        // Physics
        accumulator += delta_time;
        while (accumulator >= TIMESTEP)
        {
            for (int i = 0; i < ballArray.size(); i++)
            {
                // std::cout << "i, k: " << i << " " << k << std::endl;
                // ballArray[i].velocity = Vector2Add(ballArray[i].velocity, Vector2Scale(ballArray[i].acceleration, TIMESTEP));
                // ballArray[i].velocity = Vector2Subtract(ballArray[i].velocity, Vector2Scale(ballArray[i].velocity, ballArray[i].inverse_mass * TIMESTEP));
                // std::cout << ballArray[0].velocity.x << " " << ballArray[0].velocity.y << std::endl;
                ballArray[i].position = Vector2Add(ballArray[i].position, Vector2Scale(ballArray[i].velocity, TIMESTEP));

                // Boundary Collision
                if (ballArray[i].position.x - ballArray[i].radius <= 0 && ballArray[i].velocity.x < 0)
                {
                    ballArray[i].velocity.x *= -1;
                }
                if (ballArray[i].position.x + ballArray[i].radius >= WINDOW_WIDTH && ballArray[i].velocity.x > 0)
                {
                    ballArray[i].velocity.x *= -1;
                }
                if (ballArray[i].position.y - ballArray[i].radius <= 0 && ballArray[i].velocity.y < 0)
                {
                    ballArray[i].velocity.y *= -1;
                }
                if (ballArray[i].position.y + ballArray[i].radius >= WINDOW_HEIGHT && ballArray[i].velocity.y > 0)
                {
                    ballArray[i].velocity.y *= -1;
                }

                for (int k = 0; k < ballArray.size(); k++)
                {
                    if (k == i)
                    {
                        k++;
                    }
                    else if (k >= ballArray.size() || i >= ballArray.size())
                    {
                        break;
                    }

                    Vector2 n = Vector2Normalize(Vector2Subtract(ballArray[i].position, ballArray[k].position)); // currently swapped (as per sir's comment)
                    if (isCirclesColliding(ballArray[i], ballArray[k]) && Vector2DotProduct(n, Vector2Subtract(ballArray[i].velocity, ballArray[k].velocity /**/)) < 0)
                    {
                        float j = -(((1 + elasticityCoefficient) * Vector2DotProduct(Vector2Subtract(ballArray[i].velocity, ballArray[k].velocity /**/), n)) / (Vector2DotProduct(n, n) * (1 / ballArray[i].mass) + (1 / ballArray[k].mass)));
                        Vector2 newVelocity = Vector2Add(ballArray[i].velocity, Vector2Scale(n, (j / ballArray[i].mass)));
                        ballArray[i].velocity = newVelocity;
                        Vector2 newVelocity2 = Vector2Subtract(ballArray[k].velocity, Vector2Scale(n, (j / ballArray[k].mass)));
                        ballArray[k].velocity = newVelocity2;
                    }
                }
            }
            accumulator -= TIMESTEP;
        }

        BeginDrawing();
        ClearBackground(WHITE);

        for (int i = 0; i < ballArray.size(); i++)
        {
            DrawCircleV(ballArray[i].position, ballArray[i].radius, ballArray[i].color);
        }

        if (drawGrid)
        {
            for (int i = 0; i < grid.size(); i++)
            {
                for (int j = 0; j < grid[i].size(); j++)
                {
                    DrawRectangleLines(grid[i][j].position.x, grid[i][j].position.y, cellSize, cellSize, grid[i][j].color);
                }
            }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}