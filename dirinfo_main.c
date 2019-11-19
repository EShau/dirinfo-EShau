#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int findSize(struct dirent * entry);
void makeGoodSize(unsigned long size, char print_statement[100]);
void printSize(char dir_name[100]);
void printDirectories(char dir_name[100]);
void printFiles(char dir_name[100]);

int findSize(struct dirent * entry){
  if (entry->d_type == DT_REG){
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

void printSize(char dir_name[100]){
  DIR * directory = opendir(dir_name);
  struct dirent * entry = readdir(directory);
  unsigned long size = 0;
  while (entry){
    size += findSize(entry);
    entry = readdir(directory);
  }
  closedir(directory);
  char print_statement[100];
  makeGoodSize(size, print_statement);
  printf("Total Diectory Size: %s\n", print_statement);
}

void printDirectories(char dir_name[100]){
  printf("Directories: \n");
  DIR * directory = opendir(dir_name);
  struct dirent * entry = readdir(directory);
  while (entry){
    if (entry->d_type == DT_DIR){
      printf("\t%s\n", entry->d_name);
    }
    entry = readdir(directory);
  }
  closedir(directory);
}

void printFiles(char dir_name[100]){
  printf("Files: \n");
  DIR * directory = opendir(dir_name);
  struct dirent * entry = readdir(directory);
  while (entry){
    if (entry->d_type == DT_REG){
      printf("\t%s\n", entry->d_name);
    }
    entry = readdir(directory);
  }
  closedir(directory);
}

int main(int argc, char *argv[]){
  char dir_name[100];
  // int i;
  // for (i = 0; i < sizeof(dir_name); i++){
  //   dir_name[i] = '\0';
  // }
  if (argc == 2){
    strcpy(dir_name, argv[1]);
  }
  else{
    if (argc > 2){
      printf("Too many arguments supplied.\n");
    }
    else{
      printf("At least one argument expected.\n");
    }
    printf("Anyways, using fgets() to find a directory to scan.\n");
    printf("Enter valid directory name to scan:\n");
    // char buffer[100];
    // fgets(buffer, sizeof(buffer), stdin);
    // strncpy(dir_name, buffer, strlen(buffer) - 1);
    fgets(dir_name, sizeof(dir_name), stdin);
    dir_name[strlen(dir_name) - 1] = '\0';
  }
  printf("Statistics for directory: %s\n", dir_name);
  DIR * directory = opendir(dir_name);
  if (directory){
    closedir(directory);
    printSize(dir_name);
    printDirectories(dir_name);
    printFiles(dir_name);
  }
  else{
    printf("Failure in opening directory: %s\n", strerror(errno));
  }
  return 0;
}
