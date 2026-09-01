# Turmite visualizer
This program generates a random "turmite"--a 2-dimensional Turing machine--and plots the output in a window.
It can be compiled and run on Windows, Mac, or Linux.

## Compilation
Run the following commands to compile and link the source code:

- Windows: `gcc main.c turmite.c -lgdi32 -o turmite`
- Mac: `gcc main.c turmite.c -framework Cocoa -o turmite`
- Linux: `gcc main.c turmite.c -lX11 -o turmite`

## Usage
You can specify the number of states and symbols for the generated Turmites with command-line arguments.
For example, `./turmite 4 4` will run the program with 4 states and 4 symbols.
The defaults (with no command-line arguments) are 3 states and 5 symbols.

## Notable Turmites
"The weaver": 35100401401300230222021312032412101301201401021
"Slow triangle factory": 35200332421402011231121301211101022102300410410
"Growing cubes": 35402022331432012432231132112410132302331112232