if build.name() != 'windows'
    rcfserver = executable('rcfserver', 'main.cpp', install: true, dependencies: [rcfsdep, rcfcmdep, boostdep, ossldep])
else
    rcfserver = executable('rcfserver', 'main.cpp', install: true, dependencies: [rcfsdep, rcfcmdep, boostdep, ossldep], link_args: linkargs)
endif