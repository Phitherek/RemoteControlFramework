project('librcfclient', 'cpp')
comp = meson.get_compiler('cpp')
inc = include_directories('include')
if build.name() == 'windows'
    dep = find_library('libsrcfcommon', required : false)
    boost_dep = dependency('boost', modules: ['filesystem', 'system', 'asio'], required: false)
    ossl_dep = dependency('openssl', required: false)
    ws_dep = dependency('libws2_32', required: false)
    add_global_arguments('-lsrcfcommon -lboost_filesystem -lboost_system -lssleay32 -leay32 -lws2_32', language: 'cpp')
else
    dep = dependency('librcfcommon')
    boost_dep = dependency('boost', modules: ['filesystem', 'system', 'asio'])
    ossl_dep = dependency('openssl')
endif
subdir('include')
subdir('src')
if build.name() != 'windows'
    pkgconfig_gen(libraries : librcfcli, version : '0.1', name : 'librcfclient', filebase : 'librcfclient', description : 'A library of client-side classes for RemoteControlFramework', include_directories: inc, install : true, dependencies : [dep, boost_dep, ossl_dep])
endif
