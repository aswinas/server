#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_fn(int);
void load_page(int client, FILE *);
void http_head(int client);
int sock_create(char *,int);

int main(int argc, char *argv[])
{
	struct sockaddr_in client_addr;
    	int clientfd,sockfd;
	pthread_t thread;

        sockfd=sock_create(argv[1],argc);	// Socket Creation

  	socklen_t client_len;
	client_len=sizeof(struct sockaddr_in);

	while(1)
	{
	clientfd=accept(sockfd,(struct sockaddr *)&client_addr,&client_len);
        	if(clientfd==-1)
		{
		        perror("\nAccept Failed\n");
	        	exit (1);
	        }
	        if(pthread_create(&thread,NULL,thread_fn,clientfd))
	        {
	        perror("\nTHread Creation Failed\n");
	        exit (1);
	        }
	}
	close(sockfd);
}

void *thread_fn(int client)
{
	char buf[BUFSIZ];
	memset(buf,0,BUFSIZ);
	int n;
	FILE *p;
//	n=read(client,buf,BUFSIZ);
//	buf[n]=0;
//	printf("\nBUF=%s\n",buf);

		p=fopen("index.html","r");
		http_head(client);
		load_page(client,p);
}


void http_head(int client)
{
	char response[] = "HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html; charset=UTF-8\r\n\r\n";
	write(client, response, sizeof(response));
}


void load_page(int client, FILE *p)
{
	char buf[BUFSIZ];
	do
 	{
	fgets(buf, BUFSIZ, p);
	write(client, buf, strlen(buf));
 	}while(!feof(p));
	close(client);
}

int sock_create(char *c,int arg)
{
	int sockfd,port;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
    		if(sockfd==-1)
    		{
   		perror("\nSocket Creation Failed\n");
    		exit (1);
    		}
    
    	struct sockaddr_in server_addr;
    	
		if(arg==1)
		port=8080;
		else
		port=atoi(c);			      //custom port can be assigned during command line execution

	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons( port );            
 	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");   //localhost

    	int b;
        b=bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
       		if(b==-1)
   	 	{
    		perror("\nBind Failed\n");
	        exit (1);
    		}

    	int l;
    	l=listen(sockfd,5);
    		if(l==-1)
    		{
    		perror("\nListen Failed\n");
    		exit (1);
    		}

return sockfd;
}
