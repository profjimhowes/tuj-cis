# ---- configuration ----
CC      = gcc
CFLAGS  = -O2 -std=c11
LDFLAGS = -lgdi32 -luser32 -lm

TARGET  = particles.exe
SRC     = main.c sim.c app/win_app.c

# ---- rules ----
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	del $(TARGET)
