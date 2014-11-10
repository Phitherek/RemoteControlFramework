project('librcfserver', 'cpp')
comp = meson.get_compiler('cpp')
inc = include_directories('include')
if build.name() == 'windows'
    dep = find_library('libsrcfcommon', required : false)
    dep = dependency('boost', modules: ['filesystem', 'system'], required: false)
else
    dep = dependency('librcfcommon')
    dep = dependency('boost', modules: ['filesystem', 'system'])
endif
subdir('include')
subdir('src')
if build.name() != 'windows'
    pkgconfig_gen(libraries : librcfsrv, version : '0.1', name : 'librcfserver', filebase : 'librcfserver', description : 'A library of server-side classes for RemoteControlFramework', include_directories: inc, install : true, dependencies : dep)
endif
