project('crcftool', 'cpp')
comp = meson.get_compiler('cpp')
if build.name() == 'windows'
    rcfcdep = find_library('libsrcfclient', required: false)
    rcfcmdep = find_library('libsrcfcommon', required: false)
    boostdep = dependency('boost', modules: ['filesystem', 'system'], required: false)
    ossldep = dependency('openssl', required: false)
    add_global_arguments('-lsrcfclient -lsrcfcommon -lboost_filesystem -lboost_system -lssleay32 -leay32 -lws2_32', language: 'cpp')
    linkargs = ['-lsrcfclient', '-lsrcfcommon', '-lboost_filesystem', '-lboost_system', '-lmhash', '-lssleay32', '-leay32', '-lws2_32']
else
    rcfcdep = dependency('librcfclient')
    rcfcmdep = dependency('librcfcommon')
    boostdep = dependency('boost', modules: ['filesystem', 'system'])
    ossldep = dependency('openssl')
endif
subdir('src')
