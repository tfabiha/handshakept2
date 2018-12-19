#include "pipe_networking.h"
#include <sys/wait.h>

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  int fd_write;
  int fd_read;
  char buffer[100];

  printf("\n");
  
  if (mkfifo("client_write", 0644) == -1)
    printf("server making pipe error %d: %s\n", errno, strerror(errno));
  else
    printf("server made a pipe\n");
  
  
  fd_read = open("client_write", O_RDONLY);
  
  if (fd_read == -1)
    printf("server opening read file error %d: %s\n", errno, strerror(errno));
  else
    printf("server opened reading pipe\n");
  
  
  remove("client_write");
  
  
  if (read(fd_read, buffer, 20) == -1)
    printf("server reading from client error %d: %s\n", errno, strerror(errno));
  else
    printf("from client received: %s\n", buffer);
  
  
  fd_write = open(buffer, O_WRONLY);
  
  if (fd_write == -1)
    printf("server opening read file error %d: %s\n", errno, strerror(errno));
  else
    printf("server opened writing pipe\n");
  
  
  strcpy(buffer, ACK);
  printf("writing to client: %s\n", buffer);
  
  if (write(fd_write, buffer, HANDSHAKE_BUFFER_SIZE) == -1)
    printf("server writing to client error %d: %s\n", errno, strerror(errno));
  else
    printf("server attempted to write to client\n");
  
  
  if (read(fd_read, buffer, HANDSHAKE_BUFFER_SIZE) == -1)
    printf("server reading from client error %d: %s\n", errno, strerror(errno));
  else
    printf("from client received: %s\n", buffer);
  
  
  if (strcmp(buffer, ACK) == 0)
    printf("server can read and write\n");
  else
    printf("server did not receive proper feedback\n");
  
  printf("\n");
  
  * to_client = fd_write;
  return fd_read;
}


/*=========================
  client_handshake
  args: int * to_server
  
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

  int fd_write;
  int fd_read;
  char buffer[BUFFER_SIZE];
  
  if (mkfifo("server_write", 0644) == -1)
    printf("client making pipe error %d: %s\n", errno, strerror(errno));
  else
    printf("client made a pipe\n");
  
  
  fd_write = open("client_write", O_WRONLY);
  
  if (fd_write == -1)
    printf("client opening read file error %d: %s\n", errno, strerror(errno));
  else
    printf("client opened writing pipe\n");

  
  strcpy(buffer, "server_write\0");
  printf("writing to server: %s\n", buffer);
  
  if (write(fd_write, buffer, HANDSHAKE_BUFFER_SIZE) == -1)
    printf("client writing to server error %d: %s\n", errno, strerror(errno));
  else
    printf("client attempted to write to server\n");
  
  
  fd_read = open("server_write", O_RDONLY);
  
  if (fd_read == -1)
    printf("client opening read file error %d: %s\n", errno, strerror(errno));
  else
    printf("client opened reading pipe\n");
  

  remove("server_write");
  
  
  if (read(fd_read, buffer, HANDSHAKE_BUFFER_SIZE) == -1)
    printf("client reading from server error %d: %s\n", errno, strerror(errno));
  else
    printf("from server received: %s\n", buffer);
  
  
  if (strcmp(buffer, ACK) == 0)
    {
      strcpy(buffer, ACK);
      printf("writing to server: %s\n", buffer);
      
      if (write(fd_write, buffer, HANDSHAKE_BUFFER_SIZE) == -1)
	{
	  printf("client writing to server error %d: %s\n", errno, strerror(errno));
	}
      else
	{
	  printf("client attempted to write to server\n");
	  printf("client can read and write\n");
	}
    }
  else
    {
      printf("client did not receive proper feedback\n");
    }

  printf("\n");
  
  * to_server = fd_write;
  return fd_read;
}
