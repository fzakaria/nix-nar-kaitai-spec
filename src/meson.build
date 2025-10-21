kaitai_generated_srcs = custom_target(
    'kaitai-generated-sources',
    input: ['../NAR.ksy'],
    output: ['nix_nar.cpp', 'nix_nar.h'],
    command: [ksc, '@INPUT@', '--target', 'cpp_stl', '--outdir', '@OUTDIR@'],
)

kaitai_lib = library(
    'nix-nar-kaitai-lib',
    kaitai_generated_srcs,
)

executable('nar-ls', 'nar_ls.cc', link_with: kaitai_lib)
