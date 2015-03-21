/*
   v3 - Produttore e Consumatore.
   Introduce la sezione critica per
   garantire che la modifica delle
   variabili condivise avvenga in 
   mutua esclusione.
   NB. La cs (critical section) deve
   essere inizializzata altrimenti 
   non funziona!
*/

#include <stdio.h>
#include <windows.h>
#include "altconio.h"
#include <time.h>

int buffer;
int full; //buffer pieno
CRITICAL_SECTION mutex; //semaforo binario

unsigned long WINAPI Produce_thread(void* arg);
unsigned long WINAPI Consume_thread(void* arg);
int RandomNumber(int from, int to);

int main()
{   int low, high;
    HANDLE P_thread, C_thread;
    
	low=1;   high=6;
	srand(time(NULL));
	printf("Produttore e Consumatore WIN\n");
	printf("We have 2 thread one produce other read from buffer\n");	
    full=FALSE; //dato non pronto
    InitializeCriticalSection(&mutex);
    // creazione dei thread corrispondenti alle funzioni di lettura, elaborazione e scrittura
    P_thread = CreateThread(NULL, 1024, &Produce_thread, NULL, 0, NULL);
    C_thread = CreateThread(NULL, 1024, &Consume_thread, NULL, 0, NULL);
    // attesa della terminazione dei thread
    WaitForSingleObject(P_thread, INFINITE);
    WaitForSingleObject(C_thread, INFINITE);
    printf("\n...END\n");
    return 0;
}
unsigned long WINAPI Produce_thread(void* arg) // Produttore
{	int r; int i;
	for(i=0;i<100;i++){
	    r=RandomNumber(1,6);	    	    
        while(full) printf("<P>"); //busy waiting 
        delay(RandomNumber(1,3)*10);
		buffer=r; //scrivo sul buffer
		printf("-P:%d",r);
EnterCriticalSection(&mutex);
	    full=TRUE; //segnalo anche che ora il buffer è pieno
LeaveCriticalSection(&mutex);
	}
	ExitThread(0);
}

unsigned long WINAPI Consume_thread(void* arg) // Consumatore
{	int r; int i;
	for(i=0;i<100;i++){
		delay(RandomNumber(3,5)*10);    
        while(!full) printf("<V>"); //busy waiting 
		r=buffer;//leggo dal buffer
        printf("-C:%d\n",r);
EnterCriticalSection(&mutex);
	    full=FALSE;//segnalo che ho letto il dato
LeaveCriticalSection(&mutex);        
	}
	ExitThread(0);
}
int RandomNumber(int from, int to)
{   int r;

    r=rand()%(to-from+1)+from;
	
	return r;
}

/*N=100000;
thread P()

thred C()

main()
{
   empty=TRUE;
   full=FALSE;
   count=0;
   esegui il thread P
   esegui il thread C
   aspetta la fine di P
   aspetta la fine di C
   return 0;
}

thread P()
 total=1000;
 do
{ 

  if (full) sleep (not_full);
   1. full falsa
     2. full vera ho dormito
            e poi qualcuno mi
            ha risvegliato.
  
  r=random;
  total--
  delay(variabile fra 0.1 e 5 sec)
  b[i];
  i++;
  count++;
  if !(count<N) full=TRUE;
  if (empty==TRUE) 
     empty=FALSE;
     wake(not_empty)

  if total==0 continua=FALSE
 }while(continua) 
  
 if(empty==TRUE)
    wake(not_empty)
  exit
 


thred C()

 do{
   if(empty) sleep(not_empty)
   stampa b[j]
   delay(variabile fra 0.1 e 5 sec)
   j++
   count--
   if (count==0) 
      empty=TRUE
      if continua==FALSE
         consumatore fuori!
   if (full==TRUE) 
       full=FALSE   
       wake(not_full)
  }while(1)

*/


