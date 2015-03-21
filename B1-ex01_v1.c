/*
   v1 - Prima versione Produttore-Consumatore.
   Non abbiamo ALTERNANZA STRETTA
   come si vede dall'esecuzione i programmi
   hanno velocità diverse e quindi l'output
   non è corretto.
*/

#include <stdio.h>
#include <windows.h>
#include "altconio.h"
#include <time.h>
#define N 100

int buffer; //buffer di scambio dati fra P e C
int RandomNumber(int from, int to);
unsigned long WINAPI Produce_thread(void* arg); // Produttore
unsigned long WINAPI Consume_thread(void* arg); // Consumatore

int main()
{   HANDLE P_thread, C_thread;
    int low, high,count;
    low=1;high=6;
	srand(time(NULL));
	printf("Produttore e Consumatore WIN\n");
	printf("test phase for random numer from %d to %d:\n",low,high);
	for(count=0;count<100;count++)  printf("%d ", RandomNumber(low,high));
	while(!kbhit());	//aspetto che venga premuto un tasto
	printf("We have 2 thread one produce the other consume\n");
	delay(2000);
	clrscr();
    // creazione dei thread 
    P_thread = CreateThread(NULL, 1024, &Produce_thread, NULL, 0, NULL);
    C_thread = CreateThread(NULL, 1024, &Consume_thread, NULL, 0, NULL);
    // attesa della terminazione dei thread
    WaitForSingleObject(P_thread, INFINITE);
    WaitForSingleObject(C_thread, INFINITE);
    return 0;
}
unsigned long WINAPI Produce_thread(void* arg) // Produttore
{	int r,i;
	for(i=0;i<100;i++){
	    r=RandomNumber(1,6);
	    printf("-P:%d",r);
	    delay(RandomNumber(1,5)*10);
	    buffer=r; //scrivo sul buffer
	}
	ExitThread(0);
}
unsigned long WINAPI Consume_thread(void* arg) // Consumatore
{
	int r,i;
	for(i=0;i<100;i++){
	    delay(RandomNumber(1,5)*5);
		r=buffer;//leggo dal buffer
	    printf("-C:%d\n",r);
	    }
	ExitThread(0);
}
int RandomNumber(int from, int to)
{   int r;
    r=rand()%(to-from+1)+from;
	return r;
}
