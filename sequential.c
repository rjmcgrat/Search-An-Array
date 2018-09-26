#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CHAR_READ_PER_LINE 100

// helper functions


int main(int argc, char **argv){
  FILE *f1;
  f1 = fopen(argv[1], "r");
  int int_searched = atoi(argv[2]);
  char line[MAX_CHAR_READ_PER_LINE];
  int temp_int_array[10];
  int *int_array;
  int counter = 0;
  if (f1 == NULL){
    perror("Error opening file.\n");
    exit(0);
  }
  while(fgets(line, MAX_CHAR_READ_PER_LINE, f1) != NULL){
    char *pch;
    pch = strtok(line, " ");
    while (pch != NULL){
      temp_int_array[counter] = atoi(pch);
      counter++;
      pch = strtok(NULL, " ");
    }
    int_array = (int *)malloc(sizeof(int) * counter);
    int i;
    for (i = 0; i < counter; i++){
      int_array[i] = temp_int_array[i];
    }
  }
  int status = 0;
  int array[counter];
  int cc = 0;
  int j;
  for (j = 0; j < counter; j++){
    pid_t pid = fork();
    if(pid == 0)
    {
      printf("Child: %d with value: %d\n",getpid(), int_array[j]);
      if(int_array[j] == int_searched)
      {
        array[cc] = 1;
        cc++;
        exit(0);
      }
      else
      {
        array[cc] = -1;
        cc++;
        exit(0);
      }
    }
  }
int t;
for(t = 0; t< counter; t++)
{
  wait(&status);
}
int g;
for(g = 0; g < cc; g++)
{
  if(array[g] == 1)
  {
    printf("Found stuff: %d\n",g);
  }
}

  free(int_array);
  printf("Found stuff: -1\n");
  return 0;
}
