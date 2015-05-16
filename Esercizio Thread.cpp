/*
   v5 - Produttore e Consumatore.
   L'alternanza stretta fra P e C limita
   di molto la velocità di esecuzione.
   Usiamo ora un buffer circolare che
   permette per brevi periodi velocità 
   diverse fra P e C. Vedi caso tipico
   del buffer di un HD oppure la stampa.
   
   NB. Ora sono anche necessarie due 
   variabili diverse per segnalare pieno
   e vuoto così come per le variabili 
   condition!
*/

#include <stdio.h>
#include <windows.h>
#include "altconio.h"
#include <time.h>
#define N 10

int buffer[N];
int in,out; //indici scrittura e lettura
int full; //buffer pieno
int empty; //buffer vuoto

CRITICAL_SECTION mutex; //semaforo binario
CONDITION_VARIABLE not_full;// variabili condizione
CONDITION_VARIABLE not_empty;

unsigned long WINAPI Produce_thread(void* arg);
unsigned long WINAPI Consume_thread(void* arg);
int RandomNumber(int from, int to);

/**
 * main
 * */
int main()
{   int low, high;
    HANDLE P_thread, C_thread;
    
	low=1;   high=6;
	srand(time(NULL));
	printf("Produttore e Consumatore WIN\n");
	printf("We have 2 thread one produce other read from buffer\n");	
    full=FALSE; //buffer pieno NO
    empty=TRUE; //buffer vuoto SI
    in=0; //indice di scrittura sul buffer  
    out=0; //indice di lettura dal buffer
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

/**
 * Thread produttore
 * @param arg Argomenti della funzione
 * */
unsigned long WINAPI Produce_thread(void* arg) // Produttore
{	int r; int i;
	for(i=0;i<100;i++){
	    r=RandomNumber(1,6);	    	    
        //while(full) printf("<P>"); //busy waiting 
EnterCriticalSection(&mutex);
        if (full) 
		{ printf("<PS>");
		  SleepConditionVariableCS(&not_full, &mutex, INFINITE);
		}
LeaveCriticalSection(&mutex);
        //delay(RandomNumber(1,3)*10);
		buffer[in]=r; //scrivo sul buffer
		in=(in+1)%N;
		printf("-P:%d",r);
EnterCriticalSection(&mutex);
	    if (in==out) full=TRUE; //segnalo anche che ora il buffer è pieno
        if (empty) empty=FALSE;
LeaveCriticalSection(&mutex);
        WakeConditionVariable(&not_empty);
	}
	ExitThread(0);
}

/**
 * Thread consumatore
 * @param arg Argomenti della funzione
 */ 
unsigned long WINAPI Consume_thread(void* arg) // Consumatore
{	int r; int i;
	for(i=0;i<100;i++){
		delay(RandomNumber(1,3)*10);    
      //  while(!full) printf("<V>"); //busy waiting 
EnterCriticalSection(&mutex);
        if (empty) 
		  {
		  	 printf("<VS>");
		     SleepConditionVariableCS(&not_empty, &mutex, INFINITE);
		  }
		  
/**
 * Errori
 * */
LeaveCriticalSection(&mutex);
		r=buffer[out];//leggo dal buffer
		out=(out+1)%N;
        printf("-C:%d\n",r);
EnterCriticalSection(&mutex);
	    if (out==in) empty=TRUE;
	    if (full) full=FALSE;
LeaveCriticalSection(&mutex);    
        WakeConditionVariable(&not_full);    
	}
	ExitThread(0);
}
/**
 * Funzione che genera un numero random
 * */
int RandomNumber(int from, int to)
{   int r;

    r=rand()%(to-from+1)+from;
	
	return r;
}


