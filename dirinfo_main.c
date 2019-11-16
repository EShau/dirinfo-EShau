#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int findSize(char * dir_name, struct dirent * entry);
void makeGoodSize(unsigned long size, char print_statement[100]);
void printSize(char * dir_name);
void printDirectories(char * dir_name);
void printFiles(char * dir_name);

int findSize(char * parent_name, struct dirent * entry){
  if (entry->d_type == 4){
    return 0;
  }
  else if (entry->d_type == 8){
    struct stat buffer;
    int file = stat(entry->d_name, &buffer);
    return buffer.st_size;
  }
  else{
    return 0;
  }
}

void makeGoodSize(unsigned long size, char print_statement[100]){
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

void printSize(char * dir_name){
  DIR * directory = opendir(dir_name);
  struct dirent * entry = readdir(directory);
  unsigned long size = 0;
  while (entry){
    size += findSize(NULL, entry);
    entry = readdir(directory);
  }
  closedir(directory);
  char print_statement[100];
  makeGoodSize(size, print_statement);
  printf("Total Diectory Size: %s\n", print_statement);
}

void printDirectories(char * dir_name){
  printf("Directories: \n");
  DIR * directory = opendir(dir_name);
  struct dirent * entry = readdir(directory);
  while (entry){
    if (entry->d_type == 4){
      printf("\t%s\n", entry->d_name);
    }
    entry = readdir(directory);
  }
  closedir(directory);
}

void printFiles(char * dir_name){
  printf("Files: \n");
  DIR * directory = opendir(dir_name);
  struct dirent * entry = readdir(directory);
  while (entry){
    if (entry->d_type == 8){
      printf("\t%s\n", entry->d_name);
    }
    entry = readdir(directory);
  }
  closedir(directory);
}

int main(){
  char * dir_name = ".";
  printf("Statistics for directory: %s\n", dir_name);
  DIR * directory = opendir(dir_name);
  if (directory){
    printSize(dir_name);
    printDirectories(dir_name);
    printFiles(dir_name);
    closedir(directory);
  }
  else{
    printf("Failure in opening directory: %s\n", strerror(errno));
  }
  return 0;
}
