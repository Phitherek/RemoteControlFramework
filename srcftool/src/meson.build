if build.name() != 'windows'
    srcftool = executable('srcftool', 'main.cpp', install: true, dependencies: [rcfsdep, rcfcmdep, boostdep, ossldep])
else
    srcftool = executable('srcftool', 'main.cpp', install: true, dependencies: [rcfsdep, rcfcmdep, boostdep, ossldep], link_args: linkargs)
endif
