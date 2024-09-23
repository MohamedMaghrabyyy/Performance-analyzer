#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <stack>

using namespace std;

void insertInstrumentation(const string& inputFilename, const string& outputFilename) 
{
    ifstream inputFile(inputFilename);
    if (!inputFile) {
        cerr << "Error opening input file." << endl;
        return;
    }

    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cerr << "Error opening output file." << endl;
        return;
    }

    // code to insert
    const string bulkCode = R"(#include <iostream>
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
)";

    // Write the bulk code to the output file
    outputFile << bulkCode << endl;

    string line;
    stack<char> braceStack; // Stack to keep track of braces
    bool inFunction = false; // Flag to track if we are in a function
    bool inMain = false; // Flag to track if we are in the main function


        // Regex to match any function definition
    regex functionDefRegex(R"(.*\b\w+\s+\w+\s*\(.*\)\s*(\n\s*)*\{)");

    // Regex to match the main function definition
    regex mainFunctionRegex(R"(int\s+main\s*\(.*\)\s*(\n\s*)*\{)");

    bool flag_func;
    bool flag_main = true;
    while (getline(inputFile, line)) 
    {
        smatch match;
        if (regex_search(line, match, functionDefRegex) && !regex_search(line, match, mainFunctionRegex)) 
        {
            inFunction = true;
            flag_func = true;
        }
        if (regex_search(line, match, mainFunctionRegex))
            inMain = true;
        
        
        for (char ch : line) 
        {
            if (inFunction && !inMain && braceStack.size() == 1 && flag_func) 
            {
              // Insert ENTER_FUNCTION macro immediately after the opening brace
              outputFile << "    ENTER_FUNCTION();" << endl;
              flag_func = false;
            }
            if (inMain && braceStack.size() == 1 && flag_main) 
            {
              // Insert log file open code immediately after the opening brace of main function
              outputFile << "    logFile.open(\"function_profile.txt\");" << endl;
              outputFile << "    if (!logFile) {" << endl;
              outputFile << "        cerr << \"Failed to open log file\" << endl;" << endl;
              outputFile << "        return 1;" << endl;
              outputFile << "    }" << endl;
              outputFile << "    ENTER_FUNCTION();" << endl;
              flag_main = false;
            }
            if (ch == '{') 
              braceStack.push(ch); 
            else if (ch == '}') {
                if (!braceStack.empty()) 
                    braceStack.pop();
                
                if (braceStack.empty()) {
                    if (inFunction && !inMain) 
                    {
                        // Insert EXIT_FUNCTION macro before the last closing brace of a function
                        outputFile << "    EXIT_FUNCTION();" << endl;
                        inFunction = false; 
                    }
                    if (inMain) 
                    {
                        // Insert log file output and closing code before the last closing brace of main
                        outputFile << "    EXIT_FUNCTION();" << endl;
                        outputFile << "    logFile << endl << endl;" << endl;
                        outputFile << "    // Output the inclusive and exclusive times to the log file" << endl;
                        outputFile << "    auto it = inclusiveTime.begin();" << endl;
                        outputFile << "    auto it2 = exclusiveTime.begin();" << endl;
                        outputFile << "    for (; it != inclusiveTime.end() && it2 != exclusiveTime.end(); it++, it2++)" << endl;
                        outputFile << "    {" << endl;
                        outputFile << "        logFile << it->first << \": Inclusive time: \" << it->second << \" , Exclusive time: \" << it2->second << endl;" << endl;
                        outputFile << "    }" << endl;
                        outputFile << "    logFile.close();" << endl;
                        outputFile << "    return 0;" << endl;
                        inMain = false; // Reset main function flag
                    }
                }
            }
        }

        outputFile << line << endl;
    }

    inputFile.close();
    outputFile.close();
}

int main(int argc, char* argv[]) 
{
    if (argc != 3) 
    {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    string inputFilename = argv[1];
    string outputFilename = argv[2];

    insertInstrumentation(inputFilename, outputFilename);

    return 0;
}
