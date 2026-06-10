#include "raylib.h"
#include <vector>
#include <unordered_map>
#include <cmath>

struct Agent
{
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
    bool escaped;
};

int GetCellKey(int x, int y)
{
    return x * 100000 + y;
}

float Distance(Vector2 a, Vector2 b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

void AvoidCollision(Agent& a, Agent& b, int& collisionChecks)
{
    collisionChecks++;

    if (a.escaped || b.escaped)
    {
        return;
    }

    float dx = a.position.x - b.position.x;
    float dy = a.position.y - b.position.y;

    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = a.radius + b.radius;

    if (distanceSquared <= radiusSum * radiusSum && distanceSquared > 0)
    {
        float distance = sqrtf(distanceSquared);

        float nx = dx / distance;
        float ny = dy / distance;

        a.position.x += nx * 0.5f;
        a.position.y += ny * 0.5f;

        b.position.x -= nx * 0.5f;
        b.position.y -= ny * 0.5f;
    }
}

Agent CreateAgent(int screenWidth, int screenHeight)
{
    Agent agent;

    agent.position = {
        (float)GetRandomValue(50, screenWidth - 300),
        (float)GetRandomValue(80, screenHeight - 80)
    };

    agent.velocity = { 0.0f, 0.0f };
    agent.radius = 4.0f;
    agent.escaped = false;

    agent.color = Color{
        (unsigned char)GetRandomValue(80, 255),
        (unsigned char)GetRandomValue(80, 255),
        (unsigned char)GetRandomValue(80, 255),
        255
    };

    return agent;
}

void MoveToExit(Agent& agent, Vector2 exitPoint, float speed)
{
    if (agent.escaped)
    {
        return;
    }

    float dx = exitPoint.x - agent.position.x;
    float dy = exitPoint.y - agent.position.y;

    float length = sqrtf(dx * dx + dy * dy);

    if (length > 0)
    {
        agent.velocity.x = (dx / length) * speed;
        agent.velocity.y = (dy / length) * speed;
    }

    agent.position.x += agent.velocity.x;
    agent.position.y += agent.velocity.y;
}

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;
    const int cellSize = 25;

    int agentCount = 1000;
    int mode = 2;
    int collisionChecks = 0;
    int gridCellsUsed = 0;
    int escapedCount = 0;

    float agentSpeed = 1.4f;

    bool paused = false;

    Vector2 exitPoint = { screenWidth - 40.0f, screenHeight / 2.0f };

    InitWindow(screenWidth, screenHeight, "Simulare evacuare multime - Raylib");
    SetTargetFPS(60);

    std::vector<Agent> agents;

    for (int i = 0; i < agentCount; i++)
    {
        agents.push_back(CreateAgent(screenWidth, screenHeight));
    }

    while (!WindowShouldClose())
    {
        collisionChecks = 0;
        gridCellsUsed = 0;
        escapedCount = 0;

        if (IsKeyPressed(KEY_SPACE))
        {
            paused = !paused;
        }


        if (IsKeyPressed(KEY_UP))
        {
            for (int i = 0; i < 100; i++)
            {
                agents.push_back(CreateAgent(screenWidth, screenHeight));
            }

            agentCount += 100;
        }

        if (IsKeyPressed(KEY_DOWN))
        {
            if (agentCount > 100)
            {
                agents.resize(agentCount - 100);
                agentCount -= 100;
            }
        }

        if (IsKeyPressed(KEY_ONE))
        {
            mode = 1;
        }

        if (IsKeyPressed(KEY_TWO))
        {
            mode = 2;
        }

        if (IsKeyPressed(KEY_R))
        {
            agents.clear();

            for (int i = 0; i < agentCount; i++)
            {
                agents.push_back(CreateAgent(screenWidth, screenHeight));
            }
        }

        if (IsKeyPressed(KEY_G))
        {
            agentSpeed += 0.3f;

            if (agentSpeed > 3.0f)
            {
                agentSpeed = 0.8f;
            }
        }

        if (!paused)
        {
            for (auto& agent : agents)
            {
                MoveToExit(agent, exitPoint, agentSpeed);

                if (Distance(agent.position, exitPoint) < 25)
                {
                    agent.escaped = true;
                }
            }
        }

        if (mode == 1)
        {
            for (int i = 0; i < agents.size(); i++)
            {
                for (int j = i + 1; j < agents.size(); j++)
                {
                    AvoidCollision(agents[i], agents[j], collisionChecks);
                }
            }
        }

        if (mode == 2)
        {
            std::unordered_map<int, std::vector<int>> grid;

            for (int i = 0; i < agents.size(); i++)
            {
                if (!agents[i].escaped)
                {
                    int cellX = (int)(agents[i].position.x / cellSize);
                    int cellY = (int)(agents[i].position.y / cellSize);

                    int key = GetCellKey(cellX, cellY);
                    grid[key].push_back(i);
                }
            }

            gridCellsUsed = (int)grid.size();

            for (int i = 0; i < agents.size(); i++)
            {
                if (!agents[i].escaped)
                {
                    int cellX = (int)(agents[i].position.x / cellSize);
                    int cellY = (int)(agents[i].position.y / cellSize);

                    for (int offsetX = -1; offsetX <= 1; offsetX++)
                    {
                        for (int offsetY = -1; offsetY <= 1; offsetY++)
                        {
                            int neighborX = cellX + offsetX;
                            int neighborY = cellY + offsetY;

                            int key = GetCellKey(neighborX, neighborY);

                            if (grid.find(key) != grid.end())
                            {
                                for (int j : grid[key])
                                {
                                    if (j > i)
                                    {
                                        AvoidCollision(agents[i], agents[j], collisionChecks);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        for (auto& agent : agents)
        {
            if (agent.escaped)
            {
                escapedCount++;
            }

            if (agent.position.x <= agent.radius)
            {
                agent.position.x = agent.radius;
            }

            if (agent.position.y <= agent.radius)
            {
                agent.position.y = agent.radius;
            }

            if (agent.position.y >= screenHeight - agent.radius)
            {
                agent.position.y = screenHeight - agent.radius;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawRectangle(screenWidth - 60, screenHeight / 2 - 60, 50, 120, GREEN);
        DrawText("EXIT", screenWidth - 58, screenHeight / 2 - 10, 20, BLACK);

        for (const auto& agent : agents)
        {
            if (!agent.escaped)
            {
                DrawCircleV(agent.position, agent.radius, agent.color);
            }
        }

        DrawFPS(10, 10);
        DrawText(TextFormat("Frame time: %.2f ms", GetFrameTime() * 1000), 10, 35, 20, WHITE);
        DrawText(TextFormat("Agents: %d", agentCount), 10, 60, 20, WHITE);
        DrawText(TextFormat("Escaped: %d", escapedCount), 10, 85, 20, WHITE);
        DrawText(TextFormat("Speed: %.1f", agentSpeed), 10, 110, 20, WHITE);
        DrawText(TextFormat("Mode: %s", mode == 1 ? "Naive collision check" : "Grid optimized"), 10, 135, 20, WHITE);
        DrawText(TextFormat("Collision checks: %d", collisionChecks), 10, 160, 20, WHITE);
        DrawText(TextFormat("Grid cells used: %d", gridCellsUsed), 10, 185, 20, WHITE);

        DrawText("1: naive | 2: optimized | UP/DOWN: agents | G: speed | R: reset", 10, 215, 20, WHITE);
        DrawText("SPACE: Pause / Resume", 10, 240, 20, WHITE);
        if (paused)
        {
            DrawText("PAUSED", 550, 30, 40, YELLOW);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
