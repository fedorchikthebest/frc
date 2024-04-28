#include<poll.h>
#include<map>
#include<string>
#define POLL_SIZE 1024

namespace frc{
    class Server{
        private:
            char* addr;
            unsigned short port;
            int receaver(char* buffer, int fd, int size);
            struct pollfd poll_set[POLL_SIZE];
            int fds = 0;
        public:
            Server(char* ip, unsigned short int p);
            void run(); //Запуск сервера, принимающего запросы и отправляющего ответы
    };
}