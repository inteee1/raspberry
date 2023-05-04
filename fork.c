#include <stdio.h>
#include <unistd.h>

int g_value = 10; // 전역 변수

int main()
{
    pid_t pid;
    int l_value = 20; // 로컬변수
    ++g_value; //11
    ++l_value; //21

    pid = fork();
    if(pid == 0)
    { //자식 프로세스
        ++g_value; //12
        ++l_value; //22
    } else 
    {
        --g_value; //11
        --l_value; //21
    }
    if(pid == 0)
    {
        printf("Child Process : %d \t %d\r\n", g_value, l_value);
    }else
    {
        printf("Parent Process : %d \t %d\r\n", g_value, l_value);
    }
    return 0;
}
