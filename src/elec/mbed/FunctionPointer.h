//
// Created by paul on 24/04/18.
//

#ifndef ROOT_FUNCTIONPOINTER_H
#define ROOT_FUNCTIONPOINTER_H

#include "Callback.h"
#include <stdint.h>
#include <string.h>


// Declarations for backwards compatibility
// To be foward compatible, code should adopt the Callback class
template <typename R, typename A1>
class FunctionPointerArg1 : public Callback<R(A1)> {
public:
	R call(A1 a1) const {
		if(!Callback<R(A1)>::operator bool()) {
			return (R)0;
		}

		return Callback<R(A1)>::call(a1);
	}

	R operator()(A1 a1) const {
		return Callback<R(A1)>::call(a1);
	}
};

template <typename R>
class FunctionPointerArg1<R, void> : public Callback<R()> {
public:
	R (*get_function())() {
		return *reinterpret_cast<R (**)()>(this);
	}

	R call() const {
		if(!Callback<R()>::operator bool()) {
			return (R)0;
		}

		return Callback<R()>::call();
	}

	R operator()() const {
		return Callback<R()>::call();
	}
};

typedef FunctionPointerArg1<void, void> FunctionPointer;

#endif // ROOT_FUNCTIONPOINTER_H
