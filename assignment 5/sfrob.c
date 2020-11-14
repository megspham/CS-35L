#include <stdio.h>
#include <stdlib.h>


int frobcmp(char const *a, char const *b)
{
  
    while (1)
  {
      	if(*a == ' ' && *b == ' ')
	  return 0;
	else if (*a == ' ' || ((*a^42) < (*b^42)))
	  return -1;
	else if (*b == ' ' || ((*a^42) > (*b^42)))
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
  if (ferror(stdin)!= 0){
    fprintf(stderr, "Input error!");
    exit(1);
  }
}

void allocError() {
  fprintf(stderr, "Error allocating memory!");
  exit (1);
}

void outputError () {
  if (ferror(stdin)!=0) {
    fprintf(stderr, "Output error");
    exit(1);
  }
}

int main (){
  char** words = (char**)malloc(sizeof(char*));
  char* curr = (char*)malloc(sizeof(char));
  int cIndex =0;
  int wIndex =0;

  int b =0;

 while (b!=EOF){
   b = getchar();
   inputError();
   
   
   if (b==EOF && cIndex==0)
     break;

   if (b == EOF && cIndex!=0){
     curr [cIndex]= ' ';
   }

                                                                  
   else {
      curr[cIndex]= b;
   }


    if (curr[cIndex] == ' '){
      words [wIndex] = curr;
      //realloc and free current word
      words = (char**) realloc(words, (wIndex+2)*sizeof(char*));
      if (words ==NULL){
	free(words);
	allocError();
      }
      
      wIndex ++;
      cIndex =0;
      curr = NULL;
      curr = (char*) malloc(sizeof(char));
      
      if (curr ==NULL){
	free(curr);
	allocError();
      }
      
    }
    else {
      curr = (char*) realloc(curr, (cIndex+2)*sizeof(char));
      if (curr == NULL){
	free(curr);
	allocError();
	}
      
      cIndex++;
    }

    if (b==EOF){ 
      break;
    }

 }


  qsort(words, wIndex, sizeof(char*), compare);


   for (int j = 0; j < wIndex; j++) {
    for (int k = 0;;k++) {
      putchar(words[j][k]);
      outputError();
      if (words[j][k]==' ')
        break;
    }
   }

   for (int i=0; i<wIndex; i++)
     free(words[i]);
   
   free(curr);
   free(words);
   
   return 0;
 }
