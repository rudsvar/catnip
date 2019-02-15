with import <nixpkgs> {};

stdenv.mkDerivation rec {
  version = "0.1";
  name = "catnip-${version}";
  src = lib.sourceByRegex ./. [
    "(src|test|include)"
    "CMakeLists.txt"
    "(.*\.c|.*\.h)"
  ];
  buildInputs = [
    gcc
    gnumake
    cmake
    ncurses
  ];
  installPhase = ''
    mkdir -p $out/bin/
    cp catnip $out/bin/
  '';
}
