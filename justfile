# Build and run the project
[working-directory: 'builddir']
run:
    meson compile
    ./yoyo_viz

# Build the project
[working-directory: 'builddir']
build:
    meson compile

# Launch the project with qml debugging
[working-directory: 'builddir']
debug:
    meson compile
    ./yoyo_viz -qmljsdebugger=port:3001

[working-directory: 'builddir']
gamma-debug:
    meson compile
    gammaray ./yoyo_viz
