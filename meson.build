project(
    'nix-nar-kaitai-spec',
    'cpp',
    version: files('.version'),
    default_options: ['warning_level=3', 'cpp_std=c++17', 'errorlogs=true'],
    meson_version: '>= 1.1',
    license: 'MIT',
)

kaitai_runtime_dep = dependency('kaitai-struct-cpp-stl-runtime', required: true)

absl_flags_dep = dependency('absl_flags')
absl_crash_dep = dependency('absl_failure_signal_handler')
absl_symbolize_dep = dependency('absl_symbolize')
absl_base_dep = dependency('absl_base')

# Find the Kaitai Struct compiler
# This searches for 'ksc' in your system's PATH.
ksc = find_program('ksc', required: true)

subdir('src')
