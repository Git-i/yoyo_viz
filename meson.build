project(
    'yoyo_viz',
    'cpp',
    meson_version : '>= 1.3.0',
    version : '0.1',
    default_options : ['warning_level=3', 'cpp_std=c++23'],
)
qt6 = import('qt6')
qt_dep = dependency('qt6', modules: ['Quick', 'Gui', 'Core'])
yoyoc = dependency('yoyo-lang')
# set up tree sitter yoyo
cmake = import('cmake')
ts_yoyo_proj = cmake.subproject('tree-sitter-yoyo')
ts_yoyo_dep = ts_yoyo_proj.dependency('tree-sitter-yoyo')
dependencies = [
    qt_dep, yoyoc, dependency('tree-sitter'), ts_yoyo_dep
]
headers = [
    'include/highlighter.hpp'
]
sources = [
    'yoyo_viz.cpp',
    'src/highlighter.cpp'
]
sources += qt6.qml_module(
    'YoyoViz',
    version: '0.1',
    qml_sources: [
        'qml/Main.qml',
        'qml/Editor.qml'
    ],

)
moc_files = qt6.compile_moc(headers: headers)
exe = executable(
  'yoyo_viz',
  sources, moc_files,
  dependencies : dependencies,
  install : true,
)

test('basic', exe)
