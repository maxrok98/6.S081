
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{ 
	int p[2];
	pipe(p);
	
	char recv_buf[5];
	if(fork() == 0){ //child
		read(p[0], recv_buf, sizeof(recv_buf));
		close(p[0]);
		printf("%d: received %s\n", getpid(), recv_buf);
		write(p[1], "pong", 5);
		close(p[1]);
	} else { //parent
		write(p[1], "ping", 5);
		close(p[1]);
		read(p[0], recv_buf, 5);
		printf("%d: received %s\n", getpid(), recv_buf);
		close(p[0]);
	}

	exit(0);
}
