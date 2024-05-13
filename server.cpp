#include"server.h"
#include<netinet/in.h> 
#include<sys/socket.h> 
#include<arpa/inet.h>
#include<iostream>
#include<poll.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#define CHAR_MASK 0x11111111
#define BUFFER_SIZE 5120

std::map<std::string, std::string> spravochnik = {
    {"rekursion",
    "def f(n, k):\n"
    "   if n == k\n:"
    "       return 1\n"
    "   if n > k or n == 32:\n"
    "       return 0\n"
    "   return f(n + 3, k) + f(n + 4, k) + f(n * 3, k)\n"
    "print(f(4, 16) * f(16, 46))"},
    {"single",
    "import struct\n"
    "b = struct.pack('f', -21.25)\n"
    "# f - single, d - double\n"
    "print(b)\n"
    "ans = ''\n"
    "for el in b[::-1]:\n"
    "    ans += hex(el)[2:].rjust(2, '0') + ' '\n"
    "print(ans)"},
    {"list", "-rekursion\n-single"}
};

frc::Server::Server(char* ip, unsigned short int p){
    addr = ip;
    port = p;
    std::cout << addr << std::endl;
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

    listen(serverSocket, 5);

    poll_set[0].fd = serverSocket;
    poll_set[0].events = POLLIN;
    ++fds;
    while (1){
        status = poll(poll_set, fds, -1);
        // перебор событий
        for (int i = 0; i < fds; i++){
            if (poll_set[i].revents & POLLIN){
                //Если событие с сокетом сервера, то одобряем подключение
                if (poll_set[i].fd == serverSocket){
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
                    buffer_size = recv(poll_set[i].fd, &buffer, BUFFER_SIZE, 0);
                    // получаем отправленные клиентом данные
                    if(buffer_size > 0 && buffer_size < BUFFER_SIZE){
                        buffer[buffer_size] = '\0';
                        receaver(buffer, poll_set[i].fd, buffer_size);
                    }
                    else{
                        close(poll_set[i].fd);
                        poll_set[i].fd = -1;
                        if (i == fds - 1) fds--;
                    }
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
        for (unsigned short int i = 1; i < fds; i++){
            if (poll_set[i].fd != fd){
                send(poll_set[i].fd, buffer, size, 0);
            }
        }
        break;
        case 1:
            send(fd, spravochnik.at((std::string) &buffer[1]).c_str(), spravochnik.at((std::string) &buffer[1]).size(), 0);
            break;
        case 2:
            current_user = bytes_to_int(&buffer[1]);
            int_to_bytes(fd, &buffer[1]);
            std::cout << &buffer[3] << std::endl;
            for (unsigned short int i = 1; i < fds; i++){
                if (poll_set[i].fd == current_user){
                    send(poll_set[i].fd, buffer, size, 0);
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
