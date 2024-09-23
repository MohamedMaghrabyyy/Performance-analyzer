#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <map>
#include <chrono>

using namespace std;
using namespace chrono;

// Macros for entering and exiting functions
#define ENTER_FUNCTION() pushFunction(__FUNCTION__)
#define EXIT_FUNCTION() popFunction()

ofstream logFile;

// Function stack for tracking function calls
stack<string> functionStack;

map<string, double> inclusiveTime;
map<string, double> exclusiveTime;

// Stack to track start times for calculating exclusive times
stack<steady_clock::time_point> startTimes;

void printStack() 
{
    logFile << "CURRENT STACK:" << endl;
    stack<string> tempStack = functionStack;

    if (tempStack.empty()) {
        logFile << "Stack is empty!" << endl;
        return;
    }

    // Reverse the stack for display purposes
    stack<string> reversedStack;
    while (!tempStack.empty()) 
    {
        reversedStack.push(tempStack.top());
        tempStack.pop();
    }

    while (!reversedStack.empty()) 
    {
        logFile << "  " << reversedStack.top();
        reversedStack.pop();
        if (!reversedStack.empty())
            logFile << " --> ";
    }
    logFile << endl << endl << endl;
}

void pushFunction(const string &functionName) 
{
    if (!functionStack.empty())
        logFile << functionStack.top() << " calls " << functionName << endl;

    functionStack.push(functionName);
    startTimes.push(steady_clock::now());
    printStack();
}

void popFunction() 
{
    if (!functionStack.empty()) 
    {
        string functionName = functionStack.top();
        functionStack.pop();

        // Calculate the elapsed time
        auto endTime = steady_clock::now();
        double elapsedTime = duration_cast<duration<double, micro>>(endTime - startTimes.top()).count();
        startTimes.pop();

        // Update inclusive time
        inclusiveTime[functionName] = elapsedTime;

        // Update exclusive time
        if (!functionStack.empty()) 
        {
            string parentFunction = functionStack.top();
            exclusiveTime[parentFunction] -= elapsedTime;  // Deduct child time from parent
        }
        exclusiveTime[functionName] += elapsedTime;

        logFile << "EXIT " << functionName << endl;
        printStack(); // Print stack after popping
    }
}



void a();
void b();
void c();
void d();
void e();

void a() {
    ENTER_FUNCTION();
    
    for (int i = 0; i < 100000; i++) {
        // Simulate work
    }
    b();
 
    EXIT_FUNCTION();
}

void b() {
    ENTER_FUNCTION();
    
    for (int i = 0; i < 200000; i++) {
        // Simulate work
    }
    c();
    
    EXIT_FUNCTION();
}

void c() {
    ENTER_FUNCTION();
 
    for (int i = 0; i < 400000; i++) {
        // Simulate work
    }
    d();
   
    EXIT_FUNCTION();
}

void d() {
    ENTER_FUNCTION();
   
    for (int i = 0; i < 800000; i++) {
        // Simulate work
    }
    e();
    
    EXIT_FUNCTION();
}

void e() {
    ENTER_FUNCTION();
 
    for (int i = 0; i < 1600000; i++) {
        // Simulate work
    }
    
    EXIT_FUNCTION();
}

int main() {
    logFile.open("function_profile.txt");
    if (!logFile) {
        cerr << "Failed to open log file" << endl;
        return 1;
    }
    ENTER_FUNCTION();
    
    a();
    for (int i = 0; i < 1600000; i++) {
        // Simulate work
    }
    
    b();
    
    EXIT_FUNCTION();
    logFile << endl << endl;
    // Output the inclusive and exclusive times to the log file
    auto it = inclusiveTime.begin();
    auto it2 = exclusiveTime.begin();
    for (; it != inclusiveTime.end() && it2 != exclusiveTime.end(); it++, it2++)
    {
        logFile << it->first << ": Inclusive time: " << it->second << " , Exclusive time: " << it2->second << endl;
    }
    logFile.close();
    return 0;
}
