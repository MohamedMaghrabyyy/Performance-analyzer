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

#include <iostream>
using namespace std;

int count[10];
string paths[] = {"main calls square", "main calls f1 calls square", "main calls f2 calls square", "", "main calls cube", "main calls f1 calls cube", "main calls f2 calls cube", "", "", ""};
int r;
int square(int x);
int f1(int a, int b);
int cube(int x);
int f2 (int a, int b);

int main()  {
    logFile.open("function_profile.txt");
    if (!logFile) {
        cerr << "Failed to open log file" << endl;
        return 1;
    }
    ENTER_FUNCTION();
  for (int k = 0; k < 10; k++)
    count[k] = 0;
  r = 0;
  for (int i = 1; i < 100; i++)
  {
    int s = square(i);
    cout << i << " " << s;
    int c = cube(i);
    cout << " " << c << endl;
  }
  for (int i = 1; i < 50; i++)
  {
    int s = square(i);
    cout << i << " " << s << endl;
  }
  for (int j = 1; j < 50; j++)
  {
    f1(j, j + 1);
    if (j > 6)
      f2(j, j + 1);
  }
  cout << "\nResults" << endl;
  for (int k = 0; k < 10; k++)
    cout << k << " " << paths[k] << " " << count[k] << endl;

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
int square(int x) {
    ENTER_FUNCTION();
  count[r]++;
  return x * x;
    EXIT_FUNCTION();
}

int cube(int x) {
    ENTER_FUNCTION();
  r = r + 4;
  count[r]++;
  r = r - 4;
  return x * x * x;
    EXIT_FUNCTION();
}

int f1(int a, int b) {
    ENTER_FUNCTION();
  r = r + 1;
  for (int i = 1; i <= 5; i++)
  {
    int s1;
    s1 = square(i);
    int c1;
    c1 = cube(i);
    cout << s1 + c1;
  }
  r = r - 1;
  return 0;

    EXIT_FUNCTION();
}

int f2(int a , int b) {
    ENTER_FUNCTION();
  r = r + 2;
  for (int i = 1; i <= 5; i++)
  {
    int s1 = square(i);
    int c1 = cube(i);
    int c2 = cube(i);
    cout << s1 + c1 + c2;
  }
  r = r - 2;
  return 0;
    EXIT_FUNCTION();
}
