#include"server.h"
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<iostream>
#include <fstream>
#include<string.h>
#include <string>
#include <signal.h>

int daemon(char* ip){
    frc::Server srv(ip, 5050);
    srv.run();
}

int main(int argc, char *argv[]){
    if (argc < 2){
        std::cout << "no arguments";
        exit(-1);
    }
    if (!strcmp(argv[1], "kill")){
        std::ifstream in("pid.txt");
        if (in.is_open()){
            std::string line;
            while (std::getline(in, line))
            {
                kill(atoi(line.c_str()), SIGKILL);
            }
            in.close();
        }
        std::cout << "process killed";
        exit(0);
    }
    pid_t pid, sid;;
    pid = fork();
    if (pid == 0){
        sid = setsid();
        std::ofstream out;
        out.open("pid.txt"); 
        if (out.is_open()){
            out << sid << std::endl;
            out.close();
        }
        else exit(-1);
        daemon(argv[1]);
    }
    else{
        exit(0);
    }
    return 0;
}