#pragma once

namespace physim {
namespace math {

template<typename T>
inline T min(const T& a, const T& b) {
	if (a < b) {
		return a;
	}
	return b;
}

template<typename T>
inline T max(const T& a, const T& b) {
	if (a < b) {
		return a;
	}
	return b;
}

} // -- namsepace math
} // -- namespace physim
