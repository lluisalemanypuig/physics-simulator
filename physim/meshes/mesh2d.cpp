#include <physim/meshes/mesh2d.hpp>

namespace physim {
namespace meshes {

mesh2d::mesh2d() : mesh() {
}

mesh2d::mesh2d(float ke, float kd) : mesh(ke,kd) {
}

mesh2d::~mesh2d() {
}

} // -- namespace meshes
} // -- namespace physim
