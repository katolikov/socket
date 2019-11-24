#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>
#define BUF_SIZE 256

int day_num(int n){
  int day = n % 7;
  switch (day){
     case 2:
        printf("\033[1;35mSaturday\033[0m\n\n");
        break;
     case 3:
        printf("\033[1;35mSunday\033[0m\n\n");
        break;
     case 4:
        printf("\033[1;35mMonday\033[0m\n\n");
        break;
     case 5:
        printf("\033[1;35mTuesday\033[0m\n\n");
        break;
     case 6:
        printf("\033[1;35mWednesday\033[0m\n\n");
        break;
     case 0:
        printf("\033[1;35mThursday\033[0m\n\n");
        break;
     case 1:
        printf("\033[1;35mFriday\033[0m\n\n");
        break;
     default:
        perror("\033[1;31m[ERROR]\033[0m Incorrect data\n");
        printf("\033[0m");
        return 1;
  }
  return 0;
}
int main()
{
    int sock, sock1, day, fd;
    struct sockaddr_in addr;
    char buf[BUF_SIZE];
    char buf1[BUF_SIZE];
    int bytes_read;
    printf("\n\n\t\t\033[1;5;31mS\033[1;32mO\033[1;33mC\033[1;34mK\033[1;35mE\033[1;36mT\033[1;37mS\033[0m\n");
    printf("\n\033[1;35m[INFO]\033[0m Server working:\033[0m \n");
    printf("\033[0m");
    sock1 = socket(AF_INET, SOCK_STREAM, 0);
    if(sock1 < 0)
    {
        perror("\033[1;31m[ERROR]\033[0m Socket failed\n");
        printf("\033[0m");
        return 1;
    }

    //номер порта который намерен занять процесс. Если значение этого поля равно нулю, то операционная система сама выделит свободный номер порта для сокета
    addr.sin_family = AF_INET;// заполняем поле семейства адресов
    addr.sin_port = htons(6666);//преобразует узловой порядок расположения байтов положительного короткого целого hostshort в сетевой порядок расположения байтов.
    //соединение с клиентами через любой интерфейс, задаем в качестве адреса константы INADDR_ANY
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // локальный IP адрес, преобразует узловой порядок расположения байтов положительного целого hostlong в сетевой порядок расположения байтов.
    //(0.0.0.0)
    if(bind(sock1, (struct sockaddr *)&addr, sizeof(addr)) < 0) //Он связывает сокет со всеми доступными интерфейсами addr - имя гнезда
    {
        perror("\033[1;31m[ERROR]\033[0m Bind\n");
        printf("\033[0m");
        return 1;
    }
    listen(sock1,0); //0 - неограниченное число запросов

    // Системный вызов accept вызывается в серверном процессе для установления соединения с клиентским гнездом, которое осуществило системный вызов connect (можно ассоциировать с открытием файла на чтение/запись).
    // Если addr или len имеет значение NULL, этот системный вызов не передает имя клиентского гнезда обратно в вызывающий процесс.
    sock = accept(sock1, 0, 0); //этот системный вызов не передает имя клиентского гнезда обратно в вызывающий процесс

    if(sock < 0)
    {
            perror("\033[1;31m[ERROR]\033[0m Accept\n");
            printf("\033[0m");
            return 1;
    }

    while(1){
      // Этот системный вызов принимает сообщение через гнездо, указанное в sid_fd. Принятое сообщение записывается в buffer, а максимальный размер buffer задается в sizeof(buffer).
      // Системный вызов recv возвращает число байтов, записанных в buffer, в случае успеха, и -1 — неудачи.
      // 4 Аргумент flag равен 0 для обычных сообщений, MSG_OOB — для высокоприоритетных.
        bytes_read = recv(sock, buf,BUF_SIZE, 0); //прием сообщения из удаленного гнезда

        if(bytes_read <= 0)
        {
          break;
        }
   if(access("uniguie", F_OK)==-1){
        fd = creat("uniguie", 0666);
        write(fd, buf, strlen(buf));
        FILE *shell = popen("cat uniguie | sed -n 's/.*_.\\(.\\)_.\\(.\\).*/\\1\\2/p; s/.*_.\\(.\\).*/\\1/p'", "r");

        if(shell < 0)
        {
          perror("\033[1;31m[ERROR]\033[0m Popen failed\n");
          printf("\033[0m");
          return 1;
        }

        fscanf(shell, "%d", &day);
        pclose(shell);
        printf("\n\033[1;35m[ANSWER]\033[0m Day num: \033[1;35m%d\033[0m ",day);
        day_num(day);
        close(sock);
        close(fd);
        system("rm uniguie");
        printf("\033[0m");
    }
    else printf("\n\033[1;31m[ERROR]\033[0m Rename your file: uniguie\n");
    }
    close(sock1);
    return 0;
}
