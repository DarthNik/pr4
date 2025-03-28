#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void user(int fd, struct sockaddr_in client_addr){
	int n;
	int num = rand() % 100 + 1; //Загаданное число от 1 до 100
	char buf[1024];
	int res; //Предположение игрока
	char response[1024]; //Ответ сервера
	char ip[1024];
	inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip));
	printf("%s: клиент подключился\n", ip);
	while (1){
		memset(buf, 0, sizeof(buf));
		if ((n = recv(fd, buf, sizeof(buf) - 1, 0)) <= 0)
		    break;

		printf("%s: %s", ip, buf);
		res = atoi(buf);

		if (res < num){
            	    strcpy(response, "Больше");
            	    send(fd, response, strlen(response), 0);
        	}

		else if (res > num){
	    	    strcpy(response, "Меньше");
	    	    send(fd, response, strlen(response), 0);
		}

		else{
            	    strcpy(response, "Верно");
            	    send(fd, response, strlen(response), 0);
	    	    break;
        	}
        }

	printf("%s: Клиент отключился\n", ip);
	close (fd);
}


int main(int argc, char* argv[]){

    if (argc != 2) 
	fprintf(stderr, "Неверное количество аргументов\n");

    struct sockaddr_in serv_addr, client_addr;
    int port = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addr_len = sizeof(client_addr); 
    int sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	perror("Ошибка создания сокета");
	close(sock);
	exit(1);
    }

    if (bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
	perror("Ошибка привязки сокета");
	close(sock);
	exit(1);
    }

    listen(sock, 5);

    srand(time(NULL));
    int fd;
    while (1){
	if ((fd = accept(sock, (struct sockaddr*)&client_addr, &addr_len)) < 0){
	    perror("accept error");
	    continue;
	}
        user(fd, client_addr);
    }

    close(sock);
    return 0;
}
