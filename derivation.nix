{
  stdenv,
  lib,
  meson,
  ninja,
  kaitai-struct-compiler,
  kaitai-struct-cpp-stl-runtime,
  pkg-config,
  abseil-cpp,
  json-cpp,
  clang-tools
}: let
  fs = lib.fileset;
in
  stdenv.mkDerivation {
    name = "nix-nar-kaitai-spec";
    src = fs.toSource {
      root = ./.;
      fileset = fs.unions [
        ./meson.build
        ./.version
        ./test
        ./src
        ./NAR.ksy
      ];
    };

    buildInputs = [
      kaitai-struct-compiler
      kaitai-struct-cpp-stl-runtime
      pkg-config
      abseil-cpp
      json-cpp
    ];

    nativeBuildInputs = [
      meson
      ninja
      clang-tools
    ];
  }
