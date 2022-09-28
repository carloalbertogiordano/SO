#include <ourhdr.h>
#include <dirent.h>

int totalsizeFile = 0;

void ReadDir(char *path, int fd){
  struct Dir *dir;
  struct dirent *act;

  if(opendir(dir) == NULL)
    err_sys("errore apertura directory");

  while((act = reddir(dir))!=NULL){
    if(act->d_type == DT_REG){
      struct stat buff;
      char mompath[4096];

      strcpy(mompath, path);
      strcat(mompath, "/");

      if(stat(mompath, &buff) < 0)
        err_sys("errore stat");

      totalsizeFile+=buff.st_size;

      if(buff.st_size > 30*1024){
        char toWrite[4096];
        sprintf(toWrite, "%s\t%ld\n", act->d_name, buff.st_size);
        write(fd, toWrite, strlen(toWrite));
      }
    }

    else if(act->d_type == DT_DIR && !(strcmp(act->d_name,"."))){
      struct stat buff;
      char mompath[4096];
      strcpy(mompath, path);
      strcat(mompath,"/");
      strcat(mompath, act->d_name);
      ReadDir(mompath, fd);
    }
    free(act);
  }
  closedir(dir);
}

int main(int argc, char *argv[]){

  if(argc != 2)
    err_sys("numero di input errato");

  int fd = open("name.txt", O_CREAT | O_TRUNC | O_WRONLY, 0664);
  ReadDir(argv[1], fd);

  close(fd);

  printf("il numero totale di byte dei file incontrati è %ld", totalsizeFile);

  exit(0);
}
