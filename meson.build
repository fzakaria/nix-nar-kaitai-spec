project(
    'nix-nar-kaitai-spec',
    'cpp',
    version: files('.version'),
    default_options: ['warning_level=3', 'cpp_std=c++17', 'errorlogs=true'],
    meson_version: '>= 1.1',
    license: 'MIT',
)

kaitai_runtime_dep = dependency('kaitai-struct-cpp-stl-runtime', required: true)
json_cpp_dep = dependency('json.cpp')
absl_strings_dep = dependency('absl_strings')
absl_statusor_dep = dependency('absl_statusor')
absl_log_dep = dependency('absl_log')
absl_log_initialize_dep = dependency('absl_log_initialize')
absl_flags_parse_dep = dependency('absl_flags_parse')
absl_flags_dep = dependency('absl_flags')
absl_crash_dep = dependency('absl_failure_signal_handler')
absl_symbolize_dep = dependency('absl_symbolize')

# Find the Kaitai Struct compiler
# This searches for 'ksc' in your system's PATH.
ksc = find_program('ksc', required: true)

subdir('src')
