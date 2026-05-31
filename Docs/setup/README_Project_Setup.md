# Case Conversion API

This project is a REST API for **word case conversion** (uppercase, lowercase, camelcase, etc.), following Microsoft-style folder structure.  

This README captures **all steps from creating the repo to generating the folder structure**.

---

## 1. Create a new GitHub repository

1. Go to [GitHub → New Repository](https://github.com/new).
2. Repository name: `case-conversion-api`
3. Keep it empty (do **not** add README/license/.gitignore).

---

## 2. Initialize local repository

### Create project folder

```bash
mkdir case-conversion-api
cd case-conversion-api
```

### Initialize Git

```bash
git init
```

## 3. Add a README file

```Bash
echo "# Case Conversion API" > README.md
git add README.md
git commit -m "Initial commit"
```

### Add remote and push

Replace 'username' with your GitHub username:

```Bash
git remote add origin https://github.com/<username>/case-conversion-api.git
git push -u origin main
```

Ensure your GitHub username and repo URL are correct.

## Create folder structure script

Create a file create_ms_api_structure.sh:

```Bash
touch create_ms_api_structure.sh
```

Add the following content to it:

```Bash
#!/bin/bash
# Script to create Microsoft-style folder structure for CaseConversionAPI

ROOT="CaseConversionAPI"

echo "Creating Microsoft-style folder structure for CaseConversionAPI..."

# -----------------------------
# C++ library project
# -----------------------------
mkdir -p $ROOT/CppLib/include
mkdir -p $ROOT/CppLib/src

# -----------------------------
# .NET API project
# -----------------------------
mkdir -p $ROOT/DotNetAPI/Controllers
mkdir -p $ROOT/DotNetAPI/Models
mkdir -p $ROOT/DotNetAPI/Services

# Placeholder .NET files
touch $ROOT/DotNetAPI/Program.cs
touch $ROOT/DotNetAPI/Startup.cs
touch $ROOT/DotNetAPI/DotNetAPI.csproj
touch $ROOT/DotNetAPI/Controllers/WordCaseController.cs

# -----------------------------
# DLL output folder
# -----------------------------
mkdir -p $ROOT/lib

# -----------------------------
# Tests
# -----------------------------
mkdir -p $ROOT/tests/CppTests
mkdir -p $ROOT/tests/DotNetTests

# -----------------------------
# Scripts
# -----------------------------
mkdir -p $ROOT/scripts
touch $ROOT/scripts/create_structure.sh

# -----------------------------
# Root files
# -----------------------------
touch $ROOT/README.md
touch $ROOT/CMakeLists.txt

echo "Microsoft-style folder structure created under $ROOT!"
```

## 6. Make script executable

```Bash
chmod +x create_ms_api_structure.sh
```

## 7. Run the script

```BAsh
./create_ms_api_structure.sh
```

After running, the folder structure will look like:

```Bash
CaseConversionAPI/
├── CppLib/
│   ├── include/
│   └── src/
├── DotNetAPI/
│   ├── Controllers/
│   │   └── WordCaseController.cs
│   ├── Models/
│   ├── Services/
│   ├── Program.cs
│   ├── Startup.cs
│   └── DotNetAPI.csproj
├── lib/
├── scripts/
│   └── create_structure.sh
├── tests/
│   ├── CppTests/
│   └── DotNetTests/
├── README.md
└── CMakeLists.txt
```

## Full Local CI Steps in VS Code

### 1. Install Docker Desktop

```Bash
Download from: https://www.docker.com/products/docker-desktop/
Install it.
Start Docker Desktop manually once. You should see the whale icon in the menu bar.
```

### 2. Install VS Code Docker Extension

```Bash
Open VS Code
Go to Extensions (Cmd+Shift+X)
Search for Docker by Microsoft → Install
Reload VS Code
```

You can now manage containers, images, volumes directly from VS Code.

### 3. Verify Docker is Running

Open a terminal inside VS Code:

```Bash
docker info
```

Expected: Shows server info, containers, images.
If it fails: Start Docker Desktop or run:

```Bash
open -a Docker
```

This will launch Docker Desktop.

### 4. Install act to Run GitHub Actions Locally

```Bash
brew install act
```

This allows you to simulate GitHub Actions jobs on your Mac.

### 5. Create Your Workflow File (if not done)

```Bash
mkdir -p .github/workflows && touch .github/workflows/cpp-ci.yml
```

mkdir -p .github/workflows && touch .github/workflows/cpp-ci.yml

### 6. Run CI Locally Using act

Run CI Locally Using act

```Bash
act push -j build-and-test --container-architecture linux/amd64
```

- push → triggers the workflow as if you pushed to GitHub
- -j build-and-test → runs the job named build-and-test
- --container-architecture linux/amd64 → ensures compatibility with your Mac.

### 7. Monitor Logs

Logs appear in VS Code terminal.
You’ll see all steps: checkout, install compiler, CMake configure, build, run app, run tests.
