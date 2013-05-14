#ifndef GC_TYPEDESCRIPTOR_HPP
#define GC_TYPEDESCRIPTOR_HPP

#include <string>

#ifdef HAVE_CXXABI_H
#include <cxxabi.h>
#endif

namespace greycore {
	template <typename T>
	std::string getTypedescriptor() {
#ifdef HAVE_CXXABI_H
		int status;
		char* result = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
		if (status == 0) {
			return result;
		}
#endif
		return typeid(T).name();
	}
}

#endif

