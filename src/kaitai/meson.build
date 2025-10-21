kaitai_generated_srcs = custom_target(
    'kaitai-generated-sources',
    input: ['../../NAR.ksy'],
    output: ['nix_nar.cpp', 'nix_nar.h'],
    command: [ksc, '@INPUT@', '--target', 'cpp_stl', '--outdir', 'src/kaitai'],
)

incdir = include_directories('.')

kaitai_lib = library(
    'nix-nar-kaitai-lib',
    kaitai_generated_srcs,
    dependencies: [kaitai_runtime_dep],
    include_directories: incdir,
)
