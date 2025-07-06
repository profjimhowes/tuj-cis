# CIS 1057: Computer programming in C
Lecture notes, example code, and project files for introductory C programming course.

## Environment setup

### PowerShell script
The following commands can be used to setup PowerShell to automatically locate your compiler and make programs.
```bash
$Env:Path += ';C:\TDM-GCC-64\bin'
$Env:CC = 'gcc'
Set-Alias -Name make -Value mingw32-make
```

## Projects
The projects are intended to mimic real-world C codebases at a level that is approachable for introductory students.
### Kabu
A high-performance trade matching and settlement engine, with support for user trading account management.
### Tancho
A satellite telemetry monitoring system, with support for issuing command and control orders.
### Teban
An extensible turn-based game engine which is powerful and flexible enough to implement complicated tabletop games.

# Source control
All projects are contained in a single repository, but the code for each is confined to a branch specific to that project.
- `cis1057-kabu`: Financial trading platform
- `cis1057-tancho`: Satellite telemetry and navigation
- `cis1057-teban`: Turn-based game engine

## Initial Setup

After forking the course repository on GitHub:, you need to clone it to your local device.
>⚠️ You only need to do this once! Do not clone the repository multiple times!

### Step 1. Clone your fork
> ⛔️ **DO NOT JUST COPY-PASTE THESE COMMANDS.**

> ⚠️ Replace *YOUR-USERNAME* with your GitHub username, and *YOUR-REPO-NAME* with the name of your forked repo.
```bash
git clone https://github.com/YOUR-USERNAME/YOUR-REPO-NAME.git
cd YOUR-REPO-NAME
```

### Step 2. Add course repo as upstream
Enter this command exactly as you see it below. Copy-paste is okay for this one.
```bash
git remote add upstream https://github.com/profjimhowes/tuj-cis.git
```

### Step 3. Verify remotes
Enter this command:
```bash
git remote -v
```
And you should see something like this:
```bash
origin  https://github.com/tujstudent/tuj-cis (fetch)
origin  https://github.com/tujstudent/tuj-cis (push)
upstream    https://github.com/profjimhowes/tuj-cis (fetch)
upstream    https://github.com/profjimhowes/tuj-cis (push)
```
If you can see both the `origin` and `upstream` repositories listed, then you've successfully set up your local repository for ongoing updates! 🎉

## Staying Up-to-date

```bash
# Step 1: Fetch Latest Changes
git fetch upstream

# Step 2: Switch to Project Branch
git switch cis1057-PROJECTNAME

# Step 3: Pull Upstream Changes
git pull upstream cis1057-PROJECTNAME

# Step 4: Push to Your Fork
git push origin cis1057-PROJECTNAME
```

## Handling Merge Conflicts

Sometimes pulling changes from upstream (step 3 above) will conflict with your work and you need to manually resolve the merge process.

```bash
git pull upstream 
# Git will show conflict messages

# Edit conflicted files (look for <<<<<<< markers)
# Remove conflict markers and choose the correct code

# After fixing conflicts:
git add .
git commit -m "Merge upstream changes"
```