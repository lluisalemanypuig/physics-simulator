#include <base/cameras/observer.hpp>

observer::observer() : zN(-1), zF(1) { }
observer::~observer() { }

void observer::set_znear(double zn) {
	zN = zn;
}
float observer::get_znear() const {
	return zN;
}

void observer::set_zfar(double zf) {
	zF = zf;
}
float observer::get_zfar() const {
	return zF;
}

