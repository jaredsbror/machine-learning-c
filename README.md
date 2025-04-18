# Glowing Umbrella C Project

This project is a simple C program that demonstrates the use of functions and header files. It serves as an example of how to structure a C project using CMake for building.

## Project Structure

```
glowing-umbrella-c
├── src
│   └── main.c        # Contains the main function
├── include
│   └── main.h        # Header file with function declarations
├── CMakeLists.txt    # CMake configuration file
└── README.md         # Project documentation
```

## Building the Project

To build the project, follow these steps:

1. Ensure you have CMake installed on your system.
2. Open a terminal and navigate to the project directory:
   ```
   cd /path/to/glowing-umbrella-c
   ```
3. Create a build directory:
   ```
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```
   cmake ..
   ```
5. Compile the project:
   ```
   make
   ```

## Running the Program

After building the project, you can run the program with the following command:

```
./a.out
```

## Additional Information

- Modify `src/main.c` to change the program logic.
- Add any additional header files in the `include` directory as needed.
- Update `CMakeLists.txt` if you add new source files or libraries.