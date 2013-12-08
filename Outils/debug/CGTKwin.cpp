// CGTKwin.cpp

#include "CGTKwin.h"
#include "../Commun.h"

#if !defined GTKWIN_DISABLE && !defined RELEASE

// Tout est en vrac ici mais c'est pas grave : vu que c'est uniquement dans le .cpp et inclus dans un namespace qui plus est,
// l'utilisateur ne pourra pas aller trifouiller là dedans et l'encapsulation est ok.
namespace NDebug
{
// Pour chaque type, 2 vectors :
// Un pointeur vers la variable et un widget représentant cette variable
	std::vector<bool*>           ptr_bool;
	std::vector<GtkWidget*>      widget_bool;

	std::vector<float*>          ptr_float;
	std::vector<GtkWidget*>      widget_float;

	std::vector<double*>         ptr_double;
	std::vector<GtkWidget*>      widget_double;

#ifdef GTKWIN_BULLET
	std::vector<btScalar*>       ptr_btscalar;
	std::vector<GtkWidget*>      widget_btscalar;
#endif

	std::vector<unsigned char*>  ptr_uchar;
	std::vector<GtkWidget*>      widget_uchar;

	std::vector<short*>          ptr_short;
	std::vector<GtkWidget*>      widget_short;

	std::vector<unsigned short*> ptr_ushort;
	std::vector<GtkWidget*>      widget_ushort;

	std::vector<int*>            ptr_int;
	std::vector<GtkWidget*>      widget_int;

	std::vector<unsigned int*>   ptr_uint;
	std::vector<GtkWidget*>      widget_uint;

// La fenêtre. Doit être unique, sinon GTK plante.
	CGTKwin * memberGTKwin=NULL;

// Callbacks appelés par GTK lors d'un clic sur les widgets.
// Servent à modifier les données.
	void boolean_fn(GtkWidget *pWidget, gpointer data);
	void float_fn  (GtkWidget *pWidget, gpointer data);
	void double_fn (GtkWidget *pWidget, gpointer data);
#ifdef GTKWIN_BULLET
	void btscalar_fn (GtkWidget *pWidget, gpointer data);
#endif
	void uchar_fn  (GtkWidget *pWidget, gpointer data);
	void short_fn  (GtkWidget *pWidget, gpointer data);
	void ushort_fn (GtkWidget *pWidget, gpointer data);
	void int_fn    (GtkWidget *pWidget, gpointer data);
	void uint_fn   (GtkWidget *pWidget, gpointer data);
// Autres calbacks:
	void quit_fn(void);              // Lors de la fermeture de la fenêtre GTK ou du programme.

	// Lors de l'ouverture du thread.
#ifdef GTKWIN_NO_GLFW
	void* caller(void* arg);	// Version pthread
#else
	void GLFWCALL caller(void* arg);	// Version GLFW
#endif

	gboolean idle_fn( void * arg );  // Lorsque GTK n'a rien à faire. En profite pour mettre à jour les valeurs qui ont p-e changé.


	guint idle_handler_id;  // ID du callback idle_fn
	bool running=false;     // ...
	int taille=0;           // nombre de widgets total

}

// Callback appellé lors de la fermeture de la fenêtre
void NDebug::quit_fn(void){
		running = false;
		gtk_idle_remove(idle_handler_id);  // Il faut arrêter d'appeller le callback idle_fn
		gtk_main_quit();                   // avant de quitter
}

// Callback appellé lors d'un clic sur un bool-widget.
void NDebug::boolean_fn(GtkWidget *pWidget, gpointer data){
	bool*ptr=ptr_bool[(int)data];                                                      // On récupère le pointeur correspondant au widget cliqué
	if ( ptr == NULL ) return;                                                         // ( on sait jamais )
	*ptr = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget_bool[(int)data]));   // MAJ de la valeur en fonction de l'état du bouton
	if (*ptr)gtk_button_set_label(GTK_BUTTON(widget_bool[(int)data]),"true");          // MAJ du bouton en fonction de la valeur
	else gtk_button_set_label(GTK_BUTTON(widget_bool[(int)data]),"false");
}

// Idem pour les autres types !
void NDebug::float_fn(GtkWidget *pWidget, gpointer data){
	float*ptr=ptr_float[(int)data];
	if ( ptr == NULL ) return;
	*ptr = (float)gtk_range_get_value(GTK_RANGE(widget_float[(int)data]));
}

void NDebug::double_fn(GtkWidget *pWidget, gpointer data){
	double*ptr=ptr_double[(int)data];
	if ( ptr == NULL ) return;
	*ptr = (double)gtk_range_get_value(GTK_RANGE(widget_double[(int)data]));
}

#ifdef GTKWIN_BULLET
void NDebug::btscalar_fn(GtkWidget *pWidget, gpointer data){
	btScalar* ptr=ptr_btscalar[(int)data];
	if ( ptr == NULL ) return;
	*ptr = (btScalar)gtk_range_get_value(GTK_RANGE(widget_btscalar[(int)data]));
}
#endif

void NDebug::uchar_fn(GtkWidget *pWidget, gpointer data){
	unsigned char* ptr=ptr_uchar[(int)data];
	if ( ptr == NULL ) return;
	*ptr = (unsigned char)gtk_spin_button_get_value (GTK_SPIN_BUTTON(widget_uchar[(int)data]));
}

void NDebug::short_fn(GtkWidget *pWidget, gpointer data){
	short* ptr=ptr_short[(int)data];
	if ( ptr == NULL ) return;
	*ptr = (short)gtk_spin_button_get_value (GTK_SPIN_BUTTON(widget_short[(int)data]));
}

void NDebug::ushort_fn(GtkWidget *pWidget, gpointer data){
	unsigned short* ptr=ptr_ushort[(int)data];
	if ( ptr == NULL ) return;
	*ptr = (unsigned short)gtk_spin_button_get_value (GTK_SPIN_BUTTON(widget_ushort[(int)data]));
}

void NDebug::int_fn(GtkWidget *pWidget, gpointer data){
	int* ptr=ptr_int[(int)data];
	if ( ptr == NULL ) return;
	*ptr = (int)gtk_spin_button_get_value (GTK_SPIN_BUTTON(widget_int[(int)data]));
}

void NDebug::uint_fn(GtkWidget *pWidget, gpointer data){
	unsigned int* ptr=ptr_uint[(int)data];
	if ( ptr == NULL ) return;
	*ptr = (unsigned int)gtk_spin_button_get_value (GTK_SPIN_BUTTON(widget_uint[(int)data]));
}

// Callback appellé au début du thread. Plus spécifiquement, il s'agit d'un wrapper permettant à une fonction
// prenant en paramètre un (void *fn)(void *arg) de prendre en fait un (void *CGTKwin::fn)(void).
// MAJ 17/12/06
#ifdef GTKWIN_NO_GLFW
void* NDebug::caller(void* arg)	// Version pthread
#else
void GLFWCALL NDebug::caller(void* arg)	// Version GLFW
#endif
{
	gtk_window_set_default_size(GTK_WINDOW(memberGTKwin->get_win()), 320 ,10*taille);
	gtk_widget_show_all(memberGTKwin->get_win());
	// Tout ce qui suit est là uniquement pour positionner joliment la fenêtre.
	int w,h;
	gtk_window_get_size(GTK_WINDOW(memberGTKwin->get_win()),&w,&h);
	gtk_window_resize(GTK_WINDOW(memberGTKwin->get_win()),350,h);
#ifndef GTKWIN_NO_GLFW
	GLFWvidmode desktop;
	glfwGetDesktopMode(&(desktop));
	gtk_window_move(GTK_WINDOW(memberGTKwin->get_win()),desktop.Width-350-10,desktop.Height-h-65);
#endif
	// C'est parti ! On n'a plus à s'occuper de rien, tout se fait désormais via les callbacks.
	running = true;
	gtk_main();

#ifdef GTKWIN_NO_GLFW
	return NULL;
#endif
}

// MAJ les widgets en fonction des variables, de temps à autre ( défini par gtk ... )
gboolean NDebug::idle_fn(void * arg ){
	memberGTKwin->Update();
	return true;
}

NDebug::_CGTKwin::_CGTKwin(){
	thread = 0;
}


// Lance le thread : ouvre la fenêtre réellement, lance la boucle main de GTK.
// Utilise la fonction NDebug::CGTKwin::Go
void NDebug::_CGTKwin::Run(void){
	if (memberGTKwin != this) return;

#ifdef GTKWIN_NO_GLFW
	pthread_create(&thread, NULL, caller, NULL);
#else
	thread = glfwCreateThread(caller, NULL);
#endif
}

// Initialise la fenêtre GTK et tous ses composants de base.
void NDebug::_CGTKwin::Init()
{
//    std::cout << "GTKwin OK";
	if (memberGTKwin != NULL) return; // membreGTKwin ne doit pas encore exister !! si c'est le cas, la fenêtre a déjà été initialisée et on n'a plus rien à faire.
	memberGTKwin = this;              // Init


	gtk_init(0, NULL);
	setlocale(LC_NUMERIC,"C");

	// Création de la fenêtre avec une Vertical Box qui contiendra les onglets (pNotebook)
	pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);


#ifdef GTKWIN_TRACE
	// 1er onglet : le module de tracing
	pTable=gtk_table_new(1,3,TRUE);
#endif
#ifdef GTKWIN_LOG
	// 2ème onglet : la console
	pTextView = gtk_text_view_new();
	// Initialisation de la zone de texte
	pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pTextView));
	gtk_text_view_set_buffer( GTK_TEXT_VIEW(pTextView),pTextBuffer);

	pScrolledWin = gtk_scrolled_window_new(NULL,NULL);
	gtk_container_add(GTK_CONTAINER(pScrolledWin),pTextView);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(pScrolledWin),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
#endif


#if defined GTKWIN_TRACE && defined GTKWIN_LOG
	pVBox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

	// On crée le notebook qui gèrera les onglets
	pNotebook = gtk_notebook_new();
	gtk_box_pack_start(GTK_BOX(pVBox), pNotebook, TRUE, TRUE, 0);
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(pNotebook), GTK_POS_TOP);
	gtk_notebook_set_scrollable(GTK_NOTEBOOK(pNotebook), FALSE);
//#endif
	GtkWidget *pLabel;pLabel = gtk_label_new("Trace");
	// On lui insère une autre VBox ( pTable ) qui contiendra les widgets.
	gtk_notebook_append_page(GTK_NOTEBOOK(pNotebook), pTable, pLabel);
	GtkWidget *pLabel2;pLabel2 = gtk_label_new("Console");
	gtk_notebook_append_page(GTK_NOTEBOOK(pNotebook), pScrolledWin, pLabel2);
	// Caractéristiques de la fenêtre
#else
#ifdef GTKWIN_TRACE
	gtk_container_add(GTK_CONTAINER(pWindow), pScrolledWin);
#endif
#ifdef GTKWIN_LOG
	gtk_container_add(GTK_CONTAINER(pWindow), pTextView);
	gtk_text_view_set_wrap_mode(pTextView, GTK_WRAP_CHAR);
	gtk_text_view_set_justification(pTextView, GTK_JUSTIFY_FILL);
	gtk_text_view_set_border_window_size(pTextView, GTK_WINDOW_POPUP, 100);
#endif
#endif

#ifdef GTKWIN_TRACE
	g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(quit_fn), NULL);
	gtk_table_set_row_spacings(GTK_TABLE(pTable),1);
	//gtk_container_add(GTK_CONTAINER(pWindow), GTK_WIDGET(pTable));
#endif
	gtk_window_set_title(GTK_WINDOW(pWindow), "Fenêtre Debug");
	idle_handler_id = gtk_idle_add(idle_fn, NULL); // Ajout d'une fonction à appeler régulièrement
}

// C'est moi ou ça c'est bourrin ? à revoir.
void NDebug::_CGTKwin::Terminate(void)
{
	if (memberGTKwin != this) return;
#ifdef GTKWIN_NO_GLFW
	pthread_cancel(thread);
#else
	glfwDestroyThread(thread);
#endif
	thread = 0;
}

#ifdef GTKWIN_TRACE
// Ajoute un bool à la liste des variables à tracer.
void NDebug::CGTKwin::Attach(bool& ptr, const std::string& txt)
{
	if (memberGTKwin != this) return;
	// Crée le label ( oui c'est long et chiant )
	gchar* sUtf8 = g_locale_to_utf8(txt.c_str(), -1, NULL, NULL, NULL);
	GtkWidget*pLabel=gtk_label_new(sUtf8);
	g_free(sUtf8);
	gtk_table_attach(GTK_TABLE(pTable), pLabel,0, 1, taille, taille+1,GTK_EXPAND,GTK_EXPAND,0,0);
	gtk_label_set_justify(GTK_LABEL(pLabel),GTK_JUSTIFY_RIGHT);

	// Ajout de la nouvelle variable et du nouveau widget à la liste.
	ptr_bool.push_back(&ptr);
	widget_bool.push_back(gtk_toggle_button_new_with_label(" "));

	// Ajout du widget dans la fenêtre, et de son callback.
	gtk_table_attach(GTK_TABLE(pTable), widget_bool.back(),2, 3, taille, taille+1,GTK_EXPAND,GTK_EXPAND,0,0);
	g_signal_connect(G_OBJECT(widget_bool.back()), "clicked", G_CALLBACK(boolean_fn), (gpointer)(ptr_bool.size()-1));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget_bool.back()),(bool)(ptr));

	taille++;
	gtk_table_resize(GTK_TABLE(pTable),taille+1, 3);
}

// Implémentation générique de Attach() pour tous les types flottants (utilise un GtkHScale)
template <typename T>
void AttachFloat(T& ptr, const std::string& txt, T min, T max, std::vector<T*>& ptr_T, std::vector<GtkWidget*>& widget_T,
				void T_fn(GtkWidget*, gpointer))
{
	GtkWidget* pTable = NDebug::memberGTKwin->get_table();

	// Création du label et ajout à la table
	gchar* sUtf8 = g_locale_to_utf8(txt.c_str(), -1, NULL, NULL, NULL);
	GtkWidget*pLabel=gtk_label_new(sUtf8);
	g_free(sUtf8);
	gtk_table_attach(GTK_TABLE(pTable), pLabel, 0, 1, NDebug::taille, NDebug::taille+1, GTK_EXPAND, GTK_EXPAND, 0, 0);

	// Mémorisation de l'adresse de la variable
	ptr_T.push_back(&ptr);

	// Création, ajout et initialisation du GtkHScale
	widget_T.push_back(gtk_hscale_new_with_range((gdouble)min, (gdouble)max, 0.001));
	gtk_table_attach_defaults(GTK_TABLE(pTable), widget_T.back(), 1, 3, NDebug::taille, NDebug::taille+1);
	g_signal_connect(G_OBJECT(widget_T.back()), "value-changed", G_CALLBACK(T_fn), (gpointer)(ptr_T.size()-1));
	gtk_range_set_value(GTK_RANGE(widget_T.back()),(gdouble)(ptr));

	// Mise à jour de la NDebug::taille de la table :
	NDebug::taille++;
	gtk_table_resize(GTK_TABLE(pTable),NDebug::taille+1, 3);
}

// Implémentation générique de Attach() pour tous les types entiers (utilise un spin button)
template <typename T>
void AttachInteger(	T& ptr, const std::string& txt, T min, T max, std::vector<T*>& ptr_T, std::vector<GtkWidget*>& widget_T,
					void T_fn(GtkWidget*, gpointer))
{
	GtkWidget* pTable = NDebug::memberGTKwin->get_table();

	// Création du label et ajout à la table
	gchar* sUtf8 = g_locale_to_utf8(txt.c_str(), -1, NULL, NULL, NULL);
	GtkWidget*pLabel=gtk_label_new(sUtf8);
	g_free(sUtf8);
	gtk_table_attach(GTK_TABLE(pTable), pLabel, 0, 1, NDebug::taille, NDebug::taille+1, GTK_EXPAND, GTK_EXPAND, 0, 0);

	// Mémorisation de l'adresse de la variable
	ptr_T.push_back(&ptr);

	// Création, ajout et initialisation du spin button
	widget_T.push_back(gtk_spin_button_new_with_range((gdouble)min, (gdouble)max, 1.0));
	gtk_table_attach(GTK_TABLE(pTable), widget_T.back(), 1, 3, NDebug::taille, NDebug::taille+1, GTK_EXPAND,GTK_EXPAND, 1, 1);
	g_signal_connect(G_OBJECT(widget_T.back()), "value-changed", G_CALLBACK(T_fn), (gpointer)(ptr_T.size()-1));
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(widget_T.back()),(gdouble)(ptr));

	// Mise à jour de la NDebug::taille de la table :
	NDebug::taille++;
	gtk_table_resize(GTK_TABLE(pTable),NDebug::taille+1, 3);
}

// Implémentation des Attach() :

// - pour les types flottants :
void NDebug::_CGTKwin::Attach(float& ptr, const std::string& txt, float min, float max)
{
	if (NDebug::memberGTKwin == this)
		AttachFloat(ptr, txt, min, max, ptr_float, widget_float, float_fn);
}

void NDebug::_CGTKwin::Attach(double& ptr, const std::string& txt, double min, double max)
{
	if (NDebug::memberGTKwin == this)
		AttachFloat(ptr, txt, min, max, ptr_double, widget_double, double_fn);
}

#ifdef GTKWIN_BULLET
void NDebug::_CGTKwin::AttachBullet(btScalar& ptr, const std::string& txt, btScalar min, btScalar max)
{
	if (NDebug::memberGTKwin == this)
		AttachFloat(ptr, txt, min, max, ptr_btscalar, widget_btscalar, btscalar_fn);
}
#endif

// - pour les types entiers :

void NDebug::_CGTKwin::Attach(unsigned char& ptr, const std::string& txt, unsigned char min, unsigned char max)
{
	if (NDebug::memberGTKwin == this)
		AttachInteger(ptr, txt, min, max, ptr_uchar, widget_uchar, uchar_fn);
}

void NDebug::_CGTKwin::Attach(short& ptr, const std::string& txt, short min, short max)
{
	if (NDebug::memberGTKwin == this)
		AttachInteger(ptr, txt, min, max, ptr_short, widget_short, short_fn);
}

void NDebug::_CGTKwin::Attach(unsigned short& ptr, const std::string& txt, unsigned short min, unsigned short max)
{
	if (NDebug::memberGTKwin == this)
		AttachInteger(ptr, txt, min, max, ptr_ushort, widget_ushort, ushort_fn);
}

void NDebug::_CGTKwin::Attach(int& ptr, const std::string& txt, int min, int max)
{
	if (NDebug::memberGTKwin == this)
		AttachInteger(ptr, txt, min, max, ptr_int, widget_int, int_fn);
}

void NDebug::_CGTKwin::Attach(unsigned int& ptr, const std::string& txt, unsigned int min, unsigned int max)
{
	if (NDebug::memberGTKwin == this)
		AttachInteger(ptr, txt, min, max, ptr_uint, widget_uint, uint_fn);
}

#endif

#ifdef GTKWIN_LOG
// MAJ : Utilise la fenêtre de log pour afficher du texte.
void NDebug::_CGTKwin::Log(std::string str){
	if (thread == 0) return;
//    GtkTextIter it;
//	str+=" ";  // pffff .. je cherche même pas à comprendre. g_locale_to_utf8 fait tout bugger.
//	gtk_text_buffer_get_end_iter(pTextBuffer, &it);
//	gchar* sUtf8 = g_locale_to_utf8(str.c_str(),str.length() , NULL, NULL , NULL);
//	if (sUtf8[str.length()]=='\n')sUtf8[str.length()-1]='\n';
//	gtk_text_buffer_insert(pTextBuffer, &it, sUtf8, str.length() );
};
#endif

// Met à jour tous les widgets en fonction des valeurs des variables.
// MARCHE ENFIN !
// Sérieux les gens, GTK ça tue.
void NDebug::_CGTKwin::Update(void){

	if (!running) return;
	unsigned int a;
	// bool
	for (a=0;a!=ptr_bool.size();++a){
		bool b=(bool )(*ptr_bool [a]);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget_bool [a]),b);
		gtk_button_set_label(GTK_BUTTON(widget_bool[a]), b ? "true" : "false");
	}
	// float
	for (a=0;a!=ptr_float.size();++a){
		float f = (float)(*ptr_float[a]);
		gtk_range_set_value         (GTK_RANGE        (widget_float[a]),f);
	}
	// double
	for (a=0;a!=ptr_double.size();++a){
		double f = (double)(*ptr_double[a]);
		gtk_range_set_value         (GTK_RANGE        (widget_double[a]),f);
	}
	// btScalar
#ifdef GTKWIN_BULLET
	for (a=0;a!=ptr_btscalar.size();++a){
		btScalar f = (btScalar)(*ptr_btscalar[a]);
		gtk_range_set_value         (GTK_RANGE        (widget_btscalar[a]),f);
	}
#endif
	// uchar
	for (a=0;a!=ptr_uchar.size();++a){
		unsigned char i = (unsigned char)(*ptr_uchar[a]);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget_uchar[a]), (gdouble)i);
	}
	// short
	for (a=0;a!=ptr_short.size();++a){
		short i = (short)(*ptr_short[a]);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget_short[a]), (gdouble)i);
	}
	// ushort
	for (a=0;a!=ptr_ushort.size();++a){
		unsigned short i = (unsigned short)(*ptr_ushort[a]);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget_ushort[a]), (gdouble)i);
	}
	// int
	for (a=0;a!=ptr_int.size();++a){
		int i = (int)(*ptr_int[a]);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget_int[a]), (gdouble)i);
	}
	// uint
	for (a=0;a!=ptr_uint.size();++a){
		unsigned int i = (unsigned int)(*ptr_uint[a]);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget_uint[a]), (gdouble)i);
	}

	// Un petit msleep pour laisser le temps au PC de souffler (et empecher qu'il ne
	// ralentisse tout !)
	msleep(10);

	// Refresh de la fenêtre (pas fait automatiquement ! Bug de GTK ?)
	gint w=0, h=0;
	gtk_window_get_default_size(GTK_WINDOW(pWindow), &w, &h);
	gtk_widget_queue_draw_area(pWindow, 0, 0, w, h);
};

#endif
