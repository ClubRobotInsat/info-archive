// WebcamSimuIPC.h - implémentation de l'interface IWebcam utilisant la shared memory et les semaphores System V

#ifndef WEBCAM_SIMU_IPC_H
#define WEBCAM_SIMU_IPC_H

#ifdef WIN32
typedef WebcamDummy WebcamSimuIPC;
#else

#include "IWebcam.h"
#include "SemaphoresSetIPC.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

class WebcamSimuIPC : public IWebcam {
private:
	// Données de l'image
	unsigned char* m_data;
	int m_width, m_height;

	bool m_reversed_image_mode;

	// Parties propres à l'utilisation de IPC
	typedef int SharedMemID;

	SharedMemID m_shmid;           // ID de la shared memory
	SemaphoresSetIPC m_semaphores; // Le 1er semaphore est utilisé pour
	                               // ne pas avoir de pb de désynchronisation
public:
	WebcamSimuIPC();
	virtual ~WebcamSimuIPC();

	virtual bool Init(const char* device); // device est le nom du fichier texte contenant la clef à utiliser
	virtual void Close();
	virtual bool WaitForImageSync(); // Fonction qui attend qu'une image soit arrivée depuis la webcam
	virtual bool QueryImage();       // Fonction qui demande à la webcam de capturer une nouvelle image

	virtual unsigned char* GetData() {
		return m_data;
	}
	virtual const unsigned char* GetData() const {
		return m_data;
	}
	virtual int GetWidth() const {
		return m_width;
	}
	virtual int GetHeight() const {
		return m_height;
	}

	virtual void SetReversedImageMode(bool reversed); // Fonction qui passe au mode image inversée ou normale
	virtual bool IsReversedImageModeSet() const {
		return m_reversed_image_mode;
	}

	virtual WebcamType GetType() const {
		return WEBCAM_IPC;
	}

	// Fonction qui retourne verticalement l'image pointée par m_data
protected:
	void ReverseData();
};

#endif // !defined WIN32

#endif // WEBCAM_SIMU_IPC_H
