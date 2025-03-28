#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    
    if (argc != 3) 
	fprintf(stderr, "Неверное количество аргументов\n");

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    struct in_addr serv;
    serv.s_addr = inet_addr(argv[1]);
    serv_addr.sin_addr = serv;
    int port = atoi(argv[2]);
    serv_addr.sin_port = htons(port);
    int sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	perror("Ошибка создания сокета");
	close(sock);
	exit(1);
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
	perror("Ошибка подключения к серверу");
	close(sock);
	exit(1);
    }
 
    printf("Подключено к серверу %s\n", argv[1]);
    printf("Угадайте число от 1 до 100\n");

    char buf[1024];
    int n;
    while (1){
	memset(buf, 0, sizeof(buf));
	printf("Введите число: ");
	if (fgets(buf, sizeof(buf), stdin) == 0)
	    break;
	
	if (send(sock, buf, strlen(buf), 0) < 0){
	    perror("Ошибка передачи данных");
	    break;
	}
	
	if ((n = recv(sock, buf, sizeof(buf) - 1, 0)) <= 0){
	    printf("Соединение разорвано\n");
	    break;
	}
	
	printf("Ответ сервера: %s\n", buf);
	if (strcmp(buf, "Верно") == 0){
	    printf("Поздравляем! Вы угадали число\n");
	    break;
	}
    }

    close(sock);
    return 0;
}

