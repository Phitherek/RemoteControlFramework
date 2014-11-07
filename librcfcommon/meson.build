project('librcfcommon', 'cpp')
comp = meson.get_compiler('cpp')
if comp.has_header('mhash.h')
    inc = include_directories('include')
    subdir('include')
    subdir('src')
    pkgconfig_gen(libraries : librcfc, version : '0.1', name : 'librcfcommon', filebase : 'librcfcommon', description : 'A library of common classes for RemoteControlFramework', include_directories: inc)
else
    message('This library depends on  MHash library, which has not been found in your include directory. Please install the library or change Meson include directory')
endif
