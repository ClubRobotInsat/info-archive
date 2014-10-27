//
//  Enum.h
//  Club Robot
//
//  Created by Rémi on 27/10/2014.
//

#ifndef Club_Robot_Enum_h
#define Club_Robot_Enum_h


/* 
 * Un peu de magie noire pour permettre de récupérer une valeur d'enum sous forme de chaîne de caracères
 * ENUM(MonEnum,
 * 	 E1,
 *   E2,
 *   E3)
 * va déclarer l'enum MonEnum, avec les valeurs E1, E2, E3.
 * On peu faire de même avec ENUM_CLASS -> enum class.
 * On peut ensuite énumérer les valeurs avec quelque chose du genre :
 * for(auto val : getEnumValues<MonEnum>()) {
 *   MonEnum e = val.first;
 *   std::cout << val.second << std::endl;
 *   std::cout << toString(e) << std::endl; // (Les deux affichages sont équivalents)
 */

#include <map>

#define _NUM_ARGS(X100, X99, X98, X97, X96, X95, X94, X93, X92, X91, X90, X89, X88, X87, X86, X85, X84, X83, X82, X81, X80, X79, X78, X77, X76, X75, X74, X73, X72, X71, X70, X69, X68, X67, X66, X65, X64, X63, X62, X61, X60, X59, X58, X57, X56, X55, X54, X53, X52, X51, X50, X49, X48, X47, X46, X45, X44, X43, X42, X41, X40, X39, X38, X37, X36, X35, X34, X33, X32, X31, X30, X29, X28, X27, X26, X25, X24, X23, X22, X21, X20, X19, X18, X17, X16, X15, X14, X13, X12, X11, X10, X9, X8, X7, X6, X5, X4, X3, X2, X1, N, ...)   N
#define NUM_ARGS(...) _NUM_ARGS(__VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define EXPAND(X)       X
#define FIRSTARG(X, ...)    (X)
#define RESTARGS(X, ...)    (__VA_ARGS__)

#define FOREACH(name, MACRO, LIST)    FOREACH_(NUM_ARGS LIST, name, MACRO, LIST)
#define FOREACH_(N, name, M, LIST)    FOREACH__(N, name, M, LIST)
#define FOREACH__(N, name, M, LIST)   FOREACH_##N(name, M, LIST)
#define FOREACH_1(name, M, LIST)  M (name, LIST)
#define FOREACH_2(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_1(name, M, RESTARGS LIST)
#define FOREACH_3(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_2(name, M, RESTARGS LIST)
#define FOREACH_4(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_3(name, M, RESTARGS LIST)
#define FOREACH_5(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_4(name, M, RESTARGS LIST)
#define FOREACH_6(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_5(name, M, RESTARGS LIST)
#define FOREACH_7(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_6(name, M, RESTARGS LIST)
#define FOREACH_8(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_7(name, M, RESTARGS LIST)
#define FOREACH_9(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_8(name, M, RESTARGS LIST)
#define FOREACH_10(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_9(name, M, RESTARGS LIST)
#define FOREACH_11(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_10(name, M, RESTARGS LIST)
#define FOREACH_12(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_11(name, M, RESTARGS LIST)
#define FOREACH_13(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_12(name, M, RESTARGS LIST)
#define FOREACH_14(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_13(name, M, RESTARGS LIST)
#define FOREACH_15(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_14(name, M, RESTARGS LIST)
#define FOREACH_16(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_15(name, M, RESTARGS LIST)
#define FOREACH_17(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_16(name, M, RESTARGS LIST)
#define FOREACH_18(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_17(name, M, RESTARGS LIST)
#define FOREACH_19(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_18(name, M, RESTARGS LIST)
#define FOREACH_20(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_19(name, M, RESTARGS LIST)
#define FOREACH_21(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_20(name, M, RESTARGS LIST)
#define FOREACH_22(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_21(name, M, RESTARGS LIST)
#define FOREACH_23(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_22(name, M, RESTARGS LIST)
#define FOREACH_24(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_23(name, M, RESTARGS LIST)
#define FOREACH_25(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_24(name, M, RESTARGS LIST)
#define FOREACH_26(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_25(name, M, RESTARGS LIST)
#define FOREACH_27(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_26(name, M, RESTARGS LIST)
#define FOREACH_28(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_27(name, M, RESTARGS LIST)
#define FOREACH_29(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_28(name, M, RESTARGS LIST)
#define FOREACH_30(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_29(name, M, RESTARGS LIST)
#define FOREACH_31(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_30(name, M, RESTARGS LIST)
#define FOREACH_32(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_31(name, M, RESTARGS LIST)
#define FOREACH_33(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_32(name, M, RESTARGS LIST)
#define FOREACH_34(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_33(name, M, RESTARGS LIST)
#define FOREACH_35(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_34(name, M, RESTARGS LIST)
#define FOREACH_36(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_35(name, M, RESTARGS LIST)
#define FOREACH_37(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_36(name, M, RESTARGS LIST)
#define FOREACH_38(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_37(name, M, RESTARGS LIST)
#define FOREACH_39(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_38(name, M, RESTARGS LIST)
#define FOREACH_40(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_39(name, M, RESTARGS LIST)
#define FOREACH_41(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_40(name, M, RESTARGS LIST)
#define FOREACH_42(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_41(name, M, RESTARGS LIST)
#define FOREACH_43(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_42(name, M, RESTARGS LIST)
#define FOREACH_44(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_43(name, M, RESTARGS LIST)
#define FOREACH_45(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_44(name, M, RESTARGS LIST)
#define FOREACH_46(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_45(name, M, RESTARGS LIST)
#define FOREACH_47(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_46(name, M, RESTARGS LIST)
#define FOREACH_48(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_47(name, M, RESTARGS LIST)
#define FOREACH_49(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_48(name, M, RESTARGS LIST)
#define FOREACH_50(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_49(name, M, RESTARGS LIST)
#define FOREACH_51(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_50(name, M, RESTARGS LIST)
#define FOREACH_52(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_51(name, M, RESTARGS LIST)
#define FOREACH_53(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_52(name, M, RESTARGS LIST)
#define FOREACH_54(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_53(name, M, RESTARGS LIST)
#define FOREACH_55(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_54(name, M, RESTARGS LIST)
#define FOREACH_56(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_55(name, M, RESTARGS LIST)
#define FOREACH_57(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_56(name, M, RESTARGS LIST)
#define FOREACH_58(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_57(name, M, RESTARGS LIST)
#define FOREACH_59(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_58(name, M, RESTARGS LIST)
#define FOREACH_60(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_59(name, M, RESTARGS LIST)
#define FOREACH_61(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_60(name, M, RESTARGS LIST)
#define FOREACH_62(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_61(name, M, RESTARGS LIST)
#define FOREACH_63(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_62(name, M, RESTARGS LIST)
#define FOREACH_64(name, M, LIST)  EXPAND(M (name, FIRSTARG LIST)), FOREACH_63(name, M, RESTARGS LIST)

#define STRINGIFY(X)    				#X
#define STRINGIFY2(X, Y)    			EXPAND (STRINGIFY Y)
#define STRINGIFY_LIST(...)   			FOREACH(, STRINGIFY2, (__VA_ARGS__))
#define NAMESPACIZE(X,Y)    			X::EXPAND Y
#define NAMESPACIZE_LIST(name, ...)    	FOREACH(name, NAMESPACIZE, (__VA_ARGS__))

#define TO_ASSOC(X,Y)    				{X::EXPAND Y, EXPAND (STRINGIFY Y)}
#define TO_ASSOC_LIST(name, ...)    	FOREACH(name, TO_ASSOC, (__VA_ARGS__))

#define ENUM_GEN_NO_IMPL(attrib, name, ...) enum attrib name {__VA_ARGS__};\
static std::map<name, char const *> name##StringMap{TO_ASSOC_LIST(name, __VA_ARGS__)};\
static auto const name##TemplateTuple = make_named_enum<name, NAMESPACIZE_LIST(name, __VA_ARGS__)>(STRINGIFY_LIST(__VA_ARGS__));

#define ENUM_IMPL(name, namespace)\
inline char const * toString(namespace::name e) {\
return namespace::name##StringMap[e];\
} \
template<>\
inline std::map<namespace::name, char const *> const &getEnumValues() { return namespace::name##StringMap; }\
template<namespace::name e>\
inline char const *toString() {\
return std::get<std::pair<GetNewType<namespace::name, e, static_cast<int>(e)>, char const *>>(namespace::name##TemplateTuple).second;\
}

#define ENUM_GEN_IMPL(attrib, name, ...) ENUM_GEN_NO_IMPL(attrib, name, __VA_ARGS__)\
ENUM_IMPL(name, )

#define ENUM(...) ENUM_GEN_IMPL(, __VA_ARGS__)
#define ENUM_CLASS(...) ENUM_GEN_IMPL(class, __VA_ARGS__)

#define ENUM_NO_IMPL(...) ENUM_GEN_NO_IMPL(, __VA_ARGS__)
#define ENUM_CLASS_NO_IMPL(...) ENUM_GEN_NO_IMPL(class, __VA_ARGS__)

template<typename Enum>
std::map<Enum, char const*> const &getEnumValues();


template<typename T, T val, int indirection>
struct GetNewType {
	static constexpr T value = val;
};

template<typename T, T val>
struct GetNewType<T, val, 0> {
};

template<typename T = void, T val = static_cast<T>(0), T ...vals>
auto make_named_enum() {
	return std::tuple<>();
}

template<typename T, T val, T ...vals, typename Arg, typename ...Args>
auto make_named_enum(Arg string, Args&&...args) {
	return std::tuple_cat(make_named_enum<T, vals...>(args...), std::make_tuple(std::make_pair(GetNewType<T, val, static_cast<int>(val)>(), string)));
}

#endif
