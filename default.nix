with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "catnip";
  src = ./.;
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
