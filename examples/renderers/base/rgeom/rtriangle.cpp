#include <base/rgeom/rendered_geometry.hpp>

#include <base/include_gl.hpp>

rtriangle::rtriangle() : rgeom() {
	t = rendered_geometry_type::triangle;
}
rtriangle::~rtriangle() {}

void rtriangle::draw_geometry() const {
	glDisable(GL_LIGHTING);
	glColor4f(r,g,b,a);
	glBegin(GL_TRIANGLES);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
	glEnd();
	glEnable(GL_LIGHTING);
}
