#ifndef PROCESS_INCLUDED
#define PROCESS_INCLUDED

#define LINE_LENGTH 100
#define MAX_ARGS 7
#define MAX_LENGTH 20
#define MAX_PT_ENTRIES 32

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <string>
#include <map>
#include <fcntl.h>

#include "command.h"

class Process {
    pid_t pid;
    char stat;
    std::string cmd;
    bool isBackground;

    public:
        Process(pid_t pid, char stat, std::string cmd, bool isBackground);

        pid_t getPid();

        char getStatus();

        void setStatus(char stat);

        std::string getCmd();

        int getProcessTime();

        bool getIsBackground();
};

long get_usr_time();

long get_sys_time();

void spawn_new_process(std::map<pid_t, Process> * processMapRef, std::vector<std::string> argv, int numOfArgs, std::string outputFileName, bool isBackground);

#endif