#! /usr/bin/env bash

# Test: pack a directory containing a single file
# hello_world.txt that is executable with content "hello world"
tmpdir="$(mktemp -d)"
workdir="$tmpdir/hello_dir"

mkdir -p "$workdir"
echo 'hello world' > "$workdir/hello_world.txt"
chmod +x "$workdir/hello_world.txt"

nar_file="$tmpdir/hello.nar"
nix nar pack "$workdir" > "$nar_file"

actual_json="$(nar-ls "$nar_file")"

expcted_json="$(nix nar ls "$nar_file" / --json --recursive)"

diff -u \
    <(echo "$expcted_json" | jq -S . ) \
    <(echo "$actual_json" | jq -S .)