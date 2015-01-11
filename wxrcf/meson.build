project('wxRCF', 'cpp')
comp = meson.get_compiler('cpp')
if build.name() == 'windows'
    inc = include_directories('include', 'C:\MinGW\msys\1.0\local\lib\wx\include\msw-unicode-3.0', 'C:\MinGW\msys\1.0\local\include\wx-3.0')
    rcfcdep = find_library('libsrcfclient', required: false)
    rcfcmdep = find_library('libsrcfcommon', required: false)
    boostdep = dependency('boost', modules: ['filesystem', 'system', 'predef', 'asio', 'thread'], required: false)
    ossldep = dependency('openssl', required: false)
    wxdep = dependency('wxwidgets')
    add_global_arguments('-DUNICODE', language: 'cpp')
    add_global_arguments('-L/local/lib -Wl,--subsystem,windows -mwindows-lsrcfclient -lsrcfcommon -lboost_filesystem -lboost_system -lboost_thread -lssleay32 -leay32 -lws2_32 -lwx_mswu_xrc-3.0 -lwx_mswu_webview-3.0 -lwx_mswu_html-3.0 -lwx_mswu_qa-3.0 -lwx_mswu_adv-3.0 -lwx_mswu_core-3.0 -lwx_baseu_xml-3.0 -lwx_baseu_net-3.0 -lwx_baseu-3.0', language: 'cpp')
    linkargs = ['-L/local/lib', '-Wl,--subsystem,windows', '-mwindows', '-lwx_mswu_xrc-3.0', '-lwx_mswu_webview-3.0', '-lwx_mswu_html-3.0', '-lwx_mswu_qa-3.0', '-lwx_mswu_adv-3.0', '-lwx_mswu_core-3.0', '-lwx_baseu_xml-3.0', '-lwx_baseu_net-3.0', '-lwx_baseu-3.0', '-lsrcfclient', '-lsrcfcommon', '-lboost_filesystem', '-lboost_system', '-lboost_thread', '-lmhash', '-lssleay32', '-leay32', '-lws2_32']
else
    inc = include_directories('include', '/usr/include/wx-3.0', '/usr/lib/wx/include/gtk2-unicode-3.0')
    rcfcdep = dependency('librcfclient')
    rcfcmdep = dependency('librcfcommon')
    boostdep = dependency('boost', modules: ['filesystem', 'system', 'predef', 'asio', 'thread'])
    ossldep = dependency('openssl')
    wxdep = dependency('wxwidgets')
endif
subdir('src')
