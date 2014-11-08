if build.name() != 'windows'
    librcfsrv = shared_library('rcfserver', 'Permission.cpp', include_directories : inc, install : true, dependencies : dep)
endif
libsrcfsrv = static_library('srcfserver', 'Permission.cpp', include_directories : inc, install : true, dependencies : dep)
