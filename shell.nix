{ pkgs ? import <nixpkgs> { } }:

let
    unstable = import
        (fetchTarball "https://nixos.org/channels/nixos-unstable/nixexprs.tar.xz") {};
    buildLibs = with pkgs; (with xorg; [
    ]);
in with pkgs; with xorg; mkShell {
    buildInputs = [
        ccls
        gcc
        gdb
        pkg-config
    ] ++ buildLibs;
    shellHook = ''
        export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${lib.makeLibraryPath buildLibs}"
    '';
}

