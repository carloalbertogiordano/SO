#include <stdlib.h>
#include <stdio.h>
#include <ourhdr.h>

int main(int argc,char *argv[]){
	int lstowc[2];
	int wctosum[2];
	int fd = open("exec.log", O_WRONLY | O_APPEND| O_CREAT | O_TRUNC, 0644);
	pipe(lstowc);
	pipe(wctosum);

	for(int i = 1; i < argc || i == argc; i++){
			if(!fork()){
				dup2(lstowc[1], 1);
				close(0);
				char *tmpArgLs[]={"ls", argv[i], NULL};
				execve("/bin/ls",tmpArgLs,NULL);
			}

			wait(NULL);

			if(!fork()){
				dup2(lstowc[0], 0);
				dup2(fd,1);
				close(fd);
				//dup2(wctosum[1],fd);
				char *tmpArgWc[] = {"wc","-w",NULL};
				execve("/bin/wc",tmpArgWc,NULL);
			}

			/*if(!fork()){
				dup2(wctosum[0],0);
				FILE *stream;
				int c;
				stream = fdopen (wctosum[0], "r");
				while ((c = fgetc(stream)) != EOF)
					putchar (c);
				fclose (stream);
		}*/


	}

	//wait(NULL);

	return EXIT_SUCCESS;
}
