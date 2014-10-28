#ifndef ERREUR_H_
#define ERREUR_H_

#include <exception>
#include <string>

// erreur générique avec message
class Erreur : public std::exception {
public:
	
	// constructeur par defaut (avec un message vide)
	Erreur() : msg() {}
	
	// constructeur avec un message
	Erreur(std::string msg) : msg(msg) {}
	
	// destructeur
	virtual ~Erreur() throw() {}
	
	// affichage du message
	virtual const char * what() const throw() {return this->msg.c_str();}
	
	// accesseur
	std::string getMsg() {return msg;}
	
	// modifieur
	void setMsg(std::string msg) {this->msg = msg;}
	
private:
	// le message
	std::string msg;
};

#endif /*ERREUR_H_*/
