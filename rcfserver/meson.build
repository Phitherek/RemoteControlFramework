project('rcfserver', 'cpp')
comp = meson.get_compiler('cpp')
if build.name() == 'windows'
    rcfsdep = find_library('libsrcfserver', required: false)
    rcfcmdep = find_library('libsrcfcommon', required: false)
    boostdep = dependency('boost', modules: ['filesystem', 'system', 'predef', 'asio', 'thread'], required: false)
    ossldep = dependency('openssl', required: false)
else
    rcfsdep = dependency('librcfserver')
    rcfcmdep = dependency('librcfcommon')
    boostdep = dependency('boost', modules: ['filesystem', 'system', 'predef', 'asio', 'thread'])
    ossldep = dependency('openssl')
endif
subdir('src')
