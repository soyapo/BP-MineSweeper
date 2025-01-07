# MineSweeper

## Introduction

MineSweeper is an American video game published in the 1990s by Microsoft. It is one of the longest-running video games in history. For my mid-term project in the *Basic Programming* course, I aimed to build a simple and functional version of the game.

## Project Plans

### Project Declaration:

As mentioned earlier, my goal was to create a simplified version of MineSweeper using pure C++ and implementing graphics with Unicode and ASCII characters.

### Project Goals:

The main objective of this project was to implement all the features of the original game while adding a touch of creativity, without compromising the speed and functionality of the program. Additionally, it was crucial to meet all the requirements set by the judges in order to achieve the highest score among the other projects; Including but not limited to not using any advanced data structures, adding addiotional features and having a leaderboard.

### Additional Features:

- Difficulity Levels
- Custom Colors
- Leaderboard and scoring system 

## Implementations

### Game Behaviour

A grid is generated based the player's chosen difficulity, and the game starts. The first chosen cell is always empty, and bombs are placed with respect to that.
The Player can navigate the grid and mark it as Flag or Clear, losing if a bomb is mistakenly marked as clear. 

### Graphics

All the visuals in the game consist of ASCII art, which I modified from existing sources to suit the game's aesthetic. Mainly, Unicode characters from the *Box Drawing* block have been used.

### Saving Data

To track scores, I implemented a mechanism to write and read data from a **.txt** file. The data is stored in a text file, which is utilized to construct the leaderboard as needed.

### Compilation 

To compile the program, follow these steps:

1. Ensure you have a C++ compiler installed on your system. Popular options include GCC (GNU Compiler Collection) for Linux and MinGW (Minimalist GNU for Windows) for Windows.

1. Open a terminal or command prompt and navigate to the directory containing the source code files.

1. Run the following command to compile the program:

   ```shell
   g++ game.cpp -o game.exe
   ```

   Replace `g++` with the appropriate compiler command if you are using a different compiler.

1. Once the compilation process completes without any errors, an executable file named `game` (or the name you specified after the `-o` option) will be created in the current directory.

   - On Windows, the file will have the `.exe` extension (`game.exe`).
   - On Linux, the file will have no extension (`game`).

1. You can now run the game by executing the generated executable from the terminal or by double-clicking it in the file explorer.

   ```shell
   ./game      # Linux
   game.exe    # Windows
   ```

It's worth noting that the compilation process may vary depending on your operating system and the specific compiler you are using. Make sure to adjust the compilation commands accordingly.

## Future Plans

This game possesses substantial potential for further growth and improvement. 
Some potential future enhancements include:
1- Adding a graphical user interface (GUI)
2- Adding music to the game
3- Adding the ability for custom table size and bomb count
4- Adding a AutoSolver algorithm

## Credits

Course Instructor: 
[Dr. Kheradpisheh](https://www.linkedin.com/in/saeed-reza-kheradpisheh-7a0b18155/)  

Project Mentors: 
[Mehrdad Shirvani](https://www.linkedin.com/in/mehrdad-shirvani/)
and
[Farid Karimi](https://www.linkedin.com/in/farid-kmi/)
=======
Classic Minesweeper game designed for Terminal, written in pure c++.
>>>>>>> 0e8f49ad1901d70b07571c77c6f05b4daba3fe07
