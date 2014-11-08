#include "Permission.h"
using namespace RCF::Server;

Permission::Permission(std::string name) {
    _name = name;
}

std::string Permission::getName() {
    return _name;
}

std::string Permission::getPermissionType() {
    return "permission";
}
