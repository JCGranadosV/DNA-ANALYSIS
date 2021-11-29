#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include <assert.h>


#define max(x, y) (((x) > (y)) ? (x) : (y))
#define MAX 100000000
#define NTHREADS 20
int counterSeq = 0;
char completo[MAX];
char line[MAX];

int resultados[100000000];
int resultCounter = 0;
int included[100000000];
int includedCounter = 0;
int secMapeadas=0;
int secNoMapeadas=0;

struct lineaGenoma{
  int id; //numero de linea
  char *linea; //almacena linea completa
  int caracter; //caracter en el que se encontró, si es -1 no se encontró.
};
void encuentraString(char x[MAX], char y[MAX])
{ 
  int index;
  char *e = strstr(x,y);
  index = (int)(e-x);
  //sleep(5); para poner pausas entre lecturas de lineas
  if(strstr(x,y)){
    printf("\n %s a partir del caracter %d \n", y, index);
    resultados[resultCounter] = index;
    resultCounter++;
    resultados[resultCounter] = strlen(y) + index;
    resultCounter++;
    secMapeadas++;
  }else{printf("\n %s no se encontro\n", y);
  secNoMapeadas++;
  } 
}


bool include(int value){
   for(int i = 0; i < includedCounter; i++){
           if(included[i] == value){
                return true;
           }
        }   
  return false;  
}

float encuentraPorcentaje()
{ 
        
for(int i = 0; i < resultCounter/2; i += 2){

        for(int j = resultados[i]; j < resultados[i+1]; j++){
                if(!include(j)){
                     included[includedCounter] = j;
                     includedCounter++;
                }
        }
}



double temp = (double)includedCounter/(double)strlen(completo);

return temp;
}

void* serverSequence(void* argument) {
  struct lineaGenoma args;
  args = *((struct lineaGenoma *) argument);
  encuentraString(completo,args.linea);
  if(args.id==4 || args.id==1000){
    //AQUI

    printf("\n  Porcentaje : %f \n",encuentraPorcentaje() *100);
    printf("\n Secuencias Mapeadas: %d\n Secuencias no Mapeadas: %d",secMapeadas, secNoMapeadas);
    printf("\n Secuencias No Mapeadas: %d",secNoMapeadas);
    printf("TEST");
  }
 
  return NULL;
}
/*
void* serverReference(void* argument) {
  FILE *reference;
  int opcion=0;

    if ((reference=fopen("ejemploSecuencia.txt","r")) == NULL){
    printf("Error abriendo archivo .txt !");
// Se hace exit si el pointer da return a null
    exit(1);
  }
	fgets(completo, sizeof(completo),reference);
  fclose(reference);
  printf("REFERENCE HAS BEEN UPLOADED\n");
  printf("REFERENCE IS: %s",completo);
}
*/

void uploadReference()
{
  FILE *reference;
  int opcion=0;

    if ((reference=fopen("S._cerevisiae_processed.txt","r")) == NULL){
    printf("Error abriendo archivo .txt !");
    // Program exits if the file pointer returns NULL.
    exit(1);
  }
	fgets(completo, sizeof(completo),reference);
  fclose(reference);
  printf("REFERENCE HAS BEEN UPLOADED\n");
  printf("REFERENCE IS: %s",completo);
}

void uploadSequence()
{
  FILE *sequence;
  FILE *reference;
      if ((reference=fopen("S._cerevisiae_processed.txt","r")) == NULL){
    printf("Error con archivo .txt !");
   // Se hace exit si el pointer da return a null
    exit(1);
  }
	fgets(completo, sizeof(completo),reference);
    if ((sequence=fopen("s_cerevisia_2021_03.seq","r")) == NULL){
    printf("Error abriendo archivo .seq !");

    // Se hace exit si el pointer da return a null
    exit(1);
  }
  	while (fgets(line, sizeof(line), sequence)) {
    line[strlen(line) - 1] = '\0';
    encuentraString(completo,line);
		counterSeq ++;
	}

	printf("Lineas totales: %d \n", counterSeq);
	fclose(sequence);
}


int main(int argc, char *argv[]) {
  FILE *sequence;
  if ((sequence=fopen("s_cerevisia_2021_03.seq","r")) == NULL){
    printf("Error abriendo archivo .seq !");

    // Se hace exit si el pointer da return a null
    exit(1);
  }
  
 
  pthread_t threads[NTHREADS];
  
  struct lineaGenoma thread_args[1000];
  int rc;
  unsigned i;
  unsigned index;
  	
  int opcion=0;
  while(opcion!=3){
    
  
  printf("\n\n Elija acción a llevar a cabo: \n 1. Upload_Reference || 2. Upload_Sequence\n");
  scanf("%d",&opcion);
  if(opcion==1){
   uploadReference();
 } else if(opcion ==2){
  index=0;
  while (fgets(line, sizeof(line), sequence)) {
    line[strlen(line) - 1] = '\0';
    thread_args[index].linea = malloc (sizeof (line));
    strcpy(thread_args[index].linea,line);
    thread_args[ index ].id = index;
    printf("CREANDO THREAD: %s\n", line);
    rc = pthread_create(&threads[index], NULL, serverSequence, &thread_args[index]);
    assert(!rc);
    index ++;
	}
  //printf("\n Cantidad de lineas %d \n", index);
	
   //esperar a que acaben todos los threads
  for (index = 0; index < NTHREADS; ++index) {
		// parar hasta que el thread "index" termine
    rc = pthread_join(threads[index], NULL);
    assert(!rc);
  }
  
 } 	
}
return 0;
}