#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int status = 0;
    pid_t pid = fork(); //두개가 복사 즉, 자식 생성
    if(pid == 0)
    {
        return 3;
    }
    else
    {
        printf("Child pid: %d\r\n", pid);
        pid = fork(); // 자식 프로세스 생성
        if(pid == 0)
        {
                exit(7); // 7이라는 오류 코드 전송
        }
        else
        {
            printf("Chlid pid: %d \r\n", pid);
            wait(&status);
            if(WIFEXITED(status)) // WIFEXITED() 매크로 -> 자식 프로세스의 정상종료를 확인
            {
                printf("Child send one : %d \r\n", WEXITSTATUS(status));
                // 정상종료를 한 경우 자식 프로세스가 종료한 오류코드를 출력
            }
            wait(&status);
            if(WIFEXITED(status))
            {
                printf("Child send two : %d \r\n", WEXITSTATUS(status));
            }
            sleep(30); // 30초 쉬기
        }
    }   
    return 0;
}
