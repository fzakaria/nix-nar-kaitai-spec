subdir('kaitai')

executable(
    'nar-ls',
    'nar_ls.cc',
    dependencies: [
        nar_kaitai_dep,
        kaitai_runtime_dep,
        absl_flags_dep,
        absl_crash_dep,
        absl_symbolize_dep,
        absl_base_dep,
    ],
)
