with import <nixpkgs> {};

pkgs.mkShell {
  name = "build-env";
  buildInputs = [
    cmake
    ncurses
  ];
}
