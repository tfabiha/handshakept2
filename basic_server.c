#include <signal.h>
#include "pipe_networking.h"

static void sighandler(int signo) {

  remove("client_write");
  exit(EXIT_SUCCESS);
}

int main() {

  signal(SIGINT, sighandler);
  
  int to_client; // write
  int from_client; // read

  from_client = server_handshake( &to_client );

  char buffer[BUFFER_SIZE];

  while (1)
    {
      if ( !read(from_client, buffer, BUFFER_SIZE) ) // if you can't read
	{
	  from_client = server_handshake( &to_client ); // resets if fd suddenly closed
	}
      else // if you can read
	{
	  // process the data
	  printf("[server] received: %s", buffer);

	  //reply to client
	  write(to_client, buffer, BUFFER_SIZE);
	}
      
    }

}
