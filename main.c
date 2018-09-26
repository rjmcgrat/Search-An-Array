// ICS53 Lab2 Winter 2018
// Rodney McGrath 55352053
// Ashton Garcia 86229480

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <math.h>

#define MAX_CHAR_READ_PER_LINE 100

// helper functions
int find_index(int array[], int lower_i, int upper_i, int search){
  int i;
  for(i = lower_i; lower_i < upper_i; i++){
    if (array[i] == search){
      return i;
    }
  }
  return -1;
}
void recursive_fork(int array[], int lower_i, int upper_i, int search){
  if (upper_i == lower_i){
    if (array[lower_i] == search){ //check to see if search is found
      exit(EXIT_SUCCESS);
    }
    else
      exit(EXIT_FAILURE);
  }
  pid_t lid; //left child process id
  pid_t rid; //right child process id
  int split_point; //splitpoint of array
  split_point = (upper_i + lower_i) / 2;
  lid = fork();
  if (lid == 0){ // child
    recursive_fork(array, lower_i, split_point, search);
  }
  else{ //parent
    rid = fork();
    if(rid == 0){
      recursive_fork(array, split_point + 1, upper_i, search);
    }
  }
  int l_status;
  int r_status;
  waitpid(lid, &l_status, 0);
  printf("pid %d, value: %d\n", getpid(), array[split_point]);
  waitpid(rid, &r_status, 0);
  if(WEXITSTATUS(l_status) == 0 || WEXITSTATUS(r_status) == 0){
    exit(EXIT_SUCCESS);
  }
  else{
    exit(EXIT_FAILURE);
  }
}



int main(int argc, char **argv){
  FILE *f1;
  f1 = fopen(argv[1], "r");
  int int_searched = atoi(argv[2]);
  char line[MAX_CHAR_READ_PER_LINE];
  int int_array[10];
  int counter = 0;
  if (f1 == NULL){
    perror("Error opening file.\n");
    exit(EXIT_FAILURE);
  }
  while(fgets(line, MAX_CHAR_READ_PER_LINE, f1) != NULL){
    char *pch;
    pch = strtok(line, " ");
    while (pch != NULL){
      int_array[counter] = atoi(pch);
      counter++;
      pch = strtok(NULL, " ");
      if (counter > 10){
        printf("Error exceeded maxiumum of 10 integers\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  fclose(f1);
  int status;
  int exit_status;
  int index_found;
  pid_t pid = fork();
  if(pid == 0){
    recursive_fork(int_array, 0, counter, int_searched);
  }
  else{
    waitpid(pid, &status, 0);
    exit_status = WEXITSTATUS(status);
    if(exit_status == 1){
      printf("Search output: -1\n");
    }
    else{
      index_found = find_index(int_array, 0, counter, int_searched);
      printf("Search output: %d\n", index_found);
    }
  }
  return 0;
}
