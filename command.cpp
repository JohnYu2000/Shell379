#include "command.h"

using namespace std;

static vector<string> argv;

int len(string str)  
{  
    int length = 0;  
    for (int i = 0; str[i] != '\0'; i++)  
    {  
        length++;  
    }  
    return length;     
}  

vector<string> split(string str, vector<string> strings) {  
    int currIndex = 0, i = 0;  
    int startIndex = 0, endIndex = 0;  
    while (i <= len(str))  
    {  
        if (str[i] == ' ' || i == len(str))  
        {  
            endIndex = i;  
            string subStr = "";  
            subStr.append(str, startIndex, endIndex - startIndex);  
            strings.push_back(subStr);  
            currIndex += 1;  
            startIndex = endIndex + 1;  
        }  
        i++;  
        }
    return strings;
}  

vector<string> command(string line) {
    argv.clear();
    argv = split(line, argv);
    return argv;
}

bool checkIsBackground(vector<string> argv) {
    if (argv.size() < 2) return false;
    string lastArg = argv[argv.size() - 1];
    if (lastArg == "&") {
        return true;
    } else {
        return false;
    }
}

vector<string> getArgs(vector<string> argv) {
    vector<string> argv2;
    for (int i = 0; i < argv.size(); i++) {
        if (argv[i] == "&") {
            continue;
        }
        // I tested out execvp and it appears that execvp performs normally
        // when provided an input file. To simplify my code I have decide to
        // just have the input file name as an argument parameter for execvp.
        if (argv[i][0] == '<') {
            argv2.push_back(argv[i].substr(1));
            continue;
        }
        if (argv[i][0] == '>') {
            continue;
        }
        argv2.push_back(argv[i]);
    }
    return argv2;
}

string getOutputFile(vector<string> argv) {
    for (int i = 0; i < argv.size(); i++) {
        if (argv[i][0] == '>') {
            return argv[i].substr(1);
        }
    }
    return "";
}
