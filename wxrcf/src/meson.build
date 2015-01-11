if build.name() != 'windows'
    wxRCF = executable('wxRCF', 'App.cpp', 'layout.cpp', 'layoutBase.cpp', include_directories: inc, install: true, dependencies: [rcfcdep, rcfcmdep, boostdep, ossldep, wxdep])
else
    wxRCF = executable('wxRCF', 'App.cpp', 'layout.cpp', 'layoutBase.cpp', include_directories: inc, install: true, dependencies: [rcfcdep, rcfcmdep, boostdep, ossldep, wxdep], link_args: linkargs)
endif
