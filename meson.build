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
    'include/constraint_list_model.hpp',
    'include/highlighter.hpp',
    'include/compiler_state.hpp',
    'include/ast_tree_view_model.hpp',
    'include/type_checker_wrappers/constraint_info_object.hpp'
]
sources = [
    'yoyo_viz.cpp',
    'src/highlighter.cpp',
    'src/compiler_state.cpp',
    'src/ast_tree_view_model.cpp',
    'src/constraint_list_model.cpp'
]
inc = ['include/', 'include/type_checker_wrappers']
sources += qt6.qml_module(
    'YoyoViz',
    version: '0.1',
    qml_sources: [
        'qml/Main.qml',
        'qml/Editor.qml',
        'qml/ParserOutput.qml',
        'qml/OutputViewer.qml',
        'qml/ConstraintListViewer.qml',
        'qml/TypeCheckerViewer.qml'
    ],
)
sources += qt6.qml_module(
    'com.yoyoviz.cpp',
    version: '1.0',
    moc_headers: headers,
    include_directories: inc,
    dependencies: qt_dep
)
# moc_files = qt6.compile_moc(headers: headers)
exe = executable(
  'yoyo_viz',
  sources,
  dependencies : dependencies,
  include_directories: inc,
  install : true,
)

test('basic', exe)
