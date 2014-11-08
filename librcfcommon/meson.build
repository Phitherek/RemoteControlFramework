project('librcfcommon', 'cpp')
comp = meson.get_compiler('cpp')
inc = include_directories('include')
if build.name() == 'windows'
    dep = find_library('mhash', required : false)
else
    dep = find_library('mhash')
endif
subdir('include')
subdir('src')
if build.name() != 'windows'
    pkgconfig_gen(libraries : librcfc, version : '0.1', name : 'librcfcommon', filebase : 'librcfcommon', description : 'A library of common classes for RemoteControlFramework', include_directories: inc, install : true, dependencies : dep)
endif
