# BruinNav

BruinNav is a small C++ program that loads a street map, finds a route between two places using the A* pathfinding algorithm, and prints clear, human-readable turn-by-turn directions.

---

## Overview
- **Input:** a text map file with streets + intersections + attractions (place names/addresses).
- **Output:** a sequence of steps like “Proceed 0.12 miles east on …; Turn left onto …” plus total distance.
- **Core idea:** model the map as a graph of street segments; use **A\*** (with straight-line distance heuristic) to find a low-cost path; convert the path into readable instructions.

---

## Features
- Start/end by **attraction or address name** (resolved to coordinates).
- **A\*** routing on a street-segment graph (efficient and optimal with an admissible heuristic).
- **Directions you can read:** proceed segments, left/right turns, distances, and headings.
- Clear status results: success, no route, or bad start/end.
