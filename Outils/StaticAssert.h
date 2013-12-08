// StaticAssert.h
// Un assert statique résolu à la compilation.
// Utilisation : STATIC_ASSERT(expression);
// Doit être utilisé dans une fonction. Si l'on veut l'utiliser en global,
// il suffit de créer une fonction statique, ou plutot une structure pour éviter des warnings.
// Ex :
// struct __ValidateSize__
// {
//     __ValidateSize__()
//     {
//         STATIC_ASSERT(sizeof(int) == 4);
//     }
// };

#ifndef STATIC_ASSERT_H
#define STATIC_ASSERT_H

namespace StaticAssert
{
	template<bool> struct FAILED;
	template<>     struct FAILED< true > {};
}

#define STATIC_ASSERT(exp) (StaticAssert::FAILED< (exp) != 0 >())

#endif // STATIC_ASSERT_H
