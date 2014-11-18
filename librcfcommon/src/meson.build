if build.name() != 'windows'
    librcfc = shared_library('rcfcommon', 'MHashEngine.cpp', 'HelperFunctions.cpp', 'Exceptions.cpp', include_directories : inc, install : true, dependencies : [dep, boost_dep], version: '0.1', soversion: 0)
endif
libsrcfc = static_library('srcfcommon', 'MHashEngine.cpp', 'HelperFunctions.cpp', 'Exceptions.cpp', include_directories : inc, install : true, dependencies : [dep, boost_dep])
