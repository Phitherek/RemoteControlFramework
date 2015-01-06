if build.name() != 'windows'
    librcfcli = shared_library('rcfclient', 'ServerDefinition.cpp', 'Client.cpp',  include_directories : inc, install : true, dependencies : dep, version: '0.1', soversion: 0)
endif
libsrcfcli = static_library('srcfclient', 'ServerDefinition.cpp', 'Client.cpp',  include_directories : inc, install : true, dependencies : dep)
