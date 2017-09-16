/* client.c
* @author jrmetzger
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

void error(const char *msg)
{
  perror(msg);
  exit(0);
}

void rtt(int neg, clock_t starttime)
{
  // calculate RTT speed
  if (neg) {
    printf("RTT is %lu milliseconds\n", (clock()-starttime)*1000L/CLOCKS_PER_SEC);
  }
}

int readline(int fd, char *buffer, int maxlen) {
  int ctr = 0;
  while (1) {
    read(fd, buffer + (ctr++), 1);
    if (ctr >= 2 && buffer[ctr-2] == '\r' && buffer[ctr-1] == '\n') {
      buffer[ctr] = 0;
      return ctr;
    }
    if (ctr >= maxlen)
    return maxlen;
  }
}

int main(int argc, char *argv[])
{
  int n, portno, sockfd;
  int neg = 0;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[512];

  printf("\n\n===== Project 1 ====\n\nServer Response\n----------------\n\n");

  if (strcmp(argv[1], "-p") == 0) {
    neg = 1;
    argv++;
  }

  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  char *hostname = strdup(argv[1]), *url;

  if (strchr(hostname, '/') == NULL) {
    url = "/";
  } else {
    *(strchr(hostname, '/')) = 0;
    url = strchr(argv[1], '/');
  }
  server = gethostbyname(hostname);
  url = strchr(url, '/');

  // No argument given after ./client
  if (argc < 3){fprintf(stderr,"usage %s [-options] server port\n", argv[-neg]);exit(0);}

  // socket is negative
  if (sockfd < 0){error("ERROR opening socket");}

  // server is empty
  if (server == NULL){fprintf(stderr,"ERROR, no such host\n");exit(0);}

  // setup
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
  (char *)&serv_addr.sin_addr.s_addr,
  server->h_length);
  serv_addr.sin_port = htons(portno);

  // Connect to the server via a TCP Socket
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){error("ERROR connecting");}
  clock_t starttime = clock();
  sprintf(buffer, "GET %s HTTP/1.1\r\n", url);
  write(sockfd, buffer, strlen(buffer));

  // write more headers
  sprintf(buffer, "Host: %s\r\n", hostname);
  write(sockfd, buffer, strlen(buffer));
  sprintf(buffer, "User-Agent: Homie/4.20\r\n\r\n");
  write(sockfd, buffer, strlen(buffer));

  int sawEmptyLine = 0, // When we see an empty line it indicates that the response is begining
  contentLength = 0,
  status = -1;

  while (!sawEmptyLine)
  {
    readline(sockfd, buffer, 255);
    printf("%s", buffer);
    if (status == -1) {
      // Read the status
      char *st = strchr(buffer, ' ');
      if ((st++) == NULL) continue;
      if (strchr(st, ' ') != NULL)
      *(strchr(st, ' ')) = 0;
      status = atoi(st);
      if (status != 200)
      {
        printf("Error: Got HTTP status %d. Not success\n", status);
        sawEmptyLine = 1;
      }
    } else if (contentLength == 0 && strncasecmp(buffer, "Content-length", 14) == 0)
    {
      // WE got length
      char *l = strchr(buffer, ' ');
      if ((l++) == NULL)
      {
        continue;
      }
      if (strpbrk(l, " \r\n") != NULL)
      {
        *(strpbrk(l, " \r\n")) = 0;
      }
      contentLength = atoi(l);
    } else {
      if (buffer[0] == '\r' && buffer[1] == '\n')
        sawEmptyLine = 1;
    }
  }
if (status==200){
    FILE *out = fopen("index.html", "wb");
    int i;
    for (i = 0; i < contentLength; i++) {
      char c;
      read(sockfd, &c, 1);
      fprintf(out, "%c", c);
    }
    fclose(out);
  }

    rtt(neg, starttime);

    FILE *in = fopen("index.html", "rb");

    // // dumps out the html code
    // for (i = 0; i < contentLength; i++)
    // {
    //   char c;
    //   fread(&c, 1, 1, in);
    //   printf("%c", c);
    // }

    fclose(in);

    close(sockfd);
    free(hostname);
    printf("\nSaved to index.html\n");
    printf("\n========== EOF ==========\n\n");

    return 0;
  }
