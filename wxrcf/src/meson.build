if build.name() != 'windows'
    wxRCF = executable('wxRCF', 'App.cpp', 'layout.cpp', 'layoutBase.cpp', include_directories: inc, install: true, dependencies: [rcfcdep, rcfcmdep, boostdep, ossldep, wxdep])
else
    wxRCF = executable('wxRCF', 'App.cpp', 'layout.cpp', 'layoutBase.cpp', include_directories: inc, install: true, dependencies: [rcfcdep, rcfcmdep, boostdep, ossldep, wxdep1, wxdep2, wxdep3, wxdep4, wxdep5, wxdep6, wxdep7, wxdep8, wxdep9], link_args: linkargs)
endif
