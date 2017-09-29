# Summary
Basic version of Tetris.

### Algorithm
1) a random figure spawns
2) if figure collides with the rest field, clear field
3) once in a while move figure down
4) player can move figure left or right, rotate it, and accelerate falling
5) if figure collides, imprint it into field, go to [1]
6) if not, go to [3]

### Thoughts
* Figure rotation can be unintuitive
* Figure landing can be unintuitive
* Need to render smooth falling, probably

# Features I got for the project
* Keyboard input
* Functions for randomization (C stdlib, Lehmer)
