// Action.h

#ifndef ACTION_H
#define ACTION_H

#include <pthread.h>
#include <errno.h>
#include "Utils.h"

enum ErreurAction
{
	OK,
	ERREUR_TIMEOUT,
	ERREUR_AUTRE
};

template <class CarteDerivee, typename TypeRetour>
class Action
{
private:
	static const int DEFAULT_TIMEOUT = 500;	// en millisecondes

private:
	pthread_mutex_t mutex;
	pthread_cond_t cond;

	CarteDerivee* carte;
	void       (CarteDerivee::*ActualiserFunc)();
	TypeRetour (CarteDerivee::*LireValeurFunc)();

public:
	Action(
			CarteDerivee* carte,
			void       (CarteDerivee::*ActualiserFunc)(),
			TypeRetour (CarteDerivee::*LireValeurFunc)()
		)
		: carte(carte),
		  ActualiserFunc(ActualiserFunc),
		  LireValeurFunc(LireValeurFunc)
	{
		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&cond, NULL);
	}

	~Action()
	{
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
	}

	TypeRetour Executer(ErreurAction* erreur=NULL)
	{
		// Lock
		pthread_mutex_lock(&mutex);

		// On lance une actualisation
		(carte->*ActualiserFunc)();

		// Ici, l'élec est susceptible de répondre, mais le thread
		// de réception est bloqué par notre lock initial

		// Préparation du timeout :
		timespec timeout;
		RemplirTimespec(DEFAULT_TIMEOUT, &timeout);

		// On attend que le thread de réception nous envoie un signal
		// (ou timeout)
		// NB : pthread_cond_wait fait atomiquement :
		// 1 : unlock mutex
		// 2 : attente d'un signal ou broadcast
		// 3 : lock mutex
		int pthreadError = pthread_cond_timedwait(&cond, &mutex, &timeout);

		pthread_mutex_unlock(&mutex);

		// On regarde s'il y a eu une erreur
		if(erreur != NULL)
		{
			switch(pthreadError)
			{
				case 0:	// pas d'erreur
					break;
				case ETIMEDOUT:
					*erreur = ERREUR_TIMEOUT;
					break;
				case EINVAL:
				case EPERM:
				default:
					*erreur = ERREUR_AUTRE;
					break;
			}
		}

		// TODO : que renvoyer s'il y a timeout ??

		// On renvoie le résultat
		return (carte->*LireValeurFunc)();
	}

	void MessageRecu()
	{
		pthread_mutex_lock(&mutex);

		pthread_cond_signal(&cond);

		pthread_mutex_unlock(&mutex);
	}
};

#endif // ACTION_H
