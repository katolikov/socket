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
     case 0:
        printf("\033[1;35mSaturday\n\n");
        break;
     case 1:
        printf("\033[1;35mSunday\n\n");
        break;
     case 2:
        printf("\033[1;35mMonday\n\n");
        break;
     case 3:
        printf("\033[1;35mTuesday\n\n");
        break;
     case 4:
        printf("\033[1;35mWednesday\n\n");
        break;
     case 5:
        printf("\033[1;35mThursday\n\n");
        break;
     case 6:
        printf("\033[1;35mFriday\n\n");
        break;
     default:
        perror("\033[1;31m[ERROR] Incorrect data\n");
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
    printf("\n\033[1;35m[INFO] Server working: \n");
    printf("\033[0m");
    sock1 = socket(AF_INET, SOCK_STREAM, 0);
    if(sock1 < 0)
    {
        perror("\033[1;31m[ERROR] Socket failed\n");
        printf("\033[0m");
        return 1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6666);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock1, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("\033[1;31m[ERROR] Bind\n");
        printf("\033[0m");
        return 1;
    }

    listen(sock1, 0);
    sock = accept(sock1, 0, 0);

    if(sock < 0)
    {
            perror("\033[1;31m[ERROR] Accept\n");
            printf("\033[0m");
            return 1;
    }

    while(1){
        bytes_read = recv(sock, buf, BUF_SIZE - 75, 0);

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
          perror("\033[1;31m[ERROR] Popen failed\n");
          printf("\033[0m");
          return 1;
        }

        fscanf(shell, "%d", &day);
        pclose(shell);
        printf("\n\033[1;35mDay num: %d ",day);
        day_num(day);
        close(sock);
        close(fd);
        system("rm uniguie");
        printf("\033[0m");
    }
    else printf("Rename your file: uniguie\n");
    }

    return 0;
}
