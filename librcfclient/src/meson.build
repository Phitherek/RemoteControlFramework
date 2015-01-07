if build.name() != 'windows'
    librcfcli = shared_library('rcfclient', 'ServerDefinition.cpp', 'Client.cpp',  include_directories : inc, install : true, dependencies : [dep, boost_dep, ossl_dep], version: '0.1', soversion: 0)
    libsrcfcli = static_library('srcfclient', 'ServerDefinition.cpp', 'Client.cpp',  include_directories : inc, install : true, dependencies : [dep, boost_dep, ossl_dep])
else
    libsrcfcli = static_library('srcfclient', 'ServerDefinition.cpp', 'Client.cpp',  include_directories : inc, install : true, dependencies : [dep, boost_dep, ossl_dep, ws_dep])
endif
