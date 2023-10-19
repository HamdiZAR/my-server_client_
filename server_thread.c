#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

/* the port users will be connecting to */
#define MYPORT 3440
/* how many pending connections queue will hold */
#define BACKLOG 10
/*max MAX_CLIENT */
#define MAX_CLIENT

struct ClientData
{
    int client_fd;
    int client_num;
};



void* handle_client(void* client_data)
{
    struct ClientData* data = (struct ClientData*)client_data;
    int client_num = data->client_num;
    int client_fd = data->client_fd;
    printf("client_num = %d,client_fd = %d",client_num,client_fd);
    char message[1024];
    snprintf(message,1024,"Hello client %d",client_num);
    printf("meessage= %s",message);
    if (send(client_fd,message,strlen(message), 0) == -1) {
        perror("Server send() error <!>");
    }
    close(client_fd);
    //free(client_socket);
    free(data);
    return NULL;
}


int main() {
    /* listen on sock_fd, new connection on new_fd */
    int sock_fd, new_fd;
    /* my address information, address where I run this program */
    struct sockaddr_in my_addr;
    /* remote address is information */
    struct sockaddr_in their_addr;
    int sin_size;
    sock_fd = socket(AF_INET,SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket() error <!>");
        exit(1);
    } else {
        printf("socket() os OK...\n");
    }
    /* host byte order */
    my_addr.sin_family = AF_INET;
    /* short, network byte order */
    my_addr.sin_port = htons(MYPORT);
    /* auto-fill with my IP */
    my_addr.sin_addr.s_addr = INADDR_ANY;
    /* zero the rest of the struct */
    memset(&(my_addr.sin_zero), 0, 8);
    if (bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind() error <!>");
        exit(1);
    } else {
        printf("bind() on %d port is OK...\n", MYPORT);
    }
    if (listen(sock_fd, BACKLOG) == -1) {
        perror("listen() error <!>");
        exit(1);
    } else {
        printf("listen() is OK...\n");
    }
    
    /* //... other codes to read the received data...
    sin_size = sizeof(struct sockaddr_in);
    new_fd = accept(sock_fd, (struct sockaddr*)&their_addr, &sin_size);
    if (new_fd == -1) {
        perror("accept() error <!>");
    } else {
        printf("accept() is OK...\n");
    }
    //....other codes.... 
    if (send(new_fd, "This is a test string from server!\n", 37, 0) == -1) {
        perror("Server send() error <!>");
    }
    close(new_fd);close(sock_fd);*/
    int counter_theads=0;
    while (++counter_theads<=2)
    {
        new_fd = accept(sock_fd, (struct sockaddr*)&their_addr, &sin_size);
        if (new_fd == -1) {
        perror("accept() error <!>");
        continue;
        } else {
        printf("accept() is OK...\n");
        }
        //int* client_socket = malloc(sizeof(int));
        //*client_socket = new_fd;
        //int* client_socket =&new_fd;
        printf("hahiii\n");
        struct ClientData* client_data = (struct ClientData*)malloc(sizeof(struct ClientData));
        client_data->client_fd=new_fd;
        client_data->client_num=counter_theads;
        printf("client_num = %d,client_fd = %d\n",client_data->client_num,client_data->client_fd);
        
        pthread_t thread_id;
        if(pthread_create(&thread_id,NULL,handle_client,(void*)client_data))
        {
            perror("creation thread failed!!!");
            continue;
        }
        pthread_detach(thread_id);
        
    }

     close(sock_fd);

    return 0;
}

