#include"server.h"
#include<netinet/in.h> 
#include<sys/socket.h> 
#include<arpa/inet.h>
#include<poll.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include<iostream>
#include <stdio.h> 
#define KEYS_COUNT 10
#define BUFFER_SIZE 5120
#define CLIENT_SIZE 2048

char client_code[CLIENT_SIZE];
int client_code_size;


frc::Server::Server(char* ip, unsigned short int p){
    addr = ip;
    port = p;

    FILE *fp = fopen("client.py", "r");

    if (fp){
        client_code_size = fread(client_code, sizeof(client_code[0]), CLIENT_SIZE, fp);
    }
    fclose(fp);
}

void frc::Server::run(){
    int clientSocket;
    int serverSocket;
    char buffer[BUFFER_SIZE] = { 0 }; 
    int buffer_size;
    bool closed_found = false;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    int status;

    // specifying the address 
    sockaddr_in serverAddress; 
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(port); 
    serverAddress.sin_addr.s_addr = inet_addr(addr); 
    status = bind(serverSocket, (struct sockaddr*)&serverAddress, 
        sizeof(serverAddress));
    
    if (status < 0){
        exit(-1);
    }

    listen(serverSocket, 5);

    poll_set[0].fd = serverSocket;
    poll_set[0].events = POLLIN;
    ++fds;
    while (1){
        status = poll(poll_set, fds, -1);
        // перебор событий
        for (fd_index = 0; fd_index < fds; fd_index++){
            if (poll_set[fd_index].revents & POLLIN){
                //Если событие с сокетом сервера, то одобряем подключение
                if (poll_set[fd_index].fd == serverSocket){
                    clientSocket = accept(serverSocket, nullptr, nullptr);
                    for (unsigned short int j = 0; j < fds; j++){
                        if (poll_set[j].fd == -1){
                            poll_set[j].fd = clientSocket;
                            poll_set[j].events = POLLIN;
                            closed_found = true;
                        }
                    }
                    if (fds < POLL_SIZE && !closed_found){
                        poll_set[fds].fd = clientSocket;
                        poll_set[fds].events = POLLIN;
                        fds++;
                    }
                    closed_found = false;
                }
                else {
                    buffer_size = recv(poll_set[fd_index].fd, &buffer, BUFFER_SIZE, 0);
                    // получаем отправленные клиентом данные
                    if(buffer_size > 0 && buffer_size < BUFFER_SIZE){
                        buffer[buffer_size] = '\0';
                        receaver(buffer, poll_set[fd_index].fd, buffer_size);
                    }
                    else close_fd();
                }
            }
        }
    }
}

 void frc::Server::receaver(char* buffer, int fd, int size){
    switch (buffer[0])
    {
    case 0:
        int_to_bytes(fd, &buffer[1]);
        for (unsigned short int j = 1; j < fds; j++){
            if (poll_set[j].fd != fd){
                send(poll_set[j].fd, buffer, size, 0);
            }
        }
        break;
        case 1:
            send_ans(buffer, fd);
            break;
        case 2:
            current_user = bytes_to_int(&buffer[1]);
            int_to_bytes(fd, &buffer[1]);
            for (unsigned short int j = 1; j < fds; j++){
                if (poll_set[j].fd == current_user){
                    send(poll_set[j].fd, buffer, size, 0);
                }
        }
    }
 }


void frc::int_to_bytes(unsigned short int num, char* buf_begin){
    std::copy(static_cast<const char*>(static_cast<const void*>(&num)),
          static_cast<const char*>(static_cast<const void*>(&num)) + sizeof(num),
          buf_begin);
} 

 
unsigned short int frc::bytes_to_int(char* buf_begin){
    unsigned short int ans;
    memcpy(&ans, buf_begin, sizeof(unsigned short int));
    return ans;
}


void frc::send_ans(char* request, int fd){
    switch (request[1])
    {
    case 'l': // list
        send(fd, "recursion\nsingle\ncode", 22, 0);
        break;
    case 's': //single
        send(fd, "import struct\n"
            "b = struct.pack('f', -21.25)\n"
            "# f - single, d - double\n"
            "print(b)\n"
            "ans = ''\n"
            "for el in b[::-1]:\n"
            "    ans += hex(el)[2:].rjust(2, '0') + ' '\n"
            "print(ans)", 159, 0);
    case 'r': //recursion
        send(fd,
            "def f(n, k):\n"
            "   if n == k:\n"
            "       return 1\n"
            "   if n > k or n == 32:\n"
            "       return 0\n"
            "   return f(n + 3, k) + f(n + 4, k) + f(n * 3, k)\n"
            "print(f(4, 16) * f(16, 46))", 161, 0);
    case 'c': //client_code
        send(fd, client_code, client_code_size, 0);
        break;
    default:
        send(fd, "Not found, try list", 20, 0);
        break;
    }
}


void frc::Server::close_fd(){
    close(poll_set[fd_index].fd);
    poll_set[fd_index].fd = -1;
    if (fd_index == fds - 1) fds--;
}