fs = import('fs')

bash = find_program('bash', native : true)

tests = [
    'test_hello_world.sh',
    'test_hello_world_executable.sh',
    'test_nested_directories.sh',
    'test_nested_directories_executable.sh',
    'test_empty_file.sh',
]

find_program('nix', native : true)

deps = [
    nar_ls,
]

env_obj = environment()
env_obj.prepend('PATH', fs.parent(nar_ls.full_path()))

foreach script : tests
    name = fs.replace_suffix(script, '')
    test(name,
        bash,
        args : [
            '-x',
            '-e',
            '-u',
            '-o',
            'pipefail',
            script,
        ],
        workdir : meson.current_source_dir(),
        depends : deps,
        env : env_obj,
    )
endforeach