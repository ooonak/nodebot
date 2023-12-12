let
  pkgs = import <nixpkgs> { };
  callPackage = pkgs.lib.callPackageWith (pkgs // packages);
  packages = {
    DPP = callPackage ./DPP.nix { };
  };
in
  packages
