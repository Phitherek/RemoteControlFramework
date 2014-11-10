if build.name() != 'windows'
    librcfc = shared_library('rcfcommon', 'MHashEngine.cpp', 'HelperFunctions.cpp', include_directories : inc, install : true, dependencies : [dep, boost_dep])
endif
libsrcfc = static_library('srcfcommon', 'MHashEngine.cpp', 'HelperFunctions.cpp', include_directories : inc, install : true, dependencies : [dep, boost_dep])
