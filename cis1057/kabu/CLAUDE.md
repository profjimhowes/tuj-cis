# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

- `make` - Build the project (creates `bin/kabu` executable)
- `make clean` - Remove build artifacts
- `make test` - Run tests (not yet implemented)

## Project Structure

- `src/` - C source files
- `include/kabu/` - Header files
- `obj/` - Object files (created during build)
- `bin/` - Executable output (created during build)

## Version Control

- The `cis1057-kabu` branch is considered the root for this directory. Changes to files in this folder and subfolders shall never be merged into any branch other than `cis1057-kabu`.