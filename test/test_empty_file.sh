#! /usr/bin/env bash

tmpdir="$(mktemp -d)"
workdir="$tmpdir/hello_dir"

mkdir -p "$workdir"
touch "$workdir/empty_file.txt"

nar_file="$tmpdir/hello.nar"
nix nar pack "$workdir" > "$nar_file"

actual_json="$(nar-ls "$nar_file")"

expcted_json="$(nix nar ls "$nar_file" / --json --recursive)"

diff -u \
    <(echo "$expcted_json" | jq -S . ) \
    <(echo "$actual_json" | jq -S .)