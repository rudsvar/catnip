with import <nixpkgs> {};

stdenv.mkDerivation rec {
  version = "0.1";
  name = "catnip-${version}";
  src = lib.sourceFilesBySuffices ./. [
    "CMakeLists.txt" ".c" ".h"
  ];
  buildInputs = [
    gcc
    gnumake
    cmake
    ncurses
  ];
  installPhase = ''
    mkdir -p $out/bin/
    cp src/catnip $out/bin/
  '';
}
