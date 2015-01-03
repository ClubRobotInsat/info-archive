//
//  Enum.h
//  Club Robot
//
//  Created by Rémi on 27/10/2014.
//

#ifndef Club_Robot_Enum_h
#define Club_Robot_Enum_h

#include <type_traits>
#include <iostream>

/**
 * Convertit une valeur d'une enum (enum Enum ou enum class Enum) vers la valeur entière correspondante.
 * Une énumération définie avec un certain type entier sera convertie vers ce même type entier :
 * enum MonEnum : std::uint8_t { MaValeur = 42};
 * alors enumToInt(MaValeur) sera un std::uint8_t de valeur 42.
 * @param e L'instance du type énuméré à convertir
 * @return La valeur entière correspondant à l'enum passée en paramètre
 */
template<typename Enum>
std::enable_if_t<std::is_enum<Enum>::value, std::underlying_type_t<Enum>>
constexpr enumToInt(Enum e) {
	return static_cast<std::underlying_type_t<Enum>>(e);
}


/**
 * Permet de créer un namespace et une structure avec le même contenu.
 * Pas super super propre, mais on peut faire un using namespace avec le namepace, et passer la structure en paramètre template avec la struct.
 * @param STRUCT_NAME Le nom de la structur à générer
 * @param NAMESPACE_NAME Le nom du namespace à générer
 * @param ... Le contenu de la struct/du namespace entre accolades
 * @return Une structure de nom STRUCT_NAME et un namespace de nom NAMESPACE_NAME avec le contenu passé en 3e argument
 */

#define STRUCT_NAMESPACE(STRUCT_NAME, NAMESPACE_NAME, ...) \
struct STRUCT_NAME __VA_ARGS__; \
namespace NAMESPACE_NAME __VA_ARGS__


/**
 * Instancie explicitement la structure template passée en argument.
 * Cette structure doit avoir été déclarée dans le corps d'une structure STRUCT et d'un namespace NAMESPACE, à l'aide par exemple de la macro STRUCT_NAMESPACE.
 * @param STRUCT Le nom de la structure dans laquelle est déclarée la structure à instancier
 * @param NAMESPACE Le nom du namespace dans lequel est déclarée la structure à instancier
 * @param ... La structure template à instancier (sans le mot clé struct) précédant le nom de la structure
 * @return Une instanciation explicite dans le namespace et dans la structure avec le code fourni
 */
#define EXPLICIT_INSTANCIATION(STRUCT, NAMESPACE, ...) \
template<> struct STRUCT::__VA_ARGS__ \
namespace NAMESPACE { \
template<> struct __VA_ARGS__ \
}

/* 
 * Un peu de magie noire pour permettre de récupérer une valeur d'enum sous forme de chaîne de caracères.
 * Voir la fin du fichier pour des exemples d'utilisation.
 */

#include <map>
#include <vector>
#include "Enum.hpp"

/**
 * Définit une enum de nom NAME, et avec les valeurs contenues dans ..., comme si déclarée ainsi : enum NAME {...};
 * En plus de l'enum, 3 fonctions sont déclarées : getEnumValue (voir ci-dessus), toString, et operator<<(std::ostream &).
 * Ne convient pas pour déclarer une enum dans un namespace, pour cela utiliser les macros ENUM_NS et ENUM_CLASS_NS (erreur de compilation sinon)
 *
 * Voir l'exemple en fin de fichier pour comprendre leur utilité et leur utilisation.
 *
 * @param NAME Le nom de l'enum à déclarer
 * @param ... Les valeurs de l'énumération, séparées par des virgules. Ex : MonEnum1, MonEnum2 etc.
 * @return Le code correspondant à la génération de l'enum, ainsi qu'aux 3 fonctions citées ci-dessus.
 */
#define ENUM(NAME, ...) ENUM_GEN_IMPL(, NAME, __VA_ARGS__)

/**
 * Exactement la même chose qu'au dessus, mis à part que l'enum est désormais une enum class (fortement typée, englobée dans sa propre portée).
 */
#define ENUM_CLASS(NAME, ...) ENUM_GEN_IMPL(class, NAME, __VA_ARGS__)


/**
 * Comme au dessus, sauf que c'est destiné à des enums qu'on déclare dans un namespace, dont on doit spécifier le nom.
 * Ne gère qu'un seul niveau de namespace, i.e namespace NS1 { namespace NS2 { --- déclaration ici --- } } ne peut pas fonctionner (erreur de compilation)
 *
 * @param NS Le nom du namespace dans lequel est déclarée l'enum
 * @param NAME Le nom de l'enum à déclarer
 * @param ... Les valeurs de l'énumération, séparées par des virgules. Ex : MonEnum1, MonEnum2 etc.
 * @return Le code correspondant à la génération de l'enum, ainsi qu'aux 3 fonctions citées ci-dessus.
 */

#define ENUM_NS(NS, NAME, ...) ENUM_GEN_IMPL_NS(, NS, NAME, __VA_ARGS__)
#define ENUM_CLASS_NS(NS, NAME, ...) ENUM_GEN_IMPL_NS(class, NS, NAME, __VA_ARGS__)



/************* Exemples *************/
#if 0

// On créé une enum class. Utiliser ENUM aurait créé une enum simple.
ENUM_CLASS(MonEnum, Foo, Bar, Baz);

namespace NS {
	// On créé une enum class dans un namespace. Obligation d'utiliser la macro *_NS, sinon erreur de compilation.
	// Le premier paramètre de la macro doit être le nom du namespace dans lequel on est.
	ENUM_CLASS_NS(NS, MonEnum, Na, NaNa, NaNaNa, NaNaNaNa, Batman);
}

// Test de MonEnum définie dans le namespace global (définie avec ENUM_CLASS)
{
	MonEnum e = MonEnum::Foo; // MonEnum est une enum class comme une autre, on en manipule les valeurs comme n'importe quelle autre enum

	std::cout << e << std::endl; // Affiche la chaîne "Foo\n" sur la sortie standard
	std::cout << toString(e) << std::endl; // Idem

	char const *cString = toString(e); // Pas de copie, on récupère une chaîne constante et partagée
	std::string cppString = toString(e); // Création à partie de la chaîne partagée, modification à volonté ensuite

	// getEnumValues<MonEnum>() récupère toutes les valeur de l'enum MonEnum
	for(auto ee : getEnumValues<MonEnum>()) {
		std::cout << ee << " : " << enumToInt(ee) << std::endl; // Affiche 3 lignes : "Foo : 0", "Bar : 1" et "Baz : 2".
	}
	}

	// Test de MonEnum définie dans le namespace NS (définie avec ENUM_CLASS_NS(NS, …))
	{
		NS::MonEnum e = NS::MonEnum::Batman; // NS::MonEnum est une enum class comme une autre, on en manipule les valeurs comme n'importe quelle autre enum

		std::cout << e << std::endl; // Affiche la chaîne "Batman\n" sur la sortie standard
		std::cout << toString(e) << std::endl; // Idem

		char const *cString = toString(NS::MonEnum::Na); // Pas de copie, on récupère une chaîne constante et partagée
		std::string cppString = toString(NS::MonEnum::NaNaNa); // Création à partie de la chaîne partagée, modification à volonté ensuite

		// getEnumValues<NS::MonEnum>() récupère toutes les valeur de l'enum NS::MonEnum
		for(auto ee : getEnumValues<NS::MonEnum>()) {
			std::cout << ee << " : " << enumToInt(ee) << std::endl; // Affiche 5 lignes : "Na : 0", "NaNa : 1", "NaNaNa : 2", "NaNaNaNa : 3" et "Batman : 4".
		}
	}


#endif


#endif
