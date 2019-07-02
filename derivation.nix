{ stdenv, lib, ncurses, cmake }:

stdenv.mkDerivation rec {
  version = "0.1";
  name = "catnip-${version}";
  src = lib.sourceFilesBySuffices ./. [
    "CMakeLists.txt" ".c" ".h"
  ];
  buildInputs = [ ncurses ];
  nativeBuildInputs = [ cmake ];
}
