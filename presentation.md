# Crowd Evacuation Simulation

## 1. Project Overview

Project Name:
Crowd Evacuation Simulation Using Raylib

Language:

* C++

Graphics Library:

* Raylib

Project Type:

* Real-time simulation

The purpose of the project is to simulate a group of agents trying to reach the same exit while avoiding collisions with each other.

---

# 2. Why did I choose this project?

I wanted to create something visual and interactive instead of a console application.

The project also allowed me to practice:

* C++
* Raylib
* collision detection
* real-time rendering
* optimization techniques

---

# 3. Main idea

The simulation generates multiple agents.

Each agent:

* has a position
* has a velocity
* has a radius
* has a color

Every frame:

* the agent moves toward the exit
* collisions are checked
* the screen is updated

---

# 4. Agent Structure

Each agent contains:

* position
* velocity
* radius
* color
* escaped status

Agents are stored inside:

std::vector<Agent>

---

# 5. Collision Detection

The project implements two different methods.

## Naive Method

Every agent is checked against every other agent.

Advantages:

* simple
* easy to implement

Disadvantages:

* many unnecessary comparisons
* slower with many agents

---

## Grid Optimized Method

The simulation space is divided into cells.

Each agent belongs to one grid cell.

Collision checks are performed only with nearby agents.

Advantages:

* fewer collision checks
* better performance
* scalable solution

---

# 6. Spatial Grid

The grid is implemented using:

std::unordered_map<int, std::vector<int>>

Each key represents one cell.

Agents are inserted into their corresponding cells.

Neighboring cells are checked for collisions.

---

# 7. Movement

Every frame:

1. calculate direction to exit

2. normalize direction

3. multiply by speed

4. update position

When an agent reaches the exit:

escaped = true

The agent is no longer rendered.

---

# 8. Collision Avoidance

If two agents become too close:

* distance is computed

* overlap is detected

* positions are adjusted

This creates a simple collision avoidance behavior.

---

# 9. User Controls

1

* Naive collision mode

2

* Grid optimized mode

UP

* Add agents

DOWN

* Remove agents

G

* Change speed

R

* Reset simulation

SPACE

* Pause / Resume

---

# 10. Statistics Displayed

The application displays:

* FPS

* Frame Time

* Number of Agents

* Escaped Agents

* Current Speed

* Collision Checks

* Grid Cells Used

---

# 11. Why Raylib?

Raylib is:

* lightweight

* easy to use

* beginner friendly

* suitable for real-time graphics

* simple to integrate with C++

---

# 12. Why C++?

C++ offers:

* high performance

* efficient memory management

* real-time execution

* good support for simulation applications

---

# 13. Future Improvements

Possible extensions:

* obstacles

* multiple exits

* pathfinding

* different agent behaviors

* group movement

* better AI

* larger environments

---
