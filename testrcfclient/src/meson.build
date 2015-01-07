if build.name() != 'windows'
    testrcfclient = executable('testrcfclient', 'main.cpp', install: true, dependencies: [rcfcdep, rcfcmdep, boostdep, ossldep])
else
    testrcfclient = executable('testrcfclient', 'main.cpp', install: true, dependencies: [rcfcdep, rcfcmdep, boostdep, ossldep], link_args: linkargs)
endif
