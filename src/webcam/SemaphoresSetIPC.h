// SemaphoreIPC.h - classe représentant un ensemble de semaphores identifiés par le
// même semid pour le système Inter-Process Communication de UNIX System V.
// Doc :
// http://bat710.univ-lyon1.fr/~jciehl/Public/MAN/man5/ipc.5.html
// http://linuxgazette.net/104/ramankutty.html
// http://www.labri.fr/perso/billaud/travaux/SYSRESEAU/HTML/sysreseau-15.html

#ifndef SEMAPHORES_SET_IPC_H
#define SEMAPHORES_SET_IPC_H

#ifndef WIN32

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#if /*defined(__GNU_LIBRARY__) &&*/ !defined(_SEM_SEMUN_UNDEFINED)
/* l'union semun est définie par l'inclusion de <sys/sem.h> */
#else
/* d'après X/OPEN il faut la définir nous-mêmes */
union semun {
	int val;               /* valeur pour SETVAL */
	struct semid_ds* buf;  /* buffer pour IPC_STAT, IPC_SET */
	unsigned short* array; /* table  pour GETALL, SETALL */
	                       /* Spécificité Linux : */
	struct seminfo* __buf; /* buffer pour IPC_INFO */
};
#endif


class SemaphoresSetIPC {
private:
	int semid;        // ID du semaphore
	int nbSemaphores; // Nombre de semaphores associés à ce semid

public:
	SemaphoresSetIPC() : semid(0), nbSemaphores(0) {}

	~SemaphoresSetIPC() {}

	bool create(key_t key, int nbSemaphores);
	bool setValue(int numSemaphore, int value);
	bool waitForZero(int numSemaphore); // attend que la valeur du semaphore vaille 0
	bool remove();

	void setID(int semid) {
		this->semid = semid;
	}
	int getID() {
		return semid;
	}
};

#endif // !defined WIN32

#endif // SEMAPHORES_SET_IPC_H
