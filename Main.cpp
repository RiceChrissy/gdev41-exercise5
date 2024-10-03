//  g++ Main.cpp -o Main -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm
//  ./Main.exe
#include <raylib.h>
#include <raymath.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int numberOfCircles = 5;
const float FPS = 60;
const float TIMESTEP = 1 / FPS; // Sets the timestep to 1 / FPS. But timestep can be any very small value.
const float FRICTION = 2.5;

struct Ball
{
    Vector2 position;
    float radius;
    Color color;

    float mass;
    float inverse_mass; // A variable for 1 / mass. Used in the calculation for acceleration = sum of forces / mass
    Vector2 acceleration;
    Vector2 velocity;

    bool isStill; // Determines if the ball is still or is moving

    Vector2 defaultPosition;
    bool isVisible;
};

struct Border
{
    Vector2 position;
    float width;
    float height;
    Color color;
};

int toggleElasticity(int e)
{
    if (e == 0)
    {
        return 1;
    }
    return 0;
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
        std::cout << "colliding" << std::endl;
        return true;
    }
    return false;
}

bool isCircleCollidingWithCorner(Ball b1, Ball upperLeft, Ball upperRight, Ball lowerLeft, Ball lowerRight)
{
    if (isCirclesColliding(b1, upperLeft))
    {
        std::cout << "colliding with upper left" << std::endl;
        return true;
    }
    if (isCirclesColliding(b1, upperRight))
    {
        std::cout << "colliding with upper right" << std::endl;
        return true;
    }
    if (isCirclesColliding(b1, lowerLeft))
    {
        std::cout << "colliding with lower left" << std::endl;
        return true;
    }
    if (isCirclesColliding(b1, lowerRight))
    {
        std::cout << "colliding with lower right" << std::endl;
        return true;
    }
    return false;
}

bool isCircleCollidingWithBorder(Ball b1, Border br1)
{
    float nearestX = fmaxf(br1.position.x, fminf(b1.position.x, br1.position.x + br1.width));
    float nearestY = fmaxf(br1.position.y, fminf(b1.position.y, br1.position.y + br1.height));

    float distance = getDistanceToPoint(b1, {nearestX, nearestY});
    if (distance <= b1.radius)
    {
        return true;
    }
    return false;
}

int main()
{
    Ball ball;
    ball.position = {200, WINDOW_HEIGHT / 2};
    ball.radius = 30.0f;
    ball.color = WHITE;
    ball.mass = 1.0f;
    ball.inverse_mass = 1 / ball.mass;
    ball.acceleration = Vector2Zero();
    ball.velocity = Vector2Zero();
    ball.isVisible = true;
    ball.defaultPosition = ball.position;

    Ball ball2;
    ball2.position = {500, WINDOW_HEIGHT / 2};
    ball2.radius = 30.0f;
    ball2.color = PURPLE;
    ball2.mass = 1.0f;
    ball2.inverse_mass = 1 / ball.mass;
    ball2.acceleration = Vector2Zero();
    ball2.velocity = Vector2Zero();
    ball2.isVisible = true;
    ball2.defaultPosition = ball2.position;

    Ball ball3;
    ball3.position = {600, WINDOW_HEIGHT / 2};
    ball3.radius = 30.0f;
    ball3.color = YELLOW;
    ball3.mass = 1.0f;
    ball3.inverse_mass = 1 / ball.mass;
    ball3.acceleration = Vector2Zero();
    ball3.velocity = Vector2Zero();
    ball3.isVisible = true;
    ball3.defaultPosition = ball3.position;

    Ball ball4;
    ball4.position = {550, (WINDOW_HEIGHT / 2) - 40};
    ball4.radius = 30.0f;
    ball4.color = BLACK;
    ball4.mass = 1.0f;
    ball4.inverse_mass = 1 / ball.mass;
    ball4.acceleration = Vector2Zero();
    ball4.velocity = Vector2Zero();
    ball4.isVisible = true;
    ball4.defaultPosition = ball4.position;

    Ball ball5;
    ball5.position = {550, (WINDOW_HEIGHT / 2) + 40};
    ball5.radius = 30.0f;
    ball5.color = BLUE;
    ball5.mass = 1.0f;
    ball5.inverse_mass = 1 / ball.mass;
    ball5.acceleration = Vector2Zero();
    ball5.velocity = Vector2Zero();
    ball5.isVisible = true;
    ball5.defaultPosition = ball5.position;

    Ball ballArray[numberOfCircles] = {ball, ball2, ball3, ball4, ball5};

    Ball upperLeft;
    upperLeft.position = {37.5, 37.5};
    upperLeft.radius = 37.5;
    upperLeft.color = BLACK;
    upperLeft.mass = 0.0f;
    upperLeft.inverse_mass = 1 / ball.mass;
    upperLeft.acceleration = Vector2Zero();
    upperLeft.velocity = Vector2Zero();
    upperLeft.isVisible = true;

    Ball upperRight;
    upperRight.position = {800 - 37.5, 37.5};
    upperRight.radius = 37.5;
    upperRight.color = BLACK;
    upperRight.mass = 0.0f;
    upperRight.inverse_mass = 1 / ball.mass;
    upperRight.acceleration = Vector2Zero();
    upperRight.velocity = Vector2Zero();
    upperRight.isVisible = true;

    Ball lowerLeft;
    lowerLeft.position = {37.5, 600 - 37.5};
    lowerLeft.radius = 37.5;
    lowerLeft.color = BLACK;
    lowerLeft.mass = 0.0f;
    lowerLeft.inverse_mass = 1 / ball.mass;
    lowerLeft.acceleration = Vector2Zero();
    lowerLeft.velocity = Vector2Zero();
    lowerLeft.isVisible = true;

    Ball lowerRight;
    lowerRight.position = {800 - 37.5, 600 - 37.5};
    lowerRight.radius = 37.5;
    lowerRight.color = BLACK;
    lowerRight.mass = 0.0f;
    lowerRight.inverse_mass = 1 / ball.mass;
    lowerRight.acceleration = Vector2Zero();
    lowerRight.velocity = Vector2Zero();
    lowerRight.isVisible = true;

    Border top;
    top.position = {75, 0};
    top.width = 650;
    top.height = 40;
    top.color = RED;

    Border left;
    left.position = {0, 75};
    left.width = 40;
    left.height = 450;
    left.color = RED;

    Border right;
    right.position = {760, 75};
    right.width = 40;
    right.height = 450;
    right.color = RED;

    Border bottom;
    bottom.position = {75, 560};
    bottom.width = 650;
    bottom.height = 40;
    bottom.color = RED;

    int elasticityCoefficient = 0.5f;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OlivaresTamano - Homework 3");

    SetTargetFPS(FPS);

    float accumulator = 0;

    while (!WindowShouldClose())
    {
        float delta_time = GetFrameTime();
        Vector2 forces = Vector2Zero();
        Vector2 *dragLine0 = new Vector2{Vector2Zero()}; // Cueball Position
        Vector2 *dragLine1 = new Vector2{Vector2Zero()}; // Mouse Position
        Vector2 *dragLine2 = new Vector2{Vector2Zero()}; // Vector Between Cueball and Mouse
        bool ballsAreStill = true;

        // Checking if all of the balls have stopped
        for (int i = 0; i < numberOfCircles; i++)
        {
            ballArray[i].isStill = (abs(ballArray[i].velocity.x) < 0.1f && abs(ballArray[i].velocity.y) < 0.1f) ? true : false;
        }
        for (int i = 0; i < numberOfCircles; i++)
        {
            if (ballArray[i].isStill == false)
            {
                ballsAreStill = false;
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && ballsAreStill)
        {
            *dragLine0 = ballArray[0].position;
            *dragLine1 = GetMousePosition();

            if (Vector2Distance(*dragLine0, *dragLine1) > 150.0f)
            {
                *dragLine2 = Vector2Scale(Vector2Normalize(Vector2Subtract(*dragLine1, *dragLine0)), 150.0f);
            }
            else
            {
                *dragLine2 = Vector2Scale(Vector2Normalize(Vector2Subtract(*dragLine1, *dragLine0)), Vector2Distance(*dragLine0, *dragLine1));
            }

            std::cout << dragLine1->x << " " << dragLine1->y << "   " << GetMousePosition().x << " " << GetMousePosition().y << std::endl;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && ballsAreStill)
        {
            *dragLine0 = ballArray[0].position;
            *dragLine1 = GetMousePosition();
            if (Vector2Distance(*dragLine0, *dragLine1) > 150.0f)
            {
                *dragLine2 = Vector2Scale(Vector2Normalize(Vector2Subtract(*dragLine1, *dragLine0)), 150.0f);
            }
            else
            {
                *dragLine2 = Vector2Scale(Vector2Normalize(Vector2Subtract(*dragLine1, *dragLine0)), Vector2Distance(*dragLine0, *dragLine1));
            }
            // Multiplying forces by 500
            forces = Vector2Add(forces, Vector2Scale(*dragLine2, -500.0f));

            std::cout << "Force Vector : " << forces.x << " " << forces.y << std::endl;
            delete[] dragLine0, dragLine1, dragLine2;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            std::cout << "key pressed space" << std::endl;
            for (int i = 0; i < numberOfCircles; i++)
            {
                std::cout << "ball position " << i << " : " << ballArray[i].position.x << " " << ballArray[i].position.y << std::endl;
                ballArray[i].position = ballArray[i].defaultPosition;
                ballArray[i].acceleration = Vector2Zero();
                ballArray[i].velocity = Vector2Zero();
                ballArray[i].isVisible = true;
            }
        }
        ballArray[0].acceleration = Vector2Scale(forces, ball.inverse_mass);

        // Physics
        accumulator += delta_time;
        while (accumulator >= TIMESTEP)
        {
            for (int i = 0; i < numberOfCircles; i++)
            {
                // std::cout << "i, k: " << i << " " << k << std::endl;
                ballArray[i].velocity = Vector2Add(ballArray[i].velocity, Vector2Scale(ballArray[i].acceleration, TIMESTEP));
                ballArray[i].velocity = Vector2Subtract(ballArray[i].velocity, Vector2Scale(ballArray[i].velocity, FRICTION * ballArray[i].inverse_mass * TIMESTEP));
                // std::cout << ballArray[0].velocity.x << " " << ballArray[0].velocity.y << std::endl;
                ballArray[i].position = Vector2Add(ballArray[i].position, Vector2Scale(ballArray[i].velocity, TIMESTEP));

                if (ballArray[i].position.x + ballArray[i].radius >= WINDOW_WIDTH || ballArray[i].position.x - ballArray[i].radius <= 0)
                {
                    ballArray[i].velocity.x *= -1;
                }
                if (ballArray[i].position.y + ballArray[i].radius >= WINDOW_HEIGHT || ballArray[i].position.y - ballArray[i].radius <= 0)
                {
                    ballArray[i].velocity.y *= -1;
                }
                if (isCircleCollidingWithBorder(ballArray[i], top) && ballArray[i].isVisible)
                {
                    ballArray[i].velocity.y *= -1;
                }
                if (isCircleCollidingWithBorder(ballArray[i], bottom) && ballArray[i].isVisible)
                {
                    ballArray[i].velocity.y *= -1;
                }
                if (isCircleCollidingWithBorder(ballArray[i], left) && ballArray[i].isVisible)
                {
                    ballArray[i].velocity.x *= -1;
                }
                if (isCircleCollidingWithBorder(ballArray[i], right) && ballArray[i].isVisible)
                {
                    ballArray[i].velocity.x *= -1;
                }
                for (int k = 0; k < numberOfCircles; k++)
                {
                    if (k == i)
                    {
                        k++;
                    }
                    if (k >= numberOfCircles || i >= numberOfCircles)
                    {
                        break;
                    }

                    if (isCirclesColliding(ballArray[i], ballArray[k]) && ballArray[i].isVisible && ballArray[k].isVisible)
                    {
                        Vector2 n = Vector2Subtract(ballArray[i].position, ballArray[k].position); // currently swapped (as per sir's comment)
                        float j = -(((1 + elasticityCoefficient) * Vector2DotProduct(Vector2Subtract(ballArray[i].velocity, ballArray[k].velocity /**/), n)) / (Vector2DotProduct(n, n) * (1 / ballArray[i].mass) + (1 / ballArray[k].mass)));
                        Vector2 newVelocity = Vector2Add(ballArray[i].velocity, Vector2Scale(n, (j / ballArray[i].mass)));
                        ballArray[i].velocity = newVelocity;
                        Vector2 newVelocity2 = Vector2Subtract(ballArray[k].velocity, Vector2Scale(n, (j / ballArray[k].mass)));
                        ballArray[k].velocity = newVelocity2;
                    }
                    if (isCircleCollidingWithCorner(ballArray[i], upperLeft, upperRight, lowerLeft, lowerRight))
                    {
                        if (i == 0)
                        {
                            ballArray[i].position = ballArray[i].defaultPosition;
                            ballArray[i].acceleration = Vector2Zero();
                            ballArray[i].velocity = Vector2Zero();
                        }
                        else
                        {
                            ballArray[i].isVisible = false;
                        }
                    }
                }
            }
            accumulator -= TIMESTEP;
        }

        BeginDrawing();
        ClearBackground(WHITE);

        for (size_t i = 0; i < numberOfCircles; i++)
        {
            if (ballArray[i].isVisible)
            {
                DrawCircleV(ballArray[i].position, ballArray[i].radius, ballArray[i].color);
            }
        }

        // pool table border
        ClearBackground(LIME);
        DrawRectangle(top.position.x, top.position.y, top.width, top.height, top.color);                // Top Bar
        DrawRectangle(left.position.x, left.position.y, left.width, left.height, left.color);           // Left Bar
        DrawRectangle(right.position.x, right.position.y, right.width, right.height, right.color);      // Right Bar
        DrawRectangle(bottom.position.x, bottom.position.y, bottom.width, bottom.height, bottom.color); // Bottom Bar

        // pool table corners
        DrawCircleV(upperLeft.position, upperLeft.radius, upperLeft.color);    // Upper Left
        DrawCircleV(upperRight.position, upperRight.radius, upperRight.color); // Upper Right
        DrawCircleV(lowerLeft.position, lowerLeft.radius, lowerLeft.color);    // Lower Left
        DrawCircleV(lowerRight.position, lowerRight.radius, lowerRight.color); // Lower Right

        // cue ball dragging
        DrawLineEx(*dragLine0, Vector2Add(*dragLine0, *dragLine2), 7.5f, YELLOW);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}