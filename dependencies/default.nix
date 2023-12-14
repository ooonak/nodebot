let
  pkgs = import <nixpkgs> { };
  callPackage = pkgs.lib.callPackageWith (pkgs // packages);
  packages = {
    #DPP = callPackage ./DPP.nix { };
    mosquitto = callPackage ./mosquitto.nix { };
  };
in
  packages
