#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();
    if(pid == 0) //pid = 0이면 자식
    { //자식 프로세스
        puts("Hello, this is a zombie process. \r\n");
    }
    else
    { //부모 프로세스
        printf("the zombie process PID : %d\r\n", pid);
        sleep(30); //30 seconds 쉬기

    }
    if(pid == 0)
    {
        puts(" 안녕히계세요 여러분~(zombie process)\r\n");

    }
    else
    {
        puts("End zombie slayer process\r\n");
    }
    return 0;
}
