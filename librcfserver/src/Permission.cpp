#include "Permission.h"
using namespace RCF::Server;

Permission::Permission() {
    _name = "";
}

Permission::Permission(std::string name) {
    _name = name;
}

std::string Permission::getName() {
    return _name;
}

std::string Permission::getPermissionType() {
    return "permission";
}

void Permission::load(std::string name) {}

void Permission::save() {}

bool Permission::valid() {
    return false;
}
