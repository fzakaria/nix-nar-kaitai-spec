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
            nix-nar-kaitai-spec = final.callPackage ./derivation.nix {};
            json-cpp = final.callPackage ./nix/json-cpp.nix {};
            # https://github.com/NixOS/nixpkgs/pull/454243
            kaitai-struct-cpp-stl-runtime = final.callPackage ./nix/kaitai-struct-cpp-stl-runtime.nix {};
          })
        ];
      };
    in {
      packages.nix-nar-kaitai-spec = pkgs.nix-nar-kaitai-spec;

      devShells.default = pkgs.mkShell {
        inputsFrom = [
          pkgs.nix-nar-kaitai-spec
        ];
      };
    });
}
