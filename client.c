#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#define BUF_SIZE 256

int main(){
  char buf[BUF_SIZE];
  int sock;
  socklen_t namelen;
  FILE *shell;
  struct sockaddr_in addr;

  printf("\n\033[1;35m[INFO]\033[0m Client working:\033[0m \n");
  printf("\033[0m");
  sock = socket(AF_INET, SOCK_STREAM, 0);//создаем гнездо
  //– SOCK_STREAM — виртуальный канал с предварительно установленным соединением;
  //– SOCK_DGRAM — межпроцессное взаимодействие с помощью дейтаграмм;
  //– SOCK_SEQPACKET — виртуальный канал с предварительно установленным соединением с возможностью передачи сообщений максимальной длины.

  if (sock < 0)
  {
    perror("\033[1;31m[ERROR]\033[0m Socket failed\n");
    printf("\033[0m");
    return 1;
  }

  printf("\n\033[1;32m[DEBUG]\033[0m Sock fd = %d\n\n", sock);
  // для соединения нужен адрес интерфейса внутренней петли (loopback interface).
  // В программе ему соответствует константа INADDR_LOOPBACK.
  // Пакеты, направляемые по этому адресу, в сеть не попадают. Вместо этого они передаются стеку протоколов TCP/IP как только что принятые.
  // Таким образом моделируется наличие виртуальной сети, в которой можно отлаживать сетевые приложения.
  addr.sin_family = AF_INET;
  addr.sin_port = htons(6666);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//(127.0.0.1) всегда приписывается локальному хосту через закольцовывающий интерфейс

  // Системный вызов connect вызывается в клиентском процессе для установления соединения с серверным гнездом.
  // Если sid обозначает потоковое гнездо, то между клиентским (КГ) и серверными (СГ) гнездами устанавливается
  // соединение с использованием виртуального канала. Потоковое гнездо клиента может соединяться с гнездом сервера только один раз.
  if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)//подключаемся по имени к виртуальному адрессному пространству
  {
    perror("\033[1;31m[ERROR]\033[0m Connect failed");
    printf("\033[0m");
    return 1;
  }

  shell = popen("cal", "r");
  if(shell < 0)
  {
    perror("\033[1;31m[ERROR]\033[0m Popen failed\n");
    printf("\033[0m");
    return 1;
  }
  fread(buf, 1, sizeof(buf), shell);
  pclose(shell);
  printf("\033[1;35m[INFO]\033[0m Send to server:\n\n---------------------\n\033[1;35m%s\033[0m---------------------\n", buf);
  send(sock, buf, strlen(buf), 0); //0 - обычные сообщения в гнездо sock из buf длинны buf
  
  close(sock);
  printf("\033[0m");
  return 0;
}
