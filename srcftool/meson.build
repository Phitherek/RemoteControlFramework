project('srcftool', 'cpp')
comp = meson.get_compiler('cpp')
if build.name() == 'windows'
    rcfsdep = find_library('libsrcfserver', required: false)
    rcfcmdep = find_library('libsrcfcommon', required: false)
    boostdep = dependency('boost', modules: ['filesystem', 'system', 'asio', 'predef', 'thread'], required: false)
    ossldep = dependency('openssl', required: false)
    add_global_arguments('-lsrcfclient -lsrcfcommon -lboost_filesystem -lboost_system -lboost_thread -lssleay32 -leay32 -lws2_32', language: 'cpp')
    linkargs = ['-lsrcfclient', '-lsrcfcommon', '-lboost_filesystem', '-lboost_system', '-lboost_thread', '-lmhash', '-lssleay32', '-leay32', '-lws2_32']
else
    rcfsdep = dependency('librcfserver')
    rcfcmdep = dependency('librcfcommon')
    boostdep = dependency('boost', modules: ['filesystem', 'system', 'asio', 'predef', 'thread'])
    ossldep = dependency('openssl')
endif
subdir('src')
