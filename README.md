# 🔐 sloc — source lines of code

**sloc** is a command-line application metric used to measure the number of empty line of codes and comments in a program's source code. 

## 👨‍💻 Authors

This project was developed by José Carlos da Paz Silva [carlos.paz.707@ufrn.edu.br](mailto:carlos.paz.707@ufrn.edu.br) and [leandro.andrade.401@ufrn.edu.br](mailto:leandro.andrade.401@ufrn.edu.br) as part of the *Programação I* course at UFRN.

## 🚀 Compiling and Runnig

### 🛠️ Using cmake

*Note:* This method requires CMake to be installed on your system. CMake is a cross-platform build system generator used to configure and compile the project. If it's not already installed, you can install it using your system’s package manager (e.g., sudo apt install cmake on Debian/Ubuntu or brew install cmake on macOS).

1. Clone this repository:

```bash
git clone https://github.com/selan-active-classes/t03-projeto-sloc-qwerty
```

2. Navigate to the project directory:

```bash
cd t03-projeto-sloc-qwerty
```

3. Compile e construa o programa usando `cmake`:

```bash
cmake -S . -B build
cmake --build build
```

4. Execute:

```bash
./build/sloc <options>
```

### 🛠️ Without cmake

Alternatively, you can compile this project without cmake, manually using C++ compilers, such as c++ or clang, to do so

1. Clone this repository:

```bash
git clone https://github.com/selan-active-classes/t03-projeto-sloc-qwerty
```

2. Navigate to the project directory:

```bash
cd t03-projeto-sloc-qwerty
```

3. Create a `build` directory:

```bash
mkdir build
cd build
```

3. Create a `build` directory:
```
mkdir build
```

4. Compile with `g++`

```
g++ -Wall -Wextra -pedantic -std=c++17 -O3 -I src src/main.cpp -o ./build/sloc
```

or `clang++`

```
clang++ -Wall -Wextra -pedantic -std=c++17 -O3 -I src src/main.cpp -o ./build/sloc
```

5. Run
```
./build/sloc
```
4. Run:

```bash
./sloc <options>
```
---

> [!tip]
> Run **./build/sloc [-h | --help ]** to view all available options and usage instructions.

---

## ✅ Grading

Preencha os itens que você implementou, indicando qual a nota esperada para cada item.

| Item                                                | Valor máximo | Valor esperado |
| --------------------------------------------------- | :----------: | :------------: |
| Read one or multiple source file                    |      10      |                |
| Read a list of files from a directory               |      10      |                |
| Read a list of files recursively from a directory   |      8       |                |
| Show help as requested `-h`                         |      4       |                |
| Treat error situations                              |      10      |                |
| Show correct number of blank lines                  |      5       |                |
| Show correct number of comment lines                |      10      |                |
| Show correct number of doc comment lines            |      10      |                |
| Show correct number of code lines                   |      10      |                |
| Show correct number of total lines                  |      5       |                |
| Show the type of source file                        |      5       |                |
| Show percentages as requested                       |      10      |                |
| Show consolidation at the bottom of a table         |      5       |                |
| Table is formatted as requested                     |      8       |                |
| The project contains at least one struct or classes |      5       |                |
| Overall evaluation of the program                   |      5       |                |

## ✨ Extras

> TODO

## ⚠️ Problems found or limitations

> TODO

---

&copy; DIMAp | Departamento de Informática e Matemática Aplicada (2016 - 2025)
