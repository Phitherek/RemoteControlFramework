if build.name() != 'windows'
    crcftool = executable('crcftool', 'main.cpp', install: true, dependencies: [rcfcdep, rcfcmdep, boostdep, ossldep])
else
    crcftool = executable('crcftool', 'main.cpp', install: true, dependencies: [rcfcdep, rcfcmdep, boostdep, ossldep], link_args: linkargs)
endif
