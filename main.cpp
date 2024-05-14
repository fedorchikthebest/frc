#include "server.h"
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include<error.h>
#include<fcntl.h>
#define BD_MAX_CLOSE  8192

int main(int argc, char *argv[])
{
    if (argc < 2) return -1;

    int maxfd, fd;
    char curr_dir[128] = {0}, config_file[256] = {0};
    /* Превращение в фоновый процесс */
    switch (fork())
    {
    case -1:
        return -1;
    case 0:
        break; //  Потомок продолжает выполнение,
    default:
        exit(EXIT_SUCCESS); //  а родитель завершается
    }

    /* Процесс становится лидером новой сесии */
    if (setsid() == -1)
        return -1;

    /* Второй fork() чтобы процесс не стал лидером новой сессии */
    switch (fork())
    {
    case -1:
        return -1;
    case 0:
        break;
    default:
        exit(EXIT_SUCCESS);
    }

    /* Сбрасывание маски режима создания файлов */
    umask(0);

    /* Очищаем окружение */
    clearenv();

    /* Закрытие всех открытых файлов */
    maxfd = sysconf(_SC_OPEN_MAX);
    if (maxfd == -1)
        maxfd = BD_MAX_CLOSE;
    for (fd = 0; fd < maxfd; fd++)
        close(fd);

    /* Перенаправляем стандартные потоки данных в /dev/null для исключения работы printf() и т.д. */
    close(STDIN_FILENO);
    fd = open("/dev/null", O_RDWR);
    if (fd == -1)
        return -1;
    if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
        return -1;
    if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
        return -1;
    
    frc::Server srv(argv[1], 5050);
    srv.run();
    return 0;
}