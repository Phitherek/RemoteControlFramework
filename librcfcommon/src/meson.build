if build.name() != 'windows'
    librcfc = shared_library('rcfcommon', 'MHashEngine.cpp', include_directories : inc, install : true, dependencies : dep)
endif
libsrcfc = static_library('srcfcommon', 'MHashEngine.cpp', include_directories : inc, install : true, dependencies : dep)
