#include<poll.h>
#include<map>
#include<string>
#define POLL_SIZE 1024

namespace frc{
    void int_to_bytes(unsigned short int num, char* buf_begin);
    unsigned short int bytes_to_int(char* buf_start);

    void send_ans(char* request, int fd);

    class Server{
        private:
            unsigned short int current_user;
            char* addr;
            unsigned short port;
            void receaver(char* buffer, int fd, int size);
            struct pollfd poll_set[POLL_SIZE];
            int fds = 0;
            std::string resp;
        public:
            Server(char* ip, unsigned short int p);
            void run(); //Запуск сервера, принимающего запросы и отправляющего ответы
    };
}