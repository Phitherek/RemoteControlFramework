project('librcfserver', 'cpp')
comp = meson.get_compiler('cpp')
inc = include_directories('include')
if build.name() == 'windows'
    dep = find_library('libsrcfcommon', required : false)
    boost_dep = dependency('boost', modules: ['filesystem', 'system', 'predef', 'asio', 'thread'], required: false)
    ossl_dep = dependency('openssl', required: false)
    ws_dep = find_library('libws2_32', required: false)
    add_global_arguments('-lsrcfcommon -lboost_filesystem -lboost_system -lboost_asio -lboost_thread -lssleay32 -leay32 -lws2_32', language: 'cpp')
else
    dep = dependency('librcfcommon')
    boost_dep = dependency('boost', modules: ['filesystem', 'system', 'predef', 'asio', 'thread'])
    ossl_dep = dependency('openssl')
endif
subdir('include')
subdir('src')
if build.name() != 'windows'
    pkgconfig_gen(libraries : librcfsrv, version : '0.1', name : 'librcfserver', filebase : 'librcfserver', description : 'A library of server-side classes for RemoteControlFramework', include_directories: inc, install : true, dependencies : [dep, boost_dep, ossl_dep])
endif
