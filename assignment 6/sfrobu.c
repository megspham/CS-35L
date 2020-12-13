#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <malloc.h>
#include <ctype.h>

int fOption =0; 

int frobcmp(char const *a, char const *b)
{

    while (1)
  {
    char a1 = *a^42;
    char b1 = *b^42;

    if (fOption==1){
      a1 = toupper(a1);
      b1 = toupper(b1);
    }
        if(*a == ' ' && *b == ' ')
          return 0;
        else if (*a == ' ' || ((a1) < (b1)))
          return -1;
        else if (*b == ' ' || ((a1) > (b1)))
          return 1;

        a++;
        b++;
  }
  return 0;
}




int compare(const void *a, const void *b)
{
  return frobcmp(*(const char**)a, *(const char**)b);
}


void inputError (){
  if (ferror(stdin)!=0){
    fprintf(stderr, "Input error!\n");
    exit(1);
  }
  
}

void allocError() {
  fprintf(stderr, "Error allocating memory!\n");
  exit (1);
}

void outputError () {
  if (ferror(stdin)!=0){
    fprintf(stderr, "Output error\n");
    exit(1);
  }
}

int main (int argc, char* argv[]){
  if (argc == 2 && argv[1][0]!= '-' && argv[1][1]!= 'f'){
    fprintf(stderr, "Argument not possible\n");
    exit (1);
  }

  else if (argc==2 && argv[1][0] == '-' && argv[1][1] == 'f'){
    fOption =1; 
  }
  
  if (argc >2){
    fprintf(stderr, "wrong number of operands\n");
    exit (1); 
  }
  
  struct stat fileData;

 if (fstat(STDIN_FILENO, &fileData) < 0 ){
   fprintf(stderr, "fstat error!\n");
   exit(1);
 }
 
 int filesize = fileData.st_size; 
 if (!filesize)
   filesize = 8192; 
 int buffersize = filesize+1;
 ssize_t s =1; 
 int bIndex =0; 
  
 char* buffer= (char*)malloc(sizeof(char)*buffersize);
 
 char** words = (char**)malloc(sizeof(char*));
 int wIndex =0;

  char b =0;

  int wordcount =0; 
  //read stdin into buffer
  while (1){
    //if reaches capacity, resize buffer
    if (bIndex == buffersize){
      buffersize*=2; 
      buffer = (char*)realloc(buffer, sizeof(char*)*buffersize);
      if (buffer==NULL){
	free(buffer);
	allocError();
      }
    }
    //read in bytes from stdin
    s = read(STDIN_FILENO, &b, sizeof(char));
    inputError();
  
    if (s==0 && bIndex==0){
    break;
    }
    
   if (s==0 && bIndex!=0){
     buffer [bIndex]= ' ';
   }

   
   else {
      buffer[bIndex]= b;
   }


   
   if (buffer[bIndex] == ' '){
     words [wIndex] = buffer;
     
     //realloc and free current word
     words = (char**) realloc(words, (wIndex+2)*sizeof(char*));
     if (words ==NULL){
       free(words);
       allocError();
     }
     
     
     wIndex ++;
     //cIndex =0;
     bIndex =0; 
     buffer = NULL;
     buffer = (char*) malloc(sizeof(char));
     
     if (buffer ==NULL){
       free(buffer);
       allocError();
     }
     
   }
   else {
     buffer = (char*) realloc(buffer, (bIndex+2)*sizeof(char));
     if (buffer == NULL){
       free(buffer);
       allocError();
     }
     
     bIndex++;
   }
   
    if (s==0){
      break;
    }
    if (buffer==NULL){
      free(buffer);
    allocError();
    }
  }
  
  
  qsort(words, wIndex, sizeof(char*), compare);
  //  printf("word index: %d", wIndex);
  int wordlen =0;
  for (int j = 0; j < wIndex; j++) {
    wordlen =0; 
    for (int k = 0;;k++) {
      wordlen++;
      if (words[j][k]==' '){
	//wordcount ++;
	//	printf("number : %d", wordcount);
        break;
      }
    }
    write(STDOUT_FILENO, words[j],wordlen);
    outputError();
  }
  
  
   for (int i=0; i<wIndex; i++)
     free(words[i]);
   
   free(buffer);
   free(words);
   
   return 0;
}
