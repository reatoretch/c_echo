#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main (int argc, char *argv[]) {
  int ik;
  int sockfd, nbytes;
  int read_value = strlen("Content-Length: ");
  int contentLength = 0 - read_value;
  char *content;
  char buf [BUFSIZ];
  char endpoint [BUFSIZ];
  char mesg [BUFSIZ];
  char tmp [BUFSIZ];
  char *servername = "localhost";
  struct servent *serv;
  struct hostent *servhost;
  struct sockaddr_in servaddr;

  if (argc >= 2) {
    servername = argv[1];	/* 第1引数: サーバ名 (portは80固定)*/
  }
  
  strcpy(endpoint, servername);
  
  for(int i = 0; servername[i] != '\0'; ++i) {
      if(servername[i]=='/') {
          servername[i] = '\0';
      }
  }
  
  serv = getservbyport(htons(80),"tcp");
  
  if(serv == NULL) {
      perror ("getservbyport"); exit(1);
  }

  servhost = gethostbyname (servername);
  
  if (servhost == NULL) {
    in_addr_t addr;
    addr = inet_addr (servername);
    servhost = gethostbyaddr ((char *)&addr, sizeof(addr), AF_INET);
    if (servhost == NULL) {
      perror ("gethostbyaddr"); exit (1);
    }
  }
  
  if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
    perror ("socket"); exit (1);
  }
  
  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = serv->s_port;
  
  memcpy ((char *)&servaddr.sin_addr, servhost->h_addr, servhost->h_length);

  if (connect (sockfd, (struct sockaddr *)&servaddr, sizeof (servaddr)) < 0) {
    perror ("connect"); exit (1);
  }

  snprintf(mesg, BUFSIZ, "GET http://%s/ HTTP/1.0\r\n\r\n", endpoint);
  nbytes = write (sockfd, mesg, strlen (mesg) + 1);
  nbytes = read (sockfd, buf, sizeof (buf) - 1);
  buf [nbytes] = '\0';
  
  printf("%d",buf);
  
  content = strstr(buf, "Content-Length");
  
  for(int i = 0; content[i] != '\n'; ++i) {
    contentLength++;
  }
  
  strncpy(tmp, content+read_value, contentLength);
  tmp[contentLength] = '\0';
  contentLength = atoi(tmp);
  contentLength-=nbytes;
  
  while(contentLength > 0) {
    nbytes = read (sockfd, buf, sizeof (buf) - 1);
    buf[nbytes] = '\0';
    printf ("%s",buf);
    contentLength -= nbytes;
  }
  
  close (sockfd);
  return 0;
}
