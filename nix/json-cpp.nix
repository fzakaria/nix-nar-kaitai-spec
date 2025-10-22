{
  stdenv,
  fetchFromGitHub,
  cmake,
  copyPkgconfigItems,
  makePkgconfigItem,
  lib,
  testers,
  ctestCheckHook,
  ninja,
}:
stdenv.mkDerivation (finalAttrs: {
  pname = "json.cpp";
  version = "0-unstable-2025-10-21";

  src = fetchFromGitHub {
    owner = "fzakaria";
    repo = "json.cpp";
    rev = "f2306d5de0f8603797614f1cd6d8a3bd22e5b866";
    sha256 = "sha256-qYPS8+q6DAI22f08wVCtIsfP+rIGBYnB43zm8UJ7lZ8=";
  };

  doCheck = true;

  nativeBuildInputs = [
    cmake
    copyPkgconfigItems
    ctestCheckHook
    ninja
  ];

  passthru = {
    tests.pkg-config = testers.testMetaPkgConfig finalAttrs.finalPackage;
  };

  pkgconfigItems = [
    (makePkgconfigItem rec {
      name = "json.cpp";
      inherit (finalAttrs) version;
      cflags = ["-I${variables.includedir}"];
      libs = [
        "-L${variables.libdir}"
        "-ljson"
        "-ldouble-conversion"
      ];
      variables = rec {
        prefix = placeholder "out";
        includedir = "${prefix}/include";
        libdir = "${prefix}/lib";
      };
      inherit (finalAttrs.meta) description;
    })
  ];

  meta = {
    pkgConfigModules = ["json.cpp"];
    description = "JSON for Classic C++";
    homepage = "https://github.com/jart/json.cpp";
    license = lib.licenses.mit;
    maintainers = with lib.maintainers; [fzakaria];
  };
})
