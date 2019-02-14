# Catnip
A program that displays a 3D representation of a file using ncurses.

## Dependencies
* ncurses

## Compiling

Execute the following commands.
Dependencies needed to build will automatically be included if you use `$ nix-shell`.

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Or compile as usual with any C compiler.

## Installation

### Nix

Make sure that the build directory is not there from before, then run
```bash
$ nix-env -f default.nix -i catnip
```
As of now this will copy the entire directory to your Nix store, any suggestions to improve this are welcome.

### Manually
Place the binary in your PATH.

## Usage

Run `$ catnip <filename>`

## Example

From the build directory:

```
$ ./catnip ../CMakeLists.txt
```

[![asciicast](https://asciinema.org/a/Cl7fkZHsm8ROYvb6Tjl8xKa3q.svg)](https://asciinema.org/a/Cl7fkZHsm8ROYvb6Tjl8xKa3q)
