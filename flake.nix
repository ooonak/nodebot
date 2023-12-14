{
  description = "Nodebot, MQTT to Discord proxy.";
  inputs = { nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable"; };
  outputs = { self, nixpkgs }:

    let
      allSystems = [ "x86_64-linux" ];

      forAllSystems = func: nixpkgs.lib.genAttrs allSystems
        (system: func nixpkgs.legacyPackages.${system});

    in
    {
      packages = forAllSystems (pkgs: {
        default = pkgs.callPackage ./package.nix { };
        clang = pkgs.callPackage ./package.nix { stdenv = pkgs.clang16Stdenv; };
        gcc = pkgs.callPackage ./package.nix { stdenv = pkgs.gccStdenv; };
      });

    };
}
