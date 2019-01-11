# Star

"star.c" is an implementation of the A* pathfinding algorithm on a randomly generated maze. The maze is generated using a depth-first search algorithm. The maze is output to the console and uses '#' for walls, and '.' for the found path. The entrance is at the top left hand corner, the exit is at the bottom right hand corner and a path between the entrance and exit is guaranteed to exist. The height and width provided have to be odd.

The program can be run with:
- "./star" for testing
- "./star *height* *width*" to generate a random maze
- "./star *height* *width* Astar" to generate and solve a maze

Resources used:
- Maze generation: https://en.wikipedia.org/wiki/Maze_generation_algorithm
- An introduction to heuristic algorithms: https://www.researchgate.net/publication/228573156_An_introduction_to_heuristic_algorithms
- A* algorithm: https://en.wikipedia.org/wiki/A*_search_algorithm
