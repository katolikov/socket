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

  printf("\n\033[1;35m[INFO] Client working: \n");
  printf("\033[0m");
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror("\033[1;31m[ERROR] Socket failed\n");
    printf("\033[0m");
    return 1;
  }

  printf("\n\033[1;32m[DEBUG] Sock fd = %d\n\n", sock);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(6666);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
  {
    perror("\033[1;31m[ERROR] Bind failed");
    printf("\033[0m");
    return 1;
  }
  listen(sock, 0);
  shell = popen("cal", "r");
  if(shell < 0)
  {
    perror("\033[1;31m[ERROR] Popen failed\n");
    printf("\033[0m");
    return 1;
  }
  fread(buf, 1, sizeof(buf), shell);
  pclose(shell);
  printf("\033[1;35m[INFO] Send to server:\n\n---------------------\n%s---------------------\n", buf);
  send(sock, buf, strlen(buf), 0);
  close(sock);
  printf("\033[0m");
  return 0;
}
