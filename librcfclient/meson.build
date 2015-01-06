project('librcfclient', 'cpp')
comp = meson.get_compiler('cpp')
inc = include_directories('include')
if build.name() == 'windows'
    dep = find_library('libsrcfcommon', required : false)
    dep = dependency('boost', modules: ['filesystem', 'system', 'asio'], required: false)
else
    dep = dependency('librcfcommon')
    dep = dependency('boost', modules: ['filesystem', 'system', 'asio'])
endif
subdir('include')
subdir('src')
if build.name() != 'windows'
    pkgconfig_gen(libraries : librcfcli, version : '0.1', name : 'librcfclient', filebase : 'librcfclient', description : 'A library of client-side classes for RemoteControlFramework', include_directories: inc, install : true, dependencies : dep)
endif
