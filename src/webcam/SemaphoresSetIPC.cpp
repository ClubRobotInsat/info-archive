// SemaphoresSetIPC.cpp

#ifndef WIN32

#include "SemaphoresSetIPC.h"
#include <iostream>
using namespace std;

bool SemaphoresSetIPC::create(key_t key, int nbSemaphores) {
	if((semid = semget(key, nbSemaphores, IPC_CREAT | 0666)) < 0) {
		perror("Erreur : SemaphoresSetIPC::create");
		return false;
	}
	return true;
}

bool SemaphoresSetIPC::setValue(int numSemaphore, int value) {
	union semun arg;
	arg.val = value;
	if(semctl(semid, numSemaphore, SETVAL, arg) < 0) // Valeur initiale du semaphore : 0
	{
		perror("Erreur : SemaphoresSetIPC::setValue");
		return false;
	}
	return true;
}

// Attend que la valeur du semaphore vaille 0
bool SemaphoresSetIPC::waitForZero(int numSemaphore) {
	struct sembuf tab[1];
	tab[0].sem_num = (short)numSemaphore;
	tab[0].sem_op = 0; // i.e. le processus attend que semval vaille 0
	tab[0].sem_flg = 0;
	if(semop(semid, tab, sizeof(tab) / sizeof(struct sembuf))) {
		perror("Erreur : SemaphoresSetIPC::waitForZero");
		return false;
	}
	return true;
}

bool SemaphoresSetIPC::remove() {
	// NB : le 2nd argument est ignoré
	if(semctl(semid, 0, IPC_RMID) < 0) {
		perror("Erreur : SemaphoresSetIPC::remove");
		return false;
	}
	return true;
}

#endif // !defined WIN32
