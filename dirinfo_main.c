#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <math.h>

unsigned long findSize(char * current_dir, struct dirent * entry);
char * printFileSize(unsigned long size);

unsigned long findSize(char * current_dir, struct dirent * entry){
  unsigned long size = 0;
  printf("Directory: %s\n", current_dir);
  if (entry->d_type == 4){
    DIR * directory = opendir(entry->d_name);
    if (directory){
      struct dirent * entry_in_entry = readdir(directory);
      char copy_dir[50];
      while (entry_in_entry){
        strcpy(copy_dir, current_dir);
        strcat(copy_dir, "/");
        strcat(copy_dir, entry_in_entry->d_name);
        //printf("%s\n", entry_in_entry->d_name);
        size += findSize(copy_dir, entry_in_entry);
        entry_in_entry = readdir(directory);
      }
      closedir(directory);
    }
  }
  else if (entry->d_type == 8){
    struct stat buffer;
    int file = stat(entry->d_name, &buffer);
    return buffer.st_size;
  }
  else{
    return 0;
  }
  return size;
}

char * printFileSize(unsigned long size){
  char print_statement[10];
  double billion = pow(2, 30);
  double million = pow(2, 20);
  double thousand = pow(2, 10);
  if (size > billion){
    sprintf(print_statement, "%.3lf GB", size / billion);
  }
  else if (size > million){
    sprintf(print_statement, "%.3lf MB", size / million);
  }
  else if (size > thousand){
    sprintf(print_statement, "%.3lf KB", size / thousand);
  }
  else{
    sprintf(print_statement, "%lu B", size);
  }
}

int main(){
  printf("ok\n");
  char * dir_name = ".";
  printf("Statistics for directory: %s\n", dir_name);
  DIR * directory = opendir(dir_name);
  if (directory){
    unsigned long size = 0;
    struct dirent * entry = readdir(directory);
    printf("ok2\n");
    while (entry){
      //printf("%s\n", entry->d_name);
      size += findSize(dir_name, entry);
      entry = readdir(directory);
    }
    printf("ok3\n");
    closedir(directory);
    printf("ok4\n");
    printf("Total directory size: %s\n", printFileSize(size));
  }
  else{
    printf("Failure in opening directory: %s\n", strerror(errno));
  }
  return 0;
}
