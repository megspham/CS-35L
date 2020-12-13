
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void inputError () {
  if (ferror(stdin)!=0) {
    fprintf(stderr, "Input error!");
    exit(1);
  }
}

void outputError() {
  if (ferror(stdin)!=0){
    fprintf(stderr, "Output error");
    exit(1);
  }
}



int main (int argc, char *argv[]) {

  if (argc!= 3){
    fprintf (stderr, "Wrong number of operands\n");
    exit(1); 
  }
  
  char *from = argv[1]; 
  char *to = argv[2];
  int word = 0; 

  
  int fromsize = strlen (from);
  int tosize = strlen(to);

  if (fromsize!= tosize){
    fprintf (stderr, "Strings are not the same length\n");
    exit(1);
  }
  
  for (int i=0; i< fromsize-1; i++){
    for (int j=i+1; j< fromsize; j++){
      if (from[i] == from [j]){
	fprintf(stderr, "Duplicate letter found!\n");
	exit(1);
      }
      if (from[i] == to[j]){
	fprintf(stderr, "Duplicate letter found!\n");
	exit(1); 
      }
      if (to[i] == to [j]){
	fprintf(stderr, "Duplicate letter found!\n");
	exit(1);
      }	
    }
  }

  //	ssize_t word = read(0, &c, sizeof(char)); 
  
  while(1){
    ssize_t s = read(0, &word, sizeof(char));
    if (s==0)
      break;
    if (word == EOF)
      break;
    
    for (int l=0; l<fromsize; l++){
      if (from[l] == word)
	word = to[l];
    }
    write(1, &word, sizeof(char));
    outputError();
  }
  printf("\n");
  
  return 1;
}
