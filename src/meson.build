subdir('kaitai')

executable(
    'nar-ls',
    'nar_ls.cc',
    dependencies: [
        nar_kaitai_dep,
        kaitai_runtime_dep,
        absl_flags_parse_dep,
        absl_flags_dep,
        absl_crash_dep,
        absl_symbolize_dep,
        absl_log_initialize_dep,
        absl_log_dep,
        absl_statusor_dep,
        absl_strings_dep,
        json_cpp_dep,
    ],
    install: true,
)
