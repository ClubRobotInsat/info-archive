#ifndef ERREUR_H_
#define ERREUR_H_

#include <exception>
#include <string>

// erreur générique avec message
class Erreur : public std::exception {
public:
	
	// constructeur par defaut (avec un message vide)
	Erreur() : _msg() {}
	
	// constructeur avec un message
	Erreur(std::string msg) : _msg(std::move(msg)) {}
	
	// destructeur
	virtual ~Erreur() throw() {}
	
	// affichage du message
	virtual const char * what() const throw() {return this->_msg.c_str();}
	
	// accesseur
	std::string getMsg() {return _msg;}
	
	// modifieur
	void setMsg(std::string msg) {this->_msg = msg;}
	
private:
	// le message
	std::string _msg;
};

#endif /*ERREUR_H_*/

// écriture allégée pour définir une sous-classe d'exception.
#define EXCEPTION_CLASS(name) class name : public Erreur \
      {public: name(Str s) : Erreur(std::move(s)) {}};
               //name(const char* s) : Erreur(Str(s)) {}};