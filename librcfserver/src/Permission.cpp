#include "Permission.h"
using namespace RCF::Server;

Permission** Permission::_loadedPermissions = NULL;
int Permission::_loadedPermissionsSize = 0;

Permission::Permission() {
    _name = "";
}

Permission::Permission(std::string name) {
    _name = name;
}

Permission::~Permission() {}

std::string Permission::getName() {
    return _name;
}

std::string Permission::getPermissionType() {
    return "permission";
}

void Permission::save() {}

bool Permission::valid() {
    return false;
}

void Permission::free() {
    if(_loadedPermissions != NULL && _loadedPermissionsSize > 0) {
        for(int i = 0; i < _loadedPermissionsSize; i++) {
            delete _loadedPermissions[i];
        }
        delete[] _loadedPermissions;
    }
    _loadedPermissions = NULL;
    _loadedPermissionsSize = 0;
}
