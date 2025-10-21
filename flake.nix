{
  description = "A kaitai struct definition for Nix NAR files.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs {
        inherit system;
        overlays = [
          (final: prev: {
            kaitai-struct-cpp-stl-runtime = prev.stdenv.mkDerivation (finalAttrs: {
              pname = "kaitai-struct-cpp-stl-runtime";
              version = "0.11";

              src = prev.fetchFromGitHub {
                owner = "kaitai-io";
                repo = "kaitai_struct_cpp_stl_runtime";
                rev = "${finalAttrs.version}";
                sha256 = "sha256-2glGPf08bkzvnkLpQIaG2qiy/yO+bZ14hjIaCKou2vU=";
              };

              nativeBuildInputs = [
                prev.cmake
                prev.gtest
                prev.zlib.dev
                prev.copyPkgconfigItems
              ];

              pkgconfigItems = [
                (prev.makePkgconfigItem rec {
                  name = "kaitai-struct-cpp-stl-runtime";
                  inherit (finalAttrs) version;
                  cflags = ["-I${variables.includedir}"];
                  libs = [
                    "-L${variables.libdir}"
                    "-lkaitai_struct_cpp_stl_runtime"
                  ];
                  variables = rec {
                    prefix = placeholder "out";
                    includedir = "${prefix}/include";
                    libdir = "${prefix}/lib";
                  };
                  inherit (finalAttrs.meta) description;
                })
              ];

              meta = with prev.lib; {
                description = "Kaitai Struct C++ STL Runtime Library";
                homepage = "https://kaitai.io/";
                license = licenses.mit;
                maintainers = with maintainers; [fzakaria];
              };
            });
          })
        ];
      };
    in {
      devShells.default = pkgs.mkShell {
        packages = [
          pkgs.kaitai-struct-compiler
          pkgs.kaitai-struct-cpp-stl-runtime
          pkgs.meson
          pkgs.ninja
          pkgs.pkg-config
        ];
      };
    });
}
