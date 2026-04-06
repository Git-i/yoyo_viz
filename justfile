# Build and run the project
[working-directory: 'builddir']
run:
    meson compile
    ./yoyo_viz

# Build the project
[working-directory: 'builddir']
build:
    meson compile
    
