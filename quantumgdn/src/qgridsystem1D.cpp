#include "qgridsystem1D.hpp"

using namespace qgdn;

void qgridsystem1D::_register_methods() {
    // direct methods
    register_method("set_mass", &qgridsystem1D::set_mass);
    register_method("mass", &qgridsystem1D::mass);
    register_method("energy", &qgridsystem1D::energy);
    register_method("position", &qgridsystem1D::position);
    register_method("momentum", &qgridsystem1D::momentum);
    register_method("normalize", &qgridsystem1D::normalize);
    register_method("map", &qgridsystem1D::map);
    register_method("size", &qgridsystem1D::size);
    
    // TODO, methods that need to be adapted
    //register_method("psi", &qgridsystem1D::gdn_psi);
    //register_method("V", &qgridsystem1D::gdn_V);
}
