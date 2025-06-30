# CIS 1057: Computer programming in C
Lectures notes, example code, and project files for introductory C programming course.

# Project guide
Please refer to this section for guidance on how to seamlessly collaborate on your project with your teammates.

## Source control
All projects are contained in a single repository, but the code for each is confined to a branch specific to that project.
- `cis1057-kabu`: Financial trading platform
- `cis1057-tancho`: Satellite telemetry and navigation
- `cis1057-teban`: Turn-based game engine

### Initial Setup

After forking the course repository on GitHub:

```bash
# Step 1: Clone your fork
git clone https://github.com/your-username/your-repo-name.git
cd your-repo-name

# Step 2: Add course repo as upstream
git remote add upstream https://github.com/profjimhowes/tuj-cis.git

# Step 3: Verify remotes
git remote -v
```

You should see:
- `origin` → your fork
- `upstream` → course repo

### Staying Up-to-date
```bash
# Step 1: Fetch Latest Changes
git fetch upstream

# Step 2: Switch to Project Branch
git checkout cis1057-projectname

# Step 3: Pull Upstream Changes
git pull upstream cis1057-projectname

# Step 4: Push to Your Fork
git push origin cis1057-projectname
```

### Handling Merge Conflicts

Sometimes pulling changes from upstream (step 3 above) will conflict with your work and you need to manually resolve the merge process.

```bash
git pull upstream cis1057-projectname
# Git will show conflict messages

# Edit conflicted files (look for <<<<<<< markers)
# Remove conflict markers and choose the correct code

# After fixing conflicts:
git add .
git commit -m "Merge upstream changes"
```