//
//  Enum.hpp
//  Club Robot
//
//  Created by Rémi on 03/01/2015.
//

/**
 * Un conseil étranger, passe ton chemin.
 * Du début jusqu'à la fin des foreach, les macros servent à itérer sur un nombre variable d'éléments.
 * Ce nombre variable est compris entre 0 et 100, je te laisse comprendre le motif pour permettre des itérations plus
 * longues (modifier les macros _NUM_ARGS, NUM_ARGS et FOREACH_XXX
 *
 * Les macros STRIGIFY, NAMESPACIZE et TO_ASSOC sont des macros utilitaires qui permettent à partir d'un identificateur,
 * d'en générer la version texte et la version qualifiée d'un namespace.
 *
 * Le reste consiste en des détails d'implémentation des fonctions getEnumValue et toString.
 */

#include <vector>

#define _NUM_ARGS(X100, \
                  X99,  \
                  X98,  \
                  X97,  \
                  X96,  \
                  X95,  \
                  X94,  \
                  X93,  \
                  X92,  \
                  X91,  \
                  X90,  \
                  X89,  \
                  X88,  \
                  X87,  \
                  X86,  \
                  X85,  \
                  X84,  \
                  X83,  \
                  X82,  \
                  X81,  \
                  X80,  \
                  X79,  \
                  X78,  \
                  X77,  \
                  X76,  \
                  X75,  \
                  X74,  \
                  X73,  \
                  X72,  \
                  X71,  \
                  X70,  \
                  X69,  \
                  X68,  \
                  X67,  \
                  X66,  \
                  X65,  \
                  X64,  \
                  X63,  \
                  X62,  \
                  X61,  \
                  X60,  \
                  X59,  \
                  X58,  \
                  X57,  \
                  X56,  \
                  X55,  \
                  X54,  \
                  X53,  \
                  X52,  \
                  X51,  \
                  X50,  \
                  X49,  \
                  X48,  \
                  X47,  \
                  X46,  \
                  X45,  \
                  X44,  \
                  X43,  \
                  X42,  \
                  X41,  \
                  X40,  \
                  X39,  \
                  X38,  \
                  X37,  \
                  X36,  \
                  X35,  \
                  X34,  \
                  X33,  \
                  X32,  \
                  X31,  \
                  X30,  \
                  X29,  \
                  X28,  \
                  X27,  \
                  X26,  \
                  X25,  \
                  X24,  \
                  X23,  \
                  X22,  \
                  X21,  \
                  X20,  \
                  X19,  \
                  X18,  \
                  X17,  \
                  X16,  \
                  X15,  \
                  X14,  \
                  X13,  \
                  X12,  \
                  X11,  \
                  X10,  \
                  X9,   \
                  X8,   \
                  X7,   \
                  X6,   \
                  X5,   \
                  X4,   \
                  X3,   \
                  X2,   \
                  X1,   \
                  N,    \
                  ...)  \
	N
#define NUM_ARGS(...)      \
	_NUM_ARGS(__VA_ARGS__, \
	          100,         \
	          99,          \
	          98,          \
	          97,          \
	          96,          \
	          95,          \
	          94,          \
	          93,          \
	          92,          \
	          91,          \
	          90,          \
	          89,          \
	          88,          \
	          87,          \
	          86,          \
	          85,          \
	          84,          \
	          83,          \
	          82,          \
	          81,          \
	          80,          \
	          79,          \
	          78,          \
	          77,          \
	          76,          \
	          75,          \
	          74,          \
	          73,          \
	          72,          \
	          71,          \
	          70,          \
	          69,          \
	          68,          \
	          67,          \
	          66,          \
	          65,          \
	          64,          \
	          63,          \
	          62,          \
	          61,          \
	          60,          \
	          59,          \
	          58,          \
	          57,          \
	          56,          \
	          55,          \
	          54,          \
	          53,          \
	          52,          \
	          51,          \
	          50,          \
	          49,          \
	          48,          \
	          47,          \
	          46,          \
	          45,          \
	          44,          \
	          43,          \
	          42,          \
	          41,          \
	          40,          \
	          39,          \
	          38,          \
	          37,          \
	          36,          \
	          35,          \
	          34,          \
	          33,          \
	          32,          \
	          31,          \
	          30,          \
	          29,          \
	          28,          \
	          27,          \
	          26,          \
	          25,          \
	          24,          \
	          23,          \
	          22,          \
	          21,          \
	          20,          \
	          19,          \
	          18,          \
	          17,          \
	          16,          \
	          15,          \
	          14,          \
	          13,          \
	          12,          \
	          11,          \
	          10,          \
	          9,           \
	          8,           \
	          7,           \
	          6,           \
	          5,           \
	          4,           \
	          3,           \
	          2,           \
	          1)
#define EXPAND(X) X
#define FIRSTARG(X, ...) (X)
#define RESTARGS(X, ...) (__VA_ARGS__)

#define FOREACH(name, MACRO, LIST) FOREACH_(NUM_ARGS LIST, name, MACRO, LIST)
#define FOREACH_(N, name, M, LIST) FOREACH__(N, name, M, LIST)
#define FOREACH__(N, name, M, LIST) FOREACH_##N(name, M, LIST)
#define FOREACH_1(name, M, LIST) M(name, LIST)
#define FOREACH_2(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_1(name, M, RESTARGS LIST)
#define FOREACH_3(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_2(name, M, RESTARGS LIST)
#define FOREACH_4(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_3(name, M, RESTARGS LIST)
#define FOREACH_5(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_4(name, M, RESTARGS LIST)
#define FOREACH_6(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_5(name, M, RESTARGS LIST)
#define FOREACH_7(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_6(name, M, RESTARGS LIST)
#define FOREACH_8(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_7(name, M, RESTARGS LIST)
#define FOREACH_9(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_8(name, M, RESTARGS LIST)
#define FOREACH_10(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_9(name, M, RESTARGS LIST)
#define FOREACH_11(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_10(name, M, RESTARGS LIST)
#define FOREACH_12(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_11(name, M, RESTARGS LIST)
#define FOREACH_13(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_12(name, M, RESTARGS LIST)
#define FOREACH_14(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_13(name, M, RESTARGS LIST)
#define FOREACH_15(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_14(name, M, RESTARGS LIST)
#define FOREACH_16(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_15(name, M, RESTARGS LIST)
#define FOREACH_17(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_16(name, M, RESTARGS LIST)
#define FOREACH_18(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_17(name, M, RESTARGS LIST)
#define FOREACH_19(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_18(name, M, RESTARGS LIST)
#define FOREACH_20(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_19(name, M, RESTARGS LIST)
#define FOREACH_21(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_20(name, M, RESTARGS LIST)
#define FOREACH_22(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_21(name, M, RESTARGS LIST)
#define FOREACH_23(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_22(name, M, RESTARGS LIST)
#define FOREACH_24(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_23(name, M, RESTARGS LIST)
#define FOREACH_25(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_24(name, M, RESTARGS LIST)
#define FOREACH_26(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_25(name, M, RESTARGS LIST)
#define FOREACH_27(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_26(name, M, RESTARGS LIST)
#define FOREACH_28(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_27(name, M, RESTARGS LIST)
#define FOREACH_29(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_28(name, M, RESTARGS LIST)
#define FOREACH_30(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_29(name, M, RESTARGS LIST)
#define FOREACH_31(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_30(name, M, RESTARGS LIST)
#define FOREACH_32(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_31(name, M, RESTARGS LIST)
#define FOREACH_33(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_32(name, M, RESTARGS LIST)
#define FOREACH_34(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_33(name, M, RESTARGS LIST)
#define FOREACH_35(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_34(name, M, RESTARGS LIST)
#define FOREACH_36(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_35(name, M, RESTARGS LIST)
#define FOREACH_37(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_36(name, M, RESTARGS LIST)
#define FOREACH_38(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_37(name, M, RESTARGS LIST)
#define FOREACH_39(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_38(name, M, RESTARGS LIST)
#define FOREACH_40(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_39(name, M, RESTARGS LIST)
#define FOREACH_41(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_40(name, M, RESTARGS LIST)
#define FOREACH_42(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_41(name, M, RESTARGS LIST)
#define FOREACH_43(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_42(name, M, RESTARGS LIST)
#define FOREACH_44(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_43(name, M, RESTARGS LIST)
#define FOREACH_45(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_44(name, M, RESTARGS LIST)
#define FOREACH_46(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_45(name, M, RESTARGS LIST)
#define FOREACH_47(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_46(name, M, RESTARGS LIST)
#define FOREACH_48(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_47(name, M, RESTARGS LIST)
#define FOREACH_49(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_48(name, M, RESTARGS LIST)
#define FOREACH_50(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_49(name, M, RESTARGS LIST)
#define FOREACH_51(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_50(name, M, RESTARGS LIST)
#define FOREACH_52(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_51(name, M, RESTARGS LIST)
#define FOREACH_53(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_52(name, M, RESTARGS LIST)
#define FOREACH_54(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_53(name, M, RESTARGS LIST)
#define FOREACH_55(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_54(name, M, RESTARGS LIST)
#define FOREACH_56(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_55(name, M, RESTARGS LIST)
#define FOREACH_57(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_56(name, M, RESTARGS LIST)
#define FOREACH_58(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_57(name, M, RESTARGS LIST)
#define FOREACH_59(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_58(name, M, RESTARGS LIST)
#define FOREACH_60(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_59(name, M, RESTARGS LIST)
#define FOREACH_61(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_60(name, M, RESTARGS LIST)
#define FOREACH_62(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_61(name, M, RESTARGS LIST)
#define FOREACH_63(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_62(name, M, RESTARGS LIST)
#define FOREACH_64(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_63(name, M, RESTARGS LIST)
#define FOREACH_65(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_64(name, M, RESTARGS LIST)
#define FOREACH_66(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_65(name, M, RESTARGS LIST)
#define FOREACH_67(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_66(name, M, RESTARGS LIST)
#define FOREACH_68(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_67(name, M, RESTARGS LIST)
#define FOREACH_69(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_68(name, M, RESTARGS LIST)
#define FOREACH_70(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_69(name, M, RESTARGS LIST)
#define FOREACH_71(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_70(name, M, RESTARGS LIST)
#define FOREACH_72(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_71(name, M, RESTARGS LIST)
#define FOREACH_73(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_72(name, M, RESTARGS LIST)
#define FOREACH_74(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_73(name, M, RESTARGS LIST)
#define FOREACH_75(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_74(name, M, RESTARGS LIST)
#define FOREACH_76(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_75(name, M, RESTARGS LIST)
#define FOREACH_77(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_76(name, M, RESTARGS LIST)
#define FOREACH_78(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_77(name, M, RESTARGS LIST)
#define FOREACH_79(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_78(name, M, RESTARGS LIST)
#define FOREACH_80(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_79(name, M, RESTARGS LIST)
#define FOREACH_81(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_80(name, M, RESTARGS LIST)
#define FOREACH_82(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_81(name, M, RESTARGS LIST)
#define FOREACH_83(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_82(name, M, RESTARGS LIST)
#define FOREACH_84(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_83(name, M, RESTARGS LIST)
#define FOREACH_85(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_84(name, M, RESTARGS LIST)
#define FOREACH_86(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_85(name, M, RESTARGS LIST)
#define FOREACH_87(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_86(name, M, RESTARGS LIST)
#define FOREACH_88(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_87(name, M, RESTARGS LIST)
#define FOREACH_89(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_88(name, M, RESTARGS LIST)
#define FOREACH_90(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_89(name, M, RESTARGS LIST)
#define FOREACH_91(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_90(name, M, RESTARGS LIST)
#define FOREACH_92(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_91(name, M, RESTARGS LIST)
#define FOREACH_93(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_92(name, M, RESTARGS LIST)
#define FOREACH_94(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_93(name, M, RESTARGS LIST)
#define FOREACH_95(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_94(name, M, RESTARGS LIST)
#define FOREACH_96(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_95(name, M, RESTARGS LIST)
#define FOREACH_97(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_96(name, M, RESTARGS LIST)
#define FOREACH_98(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_97(name, M, RESTARGS LIST)
#define FOREACH_99(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_98(name, M, RESTARGS LIST)
#define FOREACH_100(name, M, LIST) EXPAND(M(name, FIRSTARG LIST)), FOREACH_99(name, M, RESTARGS LIST)

#define STRINGIFY(X) #X
#define STRINGIFY2(X, Y) EXPAND(STRINGIFY Y)
#define NAMESPACIZE(X, Y) X::EXPAND Y
#define NAMESPACIZE_LIST(name, ...) FOREACH(name, NAMESPACIZE, (__VA_ARGS__))

#define TO_QUALIFIED(X, Y) X::EXPAND Y
#define TO_QUALIFIED_LIST(name, ...) FOREACH(name, TO_QUALIFIED, (__VA_ARGS__))

#define TO_ASSOC(X, Y) \
	{ X::EXPAND Y, EXPAND(STRINGIFY Y) }
#define TO_ASSOC_LIST(name, ...) FOREACH(name, TO_ASSOC, (__VA_ARGS__))


/**
 * Déclare et définit les objets stockant l'association enum/string, mais ne définit pas les fonctions permettant d'y
 * accéder.
 */
#define ENUM_GEN_NO_IMPL(attrib, name, ...)                                               \
	enum attrib name { __VA_ARGS__ };                                                     \
	static std::map<name, char const*> name##StringMap{TO_ASSOC_LIST(name, __VA_ARGS__)}; \
	static std::vector<name> name##EnumVector{TO_QUALIFIED_LIST(name, __VA_ARGS__)};

/**
 * Fonction template donnant l'accès aux valeurs d'une enum. Elle est spécialisée pour toutes les enums déclarées avec
 * les macros ENUM_...
 */
template <typename Enum>
std::vector<Enum> const& getEnumValues();

template <typename Enum>
inline bool getFromString(const std::string &str, Enum &output) {
	for (auto value : getEnumValues<Enum>()) {
		if (toString(value) == str) {
			output = value;
			return true;
		}
	}
	return false;
}

/**
 * Définit les fonctions getEnumValue, toString et operator<<(std::ostream &) pour l'enum demandée.
 * Cette définition se fait de manière automatique par les macros ENUM et ENUM_CLASS, il ne devrait pas avoir besoin
 * d'utiliser cette macro manuellement ailleurs.
 */
#define ENUM_IMPL(name, Namespace)                                        \
	inline char const* toString(Namespace::name e) {                      \
		return Namespace::name##StringMap[e];                             \
	}                                                                     \
	inline std::ostream& operator<<(std::ostream& s, Namespace::name e) { \
		s << toString(e);                                                 \
		return s;                                                         \
	}                                                                     \
	template <>                                                           \
	inline std::vector<Namespace::name> const& getEnumValues() {          \
		return Namespace::name##EnumVector;                               \
	}

/**
 * Définit les fonctions getEnumValue, toString et operator<<(std::ostream &) pour l'enum demandée, déclarée dans le
 * namespace donné.
 * Cette définition se fait de manière automatique par les macros ENUM_NS et ENUM_CLASS_NS, il ne devrait pas avoir
 * besoin d'utiliser cette macro manuellement ailleurs.
 */
#define ENUM_IMPL_NS(name, Namespace)                                     \
	inline char const* toString(Namespace::name e) {                      \
		return Namespace::name##StringMap[e];                             \
	}                                                                     \
	inline std::ostream& operator<<(std::ostream& s, Namespace::name e) { \
		s << toString(e);                                                 \
		return s;                                                         \
	}                                                                     \
	}                                                                     \
	template <>                                                           \
	inline std::vector<Namespace::name> const& getEnumValues() {          \
		return Namespace::name##EnumVector;                               \
	}                                                                     \
	namespace Namespace {


/**
 * Définit les objets stockant l'association enum/string, et définit les fonctions permettant d'y accéder.
 */
#define ENUM_GEN_IMPL(attrib, name, ...)        \
	ENUM_GEN_NO_IMPL(attrib, name, __VA_ARGS__) \
	ENUM_IMPL(name, )

#define ENUM_GEN_IMPL_NS(attrib, Namespace, name, ...) \
	ENUM_GEN_NO_IMPL(attrib, name, __VA_ARGS__)        \
	ENUM_IMPL_NS(name, Namespace)
