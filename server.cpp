#include "header.hpp"

void	error_handling(char	*message) {
	fputs(message, stderr);
	fputs("\n", stderr);
	exit(1);
}

int main() {
    int					serv_sock; // 서버 소켓
	int					clnt_sock; // 클라 소켓
    ssize_t             strLen;
    char                buf[100];
    int                 cnt = 0;

	socklen_t   		clnt_addr_size; // 클라 소켓 길이
	struct sockaddr_in	serv_addr;
	struct sockaddr_in	clnt_addr;
/*
    struct sockaddr_in {
	    short    sin_family;          // 주소 체계: AF_INET
	    u_short  sin_port;            // 16 비트 포트 번호, network byte order
	    struct   in_addr  sin_addr;   // 32 비트 IP 주소
	    char     sin_zero[8];         // 전체 크기를 16 비트로 맞추기 위한 dummy
    };
*/

    if ((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        error_handling((char *)("socket error"));
    int option = 1;

	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY 현재 내 ip를 자동으로 가지고 온다
	serv_addr.sin_port=htons(7998);

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling((char *)("bind error"));

	if (listen(serv_sock, 5) == -1)
		error_handling((char *)("listen error"));
    
    std::cout << "server start" << std::endl;

    struct pollfd pollFDs[100];

    pollFDs[0].fd = serv_sock; // 0번째 배열에는 서버 소켓
    pollFDs[0].events = POLLIN; // 읽도록 만듬
    pollFDs[0].revents = 0; // 처음에는 0으로 초기화 (아무것도 안일어남)
    cnt++;
    for (int i = 1; i < 100; i++)
        pollFDs[i].fd = -1;

    while (true) {
        int res = poll(pollFDs, 100, -1); // -1 무한대기
        
        if (res > 0) {
            if (pollFDs[0].revents == POLLIN) {
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                cnt++;
                std::cout << "connect client" << std::endl;
                for (int i = 1; i < 100; i++) {
                    if (pollFDs[i].fd == -1) {
                        pollFDs[i].fd = clnt_sock ;
                        pollFDs[i].events = POLLIN;
                        pollFDs[i].revents = 0;
                        break ;
                    }
                }
            }

            for (int i = 1; i < cnt && i < 100; i++) {
                switch (pollFDs[i].revents){
                    case 0:
                        break ;
                    case POLLIN:
                        strLen = read(pollFDs[i].fd, buf, 100);
                        std::cout << strLen << " bytes read" << std::endl;
                        buf[strLen] = '\0';
                        fputs(buf, stdout);
                        fflush(stdout);
                        write(pollFDs[i].fd, buf, strlen(buf));
                        break ;
                    default:
                        close(pollFDs[i].fd);
                        pollFDs[i].fd = -1;
                        pollFDs->revents = 0;
                }
            }
        }
    }
    close(serv_sock);
    return (0);
}