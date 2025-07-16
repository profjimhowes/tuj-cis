# Get all .c files without extension
SOURCES = $(wildcard *.c)
BINARIES = $(SOURCES:.c=)

# Add .exe extension on Windows
ifeq ($(OS),Windows_NT)
    EXECUTABLES = $(BINARIES:=.exe)
else
    EXECUTABLES = $(BINARIES)
endif

clean:
ifeq ($(OS),Windows_NT)
	@for %%f in ($(EXECUTABLES)) do if exist "%%f" del "%%f"
else
	@rm -f $(EXECUTABLES)
endif

.PHONY: clean