if build.name() != 'windows'
    rcfclient = executable('rcfclient', 'main.cpp', install: true, dependencies: [rcfcdep, rcfcmdep, boostdep, ossldep])
else
    rcfclient = executable('rcfclient', 'main.cpp', install: true, dependencies: [rcfcdep, rcfcmdep, boostdep, ossldep], link_args: linkargs)
endif
