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

void rtriangle::make_box(box& b) const {
	b.set_min_max(p1,p1);
	b.enlarge_box(p2);
	b.enlarge_box(p3);
}
