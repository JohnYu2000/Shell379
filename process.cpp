#include "process.h"

using namespace std;

Process::Process(pid_t pid, char stat, string cmd, bool isBackground) {
    this->pid = pid;
    this->stat = stat;
    this->cmd = cmd;
    this->isBackground = isBackground;
}

pid_t Process::getPid() {
    return this->pid;
}

char Process::getStatus() {
    return this->stat;
}

void Process::setStatus(char stat) {
    this->stat = stat;
}

string Process::getCmd() {
    return this->cmd;
}

int Process::getProcessTime() {
    int processTime;
    char buffer[LINE_LENGTH];
    FILE *fp;
    sprintf(buffer, "ps -p %d -o time=", this->pid);
    fp = popen(buffer, "r");
    fscanf(fp, "%d", &processTime);
    pclose(fp);
    return processTime;
}

bool Process::getIsBackground() {
    return this->isBackground;
}

long get_usr_time() {
    struct rusage *usage;
    getrusage(RUSAGE_CHILDREN, usage);

    return usage->ru_utime.tv_sec;
}

long get_sys_time() {
    struct rusage *usage;
    getrusage(RUSAGE_CHILDREN, usage);

    return usage->ru_stime.tv_sec;
}

void spawn_new_process(map<pid_t, Process> * processMapRef, vector<string> argv, int numOfArgs, string outputFileName, bool isBackground) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Error when starting a new process!");
    } else if (pid == 0) {
        char * argv2[numOfArgs + 1];
        for (int i = 0; i < numOfArgs; i++) {
            argv2[i] = &argv[i][0];
        }
        argv2[numOfArgs] = nullptr;
        int fdd;
        if (outputFileName != "") {
            if ((fdd = open(outputFileName.c_str(), O_CREAT | O_WRONLY, 0644)) < 0) {
                perror("Failed to open output file!");
            }
            dup2(fdd, STDOUT_FILENO);
            close(fdd);
        }
        if (execvp(argv2[0], argv2) < 0) {
            perror("Error when creating a new process!");
            _exit(0);
        }
    } else {
        // bool isBackground = checkIsBackground(argv);
        processMapRef->emplace(pid, Process(pid, 'R', argv[0], isBackground));
        if (!isBackground) {
            waitpid(pid, nullptr, 0);
            processMapRef->erase(pid);
        }
    }
}