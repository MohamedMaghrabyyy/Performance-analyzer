# C++ Function Profiler

This project is a function profiler written in C++. It tracks and logs the execution time of functions, both inclusive and exclusive, as well as the function call hierarchy. The project is designed for performance analysis and optimization of C++ applications.

## Features
- **Function Call Tracking**: Uses a stack to record function calls and their hierarchical structure.
- **Time Profiling**: Utilizes the `chrono` library to measure both inclusive (total time spent in a function) and exclusive (time excluding sub-function calls) execution times.
- **Real-Time Logging**: Outputs the function call stack and timing information to a log file for further analysis.

## How It Works
- Each function call is logged with the function name and its caller.
- The profiler calculates the time spent in each function using a high-resolution clock and distinguishes between inclusive and exclusive times.
- A log file (`function_profile.txt`) is generated, displaying the function hierarchy and time metrics.

## Files
- `main.cpp`: Contains the source code for the profiler and the simulated workload functions (`a()`, `b()`, `c()`, etc.).
- `function_profile.txt`: The output log file generated during the execution of the program, containing the function call stack and timing details.
