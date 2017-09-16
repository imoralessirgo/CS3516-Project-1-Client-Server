
/* server.c
* @author jrmetzger
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>


void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  time_t t;
  time(&t);

  printf("\n==== Welcome to the Server =====\n\n");

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    error("ERROR opening socket");
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
    error("ERROR on binding\n");
    printf("\n=========== EOF =================\n");
  }

  listen(sockfd,5);
  clilen = sizeof(cli_addr);

  while (1)
  {
    printf("\nListening . . .\n");
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);

    if (newsockfd < 0)
    {
      error("ERROR on accept");
      printf("\n=========== EOF =================\n");
    }

    bzero(buffer,256);
    n = read(newsockfd,buffer,255);

    if (n < 0)
    {
      error("ERROR reading from socket");
      printf("\n=========== EOF =================\n");
    }

    char* url_orig = strchr(buffer, ' ') + 1;
    *(strchr(url_orig, ' ')) = 0;
    char* url = malloc(strlen(url_orig) + 1);
    strcpy(url, url_orig);

    if (n == 255)
    {
      do
      {
        n = read(newsockfd, buffer, 255);
      }
      while (n == 255);
    }

    printf("Request Received for: %s\n", url);

    // FIND URL TO BE DISPLAYED
    if(strcmp(url,"/") == 0 || strcmp(url,"/index.html")==0)
    {

      FILE *fp = fopen("./TMDG.html", "r");
      fseek(fp, 0L, SEEK_END);
      long sz = ftell(fp);
      rewind(fp);

      char status[128];
      char date[128];
      char server[128];
      char length[128];
      char type[128];
      char print[128];

      sprintf(status, "HTTP/1.1 200 OK\r\n");
      sprintf(date, "Date: %s", ctime(&t));
      sprintf(server, "Server: (Unix)\r\n");
      sprintf(length, "Content-Length: %ld\r\n", sz);
      sprintf(type, "Content-Type: text/html; charset=utf-8\r\n\r\n");

      write(newsockfd, status, strlen(status));
      write(newsockfd, date, strlen(date));
      write(newsockfd, server, strlen(server));
      write(newsockfd, length, strlen(length));
      write(newsockfd, type, strlen(type));
      write(newsockfd, print, strlen(print));

      char *fbuf = malloc(sz + 8);
      fread(fbuf,1,sz,fp);
      write(newsockfd, fbuf, sz);
      free(fbuf);

    }
    else
    {
      char *mainresponse = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
      write(newsockfd, mainresponse, strlen(mainresponse));
    }

    if (n < 0)
    {
      error("ERROR writing to socket");
      printf("\n=========== EOF =================\n");
    }

    free(url);
    close(newsockfd);
  }

  close(sockfd);
  return 0;
}
