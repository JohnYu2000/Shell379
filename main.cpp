#include "main.h"
#include "command.h"
#include "process.h"

using namespace std;

pid_t pid;
string commandLine;
int numOfArgs;
string argc;
vector<string> argv;
map<pid_t, Process> ProcessMap;

void signal_callback_handler(int signum) {
    pid_t pid = waitpid(-1, nullptr, WNOHANG);
    ProcessMap.erase(pid);
}

int main() {
    signal(SIGCHLD, signal_callback_handler);
    signal(SIGINT, signal_callback_handler);  // I included this signal handler when it is absolutely
    // necessary to terminate the process using ctrl+C. An example is invoking the top command where
    // the only way I can think of to terminate it is to invoke ctrl+C. I included this signal so as
    // not to terminate the shell when calling ctrl+C.
    signal(SIGTSTP, SIG_IGN);
    while (1) {
        getline(cin, (cout << "SHELL379: ", commandLine));
        argv = command(commandLine);
        numOfArgs = argv.size();
        argc = argv[0];
        if (argc == "") continue;
        // ############### Exit ############### //
        if (argc == "exit") {
            for (map<pid_t, Process>::iterator iter = ProcessMap.begin(); iter != ProcessMap.end(); iter++) {
                kill(iter->first, SIGKILL);
            }
            ProcessMap.clear();
            printf("Resources used\n");
            int usr_time = get_usr_time();
            int sys_time = get_sys_time();
            printf("User time =      %i seconds\n", usr_time);
            printf("Sys  time =      %i seconds\n", sys_time);
            return 0;
        // ############### Jobs ############### //
        } else if (argc == "jobs") {
            printf("Running processes:\n");
            int i = 0;
            if (!ProcessMap.empty()) {
                printf(" %s     %s %s %s %s\n", "#", "PID", "S", "SEC", "COMMAND");
                for (map<pid_t, Process>::iterator iter = ProcessMap.begin(); iter != ProcessMap.end(); iter++) {
                    Process p = iter->second;
                    char status = p.getStatus();
                    int processTime = p.getProcessTime();
                    printf("%2d  %d %c   %d %s\n", i, iter->first, status, processTime, p.getCmd().c_str());
                    i++;
                }
            }
            printf("Processes =      %i active\n", i);
            printf("Completed processes:\n");
            int usr_time = get_usr_time();
            int sys_time = get_sys_time();
            printf("User time =      %i seconds\n", usr_time);
            printf("Sys  time =      %i seconds\n", sys_time);

        // ############### Kill ############### //
        } else if (argc == "kill") {
            if (numOfArgs < 2) {
                printf("Please specify the PID to kill a process!\n");
            } else if (numOfArgs == 2) {
                pid_t kid;
                sscanf(argv[1].c_str(), "%i", &kid);
                if (ProcessMap.find(kid) == ProcessMap.end()) {
                    printf("Process does not exist!\n");
                    continue;
                } else {
                    ProcessMap.erase(kid);
                }
                kill(kid, SIGKILL);
            } else {
                printf("Too many arguments!\n");
            }
        // ############### Resume ############### //
        } else if (argc == "resume") {
            if (numOfArgs < 2) {
                printf("Please specify the PID to resume a process!\n");
            } else if (numOfArgs == 2) {
                pid_t rid;
                sscanf(argv[1].c_str(), "%i", &rid);
                if (ProcessMap.find(rid) == ProcessMap.end()) {
                    printf("Process does not exist!\n");
                } else {
                    (ProcessMap.find(rid)->second).setStatus('R');
                }
                kill(rid, SIGCONT);
            } else {
                printf("Too many arguments!\n");
            }
        // ############### Sleep ############### //
        } else if (argc == "sleep") {
            if (numOfArgs < 2) {
                printf("Please specify the sleep duration!\n");
            } else if (numOfArgs == 2) {
                unsigned int sleep_time;
                sscanf(argv[1].c_str(), "%i", &sleep_time);
                sleep(sleep_time);
            } else {
                printf("Too many arguments!\n");
            }
        // ############### Suspend ############### //
        } else if (argc == "suspend") {
            if (numOfArgs < 2) {
                printf("Please specify the PID to suspend a process!\n");
            } else if (numOfArgs == 2) {
                pid_t sid;
                sscanf(argv[1].c_str(), "%i", &sid);
                if (ProcessMap.find(sid) == ProcessMap.end()) {
                    printf("Process does not exist!\n");
                } else {
                    (ProcessMap.find(sid)->second).setStatus('S');
                }
            } else {
                printf("Too many arguments!\n");
            }
        // ############### Wait ############### //
        } else if (argc == "wait") {
            pid_t wid;
            sscanf(argv[1].c_str(), "%i", &wid);

            if (ProcessMap.find(wid) == ProcessMap.end()) {
                printf("Process does not exist!\n");
            } else {
                if (waitpid(wid, nullptr, 0) < 0) {
                    perror("Error waiting for process!");
                } else {
                    ProcessMap.erase(wid);  // Process is completed so I remove the process from
                    // the process table.
                }
            }
            
        // ############### Spawn a process ############### //
        } else {
            vector<string> argv_par = getArgs(argv);
            string outputFileName = getOutputFile(argv);
            bool isBackground = checkIsBackground(argv);
            spawn_new_process(&ProcessMap, argv_par, numOfArgs, outputFileName, isBackground);
        }
    }

    return 0;
}