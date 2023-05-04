#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h> // uint8_t, uint16_t
#include <arpa/inet.h>
#include <sys/socke.h>
#include <unistd.h>

#define TTL 64

void error_handling(const char*);

int main(int argc, const char * argv[])
{
    int recv_sock = 0;
    int str_length = 0;
    char buffer[BUFSIZ] = {'\0', }; 
    struct sockaddr_in serv_addr;
    struct ip_mreq join_addr; //Grouping 하기 위한 구조체
    if(argc != 3)
    {
        error_handling("./NEWS_RECEIVE <GroupIP> <PORT>");
    }
    recv_sock = socket(PF_INET, SOCK_DGRAM,0);
    if(recv_sock == -1) error_handling("socket() 오류 ");
    memset(&serv_addr, 0, sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // receiver -> 모든 ip에서 받겠다.
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(bind(recv_sock, (const struct sockaddr*)&serv_addr, sizeof serv_addr) == -1) error_handling("bind() 오류");
    /* GROUPING*/
    join_addr.imr_multiaddr.s_addr = inet_addr(argv[1]); // Group IP
    join_addr.imr_interface.s_addr = htonl(INADDR_ANY); // PORT

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,(void*)&join_addr, sizeof join_addr);
    //받은 메세지를 화면에 출력
    while(true)
    {
        str_length = recvfrom(recv_sock, buffer, BUFSIZ-1, 0, NULL, 0);
        if(str_length < 0) break;
        buffer[str_length] = '\0';
        fputs(buffer, stdout);
    }
    close(recv_sock);
    return 0;
}
        

void error_handling(const char* _message)
{
    fputs(_message, stderr);
    fputs("\r\n", stderr);
    exit(1);
}
