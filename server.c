#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[]){

    if (argc != 2) 
	fprintf(stderr, "Неверное количество аргументов\n");

    struct sockaddr_in serv_addr, client_addr;
    int port = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t addr_len = sizeof(client_addr); 
    int sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	perror("Ошибка создания сокета\n");
	close(sock);
	exit(1);
    }

    if (bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
	perror("Ошибка привязки сокета\n");
	close(sock);
	exit(1);
    }

    listen(sock, 5);

    int n;
    srand(time(NULL));
    int num = rand() % 100 + 1;//Загаданное число от 1 до 100
    printf("%d", num);
    char buf[1024];
    char ip[1024];
    int res;
    char response[1024];
    int fd;
    while (1){
	if ((fd = accept(sock, (struct sockaddr*)&client_addr, &addr_len)) < 0){
	    perror("accept error");
	    break;
	}
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip));
	printf("%s: клиент подключился\n", ip);
	memset(buf, 0, 1024);
	if ((n = recv(fd, buf, 1023, 0)) <= 0)
	    break;
	buf[n] = '\0';
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
	    close(fd);
	    break;
        }
    }

    close(sock);
    return 0;
}
