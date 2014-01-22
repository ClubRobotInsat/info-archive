//
// CppUtils.h
// projet_xcode
//
// Created by Rémi on 10/01/2014.
//

#ifndef projet_xcode_CppUtils_h
#define projet_xcode_CppUtils_h

#include <memory>
#include <type_traits>

namespace std {
	// Pallie à l'absence de std::make_unique dans C++11, à venir dans C++14 cependant.
	// Source : http://stackoverflow.com/a/13512344/1276072
	
	template <typename T, typename... Args>
	unique_ptr<T> make_unique_helper(false_type, Args&&... args) {
		return unique_ptr<T>(new T(forward<Args>(args)...));
	}
	
	template <typename T, typename... Args>
	unique_ptr<T> make_unique_helper(true_type, Args&&... args) {
		static_assert(extent<T>::value == 0,
			      "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");
		
		return unique_ptr<T>(new typename remove_extent<T>::type[sizeof...(Args)]{forward<Args>(args)...});
	}
	
	template <typename T, typename... Args>
	unique_ptr<T> make_unique(Args&&... args) {
		return make_unique_helper<T>(is_array<T>(), forward<Args>(args)...);
	}
}

#endif
