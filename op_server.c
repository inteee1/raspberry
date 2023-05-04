#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define OPSZ 4
int calculate(int, int[], char); 

int main(int argc, const char* argv[])
{
    int serv_sock = 0; // Server FD
    int clnt_sock = 0; // Client FD
    char op_info[BUFSIZ] = {'\0', };
    int result = 0; // 결과값
    int operand_count = 0; // 피연산자들의 갯수
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int receive_count = 0;
    int receive_length = 0;
    if (argc != 2)
        {
                puts("OP_SERVER 9999\r\n");
                exit(1);
        }
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
    {
        puts("socket() 함수 오류");
        exit(1);
    }
    memset(&serv_addr, 0, sizeof serv_addr);
    memset(&clnt_addr, 0, sizeof clnt_addr);
    serv_addr.sin_family = AF_INET; //IPv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 127.0.0.1 or myIP
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr* )&serv_addr, sizeof serv_addr) == -1) 
    {
        puts("bind() 함수 오류");
        exit(1);
    }
    if(listen(serv_sock, 5) == -1)
    {
        puts("listen() 함수 오류");
        exit(1);
    }
    int clnt_addr_size = sizeof clnt_addr;
    for(int i = 0; i < 5; ++i)
    {
        operand_count = 0;
        clnt_sock = accept(serv_sock, (struct sockaddr* )&clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1)
        {
            puts("accept() 함수 오류");
            exit(1);
        }
        read(clnt_sock, &operand_count, 1);
        receive_length = 0;
        while((operand_count * OPSZ + 1) > receive_length)
        {
            receive_count = read(clnt_sock, &op_info[receive_length], BUFSIZ - 1);
            receive_length += receive_count;
        }
        result += calculate(operand_count, (int* )op_info, op_info[receive_length - 1]);
        write(clnt_sock, (const char* )&result, sizeof result);
        close(clnt_sock);
        result = 0;
    }
    close(serv_sock);
    return 0;
}

int calculate(int op_num, int opnds[], char op)
{
    int result = opnds[0];
    switch(op)
    {
    case '+':
                for(int i = 1; i < op_num; ++i)
                {
                    result += opnds[i];
                }
              break;
    case '-' :
              for(int i = 1; i < op_num; ++i)
              {
                      result -= opnds[i];
              }
              break;
    case '*' :
              for (int i = 1; i < op_num; ++i)
              {
                      result *= opnds[i];
              }
    }
    return result;
}   
