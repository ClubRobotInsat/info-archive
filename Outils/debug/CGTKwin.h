/* CGTKwin.h
   Wrapper GTK/glfw pour ouvrir une fenêtre de débug et de tracing en temps réel dans une application openGL
   Arnaud Masserann aka Calvin1602
   Finalisé le 17/12/06

   Exemple d'utilisation:

	NDebug::CGTKwin GTKwin;
	GTKwin.Init();
	GTKwin.Attach(my_int,"un entier",0,255);
	GTKwin.Run();
	GTKwin.Log("Kikooo!");


   TODO:
   Taille de la console qui fait tout bugger !!
   La majorité du travail restant ne concerne plus le C/C++ mais le préprocesseur.
   Il s'agit de permettre à l'utilisateur:
   - d'activer certains onglets mais pas d'autres                                                    // ok !
   - d'utiliser glfw ou non ( mais perte de fonctionnalités: mauvais positionnement de la fenêtre )  // En cours
   - de désactiver toutes les commandes GKTwin en mode release, par modification d'une unique macro. // ok !

   FIXED:
   Update plantait à cause du multithreading ( accès concurrentiel aux variables tracées )
   Fenêtre désactivés en fullscreen.
*/
#include <vector>
#include <string>
#include <iostream>

#ifndef CGTKWIN_H
#define CGTKWIN_H

// ------------------- Début de la section modifiable -------------------------
#define GTKWIN_TRACE    // Active l'onglet de tracing
#define GTKWIN_LOG      // Active l'onglet de log
//#define GTKWIN_DISABLE  // Désactive tout
#define GTKWIN_NO_GLFW  // Expérimental.
//#define GTKWIN_BULLET	// Support du type de donnée btScalar
// -------------------  Fin de la section modifiable  -------------------------


#ifdef GTKWIN_BULLET
#include "LinearMath/btScalar.h"
#endif

#if defined RELEASE || defined FULLSCREEN
#ifndef GTKWIN_DISABLE
#define GTKWIN_DISABLE
#endif
#endif

#ifdef GTKWIN_NO_GLFW
#include <pthread.h>
#else
#include <GL/glfw.h>
#endif

#ifndef GTKWIN_DISABLE
#include <gtk/gtk.h>
#endif

#ifndef GTKWIN_DISABLE
#define CGTKwin _CGTKwin
#else
#define CGTKwin dumbCGTKwin
#endif

namespace NDebug
{
#ifndef GTKWIN_DISABLE
    class _CGTKwin
    {
    public:
        _CGTKwin();
        void Init();
        void Run(void);
#ifdef GTKWIN_TRACE
        void Attach ( bool& ptr , const std::string& txt);
        void Attach ( float& ptr, const std::string& txt, float min, float max);
        void Attach ( double& ptr, const std::string& txt, double min, double max);
#ifdef GTKWIN_BULLET
        void AttachBullet ( btScalar& ptr, const std::string& txt, btScalar min, btScalar max);
#endif
        void Attach ( unsigned char& ptr, const std::string& txt, unsigned char min, unsigned char max);
        void Attach ( short& ptr  , const std::string& txt, short min, short max);
        void Attach ( unsigned short& ptr, const std::string& txt, unsigned short min, unsigned short max);
        void Attach ( int& ptr, const std::string& txt,int min, int max);
        void Attach ( unsigned int& ptr, const std::string& txt, unsigned int min, unsigned int max);
#endif //GTKWIN_TRACE
#ifdef GTKWIN_LOG
        void Log(std::string str);
#endif //GTKWIN_LOG
        void Terminate(void);
		void Update(void);

		inline GtkWidget * get_win(void){return pWindow;}; // utilisé dans le namespace.
		inline GtkWidget * get_table(void){return pTable;}; // utilisé dans le namespace.

	private:

#ifdef GTKWIN_NO_GLFW
        pthread_t thread;
#else
		GLFWthread thread;
#endif

		GtkWidget *pWindow;        // La fenêtre principale
		GtkWidget *pVBox;          // ne contient qu'une VBox
		GtkWidget *pNotebook;      // qui ne contient qu'un notebook ( page à onglets )
#ifdef GTKWIN_TRACE
		GtkWidget *pTable;       // Le premier onglet contient une table de widgets ( ceux contenus dans le vector )
#endif //GTKWIN_TRACE
#ifdef GTKWIN_LOG
		GtkWidget * pScrolledWin;
		GtkWidget * pTextView;   // Le 2ème contient du texte et ce qu'il faut pour le manipuler
		GtkTextBuffer* pTextBuffer;
#endif //GTKWIN_LOG
    };
#else //GTKWIN_DISABLE
    class dumbCGTKwin{
    public:
		void Init(){}
		void Run(void){}
		void Attach ( bool& ptr , const std::string& txt) {}
		void Attach ( float& ptr, const std::string& txt, float min, float max) {}
		void Attach ( double& ptr, const std::string& txt, double min, double max) {}
#ifdef GTKWIN_BULLET
		void AttachBullet ( btScalar& ptr, const std::string& txt, btScalar min, btScalar max) {}
#endif
		void Attach ( unsigned char& ptr, const std::string& txt, unsigned char min, unsigned char max) {}
		void Attach ( short& ptr  , const std::string& txt, short min, short max) {}
		void Attach ( unsigned short& ptr, const std::string& txt, unsigned short min, unsigned short max) {}
		void Attach ( int& ptr, const std::string& txt,int min, int max) {}
		void Attach ( unsigned int& ptr, const std::string& txt, unsigned int min, unsigned int max) {}
		void Log(std::string str){}
		void Terminate(void){}
		void Update(void){}
    };
#endif //GTKWIN_DISABLE
};

#endif //CGTKWIN_H
