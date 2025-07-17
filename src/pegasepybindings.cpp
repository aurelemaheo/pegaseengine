#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "vector3.hpp"
#include "rigidbody.hpp"
#include "pegaseengine.hpp"

PYBIND11_MODULE(pegase_engine, m) {

  m.doc() = "3D Physic engine";

  pybind11::class_<Vector3>(m, "Vector3")
    .def("__add__", [](const Vector3& a, const Vector3& b) { return a + b; })
    .def("__mul__", [](const Vector3& v, double scalar){ return v * scalar; });

  pybind11::class_<RigidBody>(m, "RigidBody")
    .def("update", &RigidBody::update);

  pybind11::class_<PegaseEngine>(m, "PegaseEngine")
    .def("addBody", &PegaseEngine::addBody, "Add body in a list")
    .def("run", &PegaseEngine::run);
}
