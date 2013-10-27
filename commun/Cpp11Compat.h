//
//  Cpp11Compat.h
//  Created on 27/10/2013.
//

#ifndef CPP11COMPAT_H
#define CPP11COMPAT_H

// À utiliser tant que tout le monde ne compilera pas avec la version 2011 du standard.
// Ce fichier sert à autoriser le compilation en C++03 du code, tant que les emprunts à C++11 ne sont pas trop complexes
#if __cplusplus < 201103L

// Petit hack qui permet l'initialisation de constantes statiques dans une classe en C++11.
// Les constantes doivent êtres déclarées avec constexpr à la place de const, mot clé qui n'existe pas dans les standards précédents.

#define constexpr const

#endif


#endif
