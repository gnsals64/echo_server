#include "header.hpp"

void	error_handling(char	*message) {
	fputs(message, stderr);
	fputs("\n", stderr);
	exit(1);
}


int main(int ac, char **av) {
    int					sock;
	struct sockaddr_in	serv_addr;
	char				id[100];

    if (ac != 2) {
        std::cout << "you have to enter Id" << std::endl;
        return (1);
    }

    strcpy(id, av[1]);
    std::cout << "id : " << id << std::endl;

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		error_handling((char *)("socket errro"));

	memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(7998);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling((char *)("connect error"));
    
    int readBytes, writtenBytes;
    char sendBuffer[100];
    char receiveBuffer[100];

	while (true)
	{
		printf("input : ");
 
            fgets(sendBuffer, 100,stdin);
 
            write(sock, sendBuffer, strlen(sendBuffer));
            
 
            readBytes = read(sock, receiveBuffer, 100);
            printf("%d bytes read\n", readBytes);
            receiveBuffer[readBytes] = '\0';
        
            fputs(receiveBuffer, stdout);
            fflush(stdout);
	}
	close(sock);


}