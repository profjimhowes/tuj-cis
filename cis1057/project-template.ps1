# Project Template Script
# Run this in PowerShell to create a complete C project structure

param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectName
)

# Validate project name (basic alphanumeric and underscore check)
if ($ProjectName -notmatch '^[a-zA-Z][a-zA-Z0-9_]*$') {
    Write-Error "Project name must start with a letter and contain only letters, numbers, and underscores."
    exit 1
}

if (Test-Path $ProjectName) {
    Write-Host "Directory $ProjectName already exists. Remove it first or choose a different name." -ForegroundColor Red
    exit 1
}

# Create main project directory
Write-Host "Creating '$ProjectName' project structure..." -ForegroundColor Cyan

New-Item -ItemType Directory -Path $ProjectName
Set-Location $ProjectName

# Create main directory structure
$directories = @(
    "include",
    "include/$ProjectName",
    "src",
    "examples",
    "tests",
    "docs",
    "data"
)

foreach ($dir in $directories) {
    New-Item -ItemType Directory -Path $dir -Force
    Write-Host "Created: $dir" -ForegroundColor Green
}

# Create main CMakeLists.txt
$mainCMake = @"
cmake_minimum_required(VERSION 3.16)
project($ProjectName VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(include)

# Build options
option(BUILD_EXAMPLES "Build example programs" ON)
option(BUILD_TESTS "Build test programs" ON)

# Add subdirectories
add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
"@

$mainCMake | Out-File -FilePath "CMakeLists.txt" -Encoding UTF8
Write-Host "Created main CMakeLists.txt" -ForegroundColor Green

# Create README.md
$readme = @"
# $ProjectName
An example project for the TUJ course CIS 1057: Computer Programming in C.

## License

GNU GPLv3 License - see LICENSE file in parent repository for details.
"@

$readme | Out-File -FilePath "README.md" -Encoding UTF8
Write-Host "Created README.md" -ForegroundColor Green

Write-Host "`n$ProjectName project structure created successfully!" -ForegroundColor Cyan