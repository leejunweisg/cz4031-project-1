[OPTION 1] CLion
Requirements: You need to have CLion installed.

1. Open the root folder in CLion.
2. During the first load, CLion should detect the CMake file and display a prompt. Simply accept the defaults.
3. Build and run the project.


[OPTION 2] g++
Requirements: You need to have g++ installed.

Steps to run the program:
1. Open a terminal in the same directory as the source files.

2. Compile the program with the g++ command:
    g++ main.cpp disk.cpp tree.cpp tree_display.cpp tree_insert.cpp tree_remove.cpp tree_search.cpp -o main

3. Run the program.

	Windows:
	$ main.exe

	Unix/Linux:
    $ ./main
