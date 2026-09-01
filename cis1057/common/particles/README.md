# Build command

gcc -O2 -std=c11 main_win32.c systems.c input_win32.c -o win_particles.exe -lgdi32 -luser32 -lm