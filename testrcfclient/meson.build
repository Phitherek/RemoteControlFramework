project('testrcfclient', 'cpp')
comp = meson.get_compiler('cpp')
if build.name() == 'windows'
    rcfcdep = find_library('libsrcfclient', required: false)
    rcfcmdep = find_library('libsrcfcommon', required: false)
    boostdep = dependency('boost', modules: ['filesystem', 'system', 'asio'], required: false)
    ossldep = dependency('openssl', required: false)
else
    rcfcdep = dependency('librcfclient')
    rcfcmdep = dependency('librcfcommon')
    boostdep = dependency('boost', modules: ['filesystem', 'system', 'asio'])
    ossldep = dependency('openssl')
endif
subdir('src')
