// @maxrok98
// does not work deterministingly
// read() syscall may read one line or couple and then
// close without waiting of closing stdin file descriptor
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

char buf[512];
char whitespace[] = " \t\r\n\v";

void
xargs(int argc, char *argv[])
{
  int n;
	char *args[MAXARG];

  while((n = read(0, buf, sizeof(buf))) > 0) {
		int j = 0;
	 	// copy argv into args
		for(int i = 1; i < argc; i++){
			args[j++] = argv[i];
		}
		// buf index for start of line
		int k = 0;
		int buf_len = strlen(buf);
		while(k <= buf_len){ // split buf by \n
			int args_j = j;
			//clear all args pointers after argv
			memset(args+j, 0, MAXARG-j);
			// copy pointers to the starts of words and null all spaces between words
			int i = k;
			for(; args_j < MAXARG && buf[i] != '\0'; i++){  
				if(!strchr(whitespace, buf[i]) && (buf[i-1] == '\0' || i == k))
					args[args_j++] = buf+i;
				if(buf[i] == '\n'){
					buf[i] = '\0';
					break;
				if(strchr(whitespace, buf[i]))
					buf[i] = '\0';
				}
			}
			k = i+1;
			int f = fork();
			if(f == 0){
				exec(argv[1], args);
				exit(1);
			}
			wait(&f);
		}
  }
  if(n < 0){
    fprintf(2, "xargs: read error\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
	xargs(argc, argv);
	exit(0);
}
