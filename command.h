#ifndef COMMAND_INCLUDED
#define COMMAND_INCLUDED

#define LINE_LENGTH 100
#define MAX_ARGS 7
#define MAX_LENGTH 20
#define MAX_PT_ENTRIES 32

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

int len(std::string str);

std::vector<std::string> split(std::string str, std::vector<std::string> strings);

std::vector<std::string> command(std::string line);

bool checkIsBackground(std::vector<std::string> argv);

std::vector<std::string> getArgs(std::vector<std::string> argv);

std::string getOutputFile(std::vector<std::string> argv);

#endif