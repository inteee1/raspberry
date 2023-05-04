#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h> // true, false
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h> //signal()
#include <sys/wait.h> //wait()
#include <string.h>

void error_handling(const char* );
void read_childproc(int);
int main(int argc, const char* argv[])
{
    pid_t pid; // child process id를 저장할 변수
    int serv_sock = 0;
    int clnt_sock = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    struct sigaction act; // 구조체를 사용해서 siganl 제어
    socklen_t addr_size = 0;
    int str_length = 0;
    bool state = false;
    char buf[BUFSIZ] = {'\0', };

    if(argc != 2)
    {
        error_handling("ECHO_MULTI_SERVER 9999");
    }
    act.sa_handler = read_childproc; //함수 등록
    sigemptyset(&act.sa_mask); // sigemptyset() 함수를 이용해서 sa_mask 초기화
    act.sa_flags = 0; // 기본이 0으로 세팅
    state = sigaction(SIGCHLD, &act, 0); // child process가 종료될떄 호출해주세요.
    if(state == -1)
    {
        error_handling("sigaction() ╰(*°▽°*)╯");
    }
    /* socket 생성하기*/

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
    {
        error_handling("socket() ╰(*°▽°*)╯");
    }
    memset(&serv_addr, 0, sizeof serv_addr);
    memset(&clnt_addr, 0, sizeof clnt_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof serv_addr) == -1) 
    {error_handling("bind() ╰(*°▽°*)╯");}
    if(listen(serv_sock,5) == -1) 
    {error_handling("listen() ╰(*°▽°*)╯");}

    while(true)
    {
        addr_size = sizeof clnt_addr;
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*)&addr_size);
        if(clnt_sock == -1) 
        {continue;}
        // 클라이먼트 오류가 발생해도 끊지 않고 다음 클라이언트를 기다림
        else {puts("new client connected.... \r\n");}
        // 클론 생성
        pid = fork(); //자식이 하나 만들어짐
        if(pid == -1)
        {
            close(clnt_sock);
            continue;
        }
        // 자식 프로세스 동작코드
        if(pid == 0)
        {
            close(serv_sock); // 복사되어서 serv_sock이 2개 생김 -> 하나 제거
        
            while((str_length = read(clnt_sock, buf, BUFSIZ)) != 0)
            {
                write(clnt_sock, buf, str_length); // 클라이언트에 전송
            }
            close(clnt_sock);
            puts("CLient disconnected...");
            return 0;
        }
     else close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void error_handling(const char* _message)
{
    fputs(_message, stdout);
    fputs("\r\n", stdout);
    exit(1);
}
// 자식 프로세스가 잘 종료되었는지 확인하는  코드
void read_childproc(int _signal)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed process id : %d\r\n", pid);
}
