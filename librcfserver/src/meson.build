if build.name() != 'windows'
    librcfsrv = shared_library('rcfserver', 'Permission.cpp', 'User.cpp', 'Group.cpp', 'Command.cpp', 'ExecuteImpl.cpp', 'CommandGroup.cpp', 'Server.cpp',  include_directories : inc, install : true, dependencies : [dep, boost_dep, ossl_dep], version: '0.1', soversion: 0)
    libsrcfsrv = static_library('srcfserver', 'Permission.cpp', 'User.cpp', 'Group.cpp', 'Command.cpp', 'ExecuteImpl.cpp', 'CommandGroup.cpp', 'Server.cpp',  include_directories : inc, install : true, dependencies : [dep, boost_dep, ossl_dep])
else
    libsrcfsrv = static_library('srcfserver', 'Permission.cpp', 'User.cpp', 'Group.cpp', 'Command.cpp', 'ExecuteImpl.cpp', 'CommandGroup.cpp', 'Server.cpp',  include_directories : inc, install : true, dependencies : [dep, boost_dep, ossl_dep, ws_dep])
endif
