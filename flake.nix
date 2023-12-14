{
  description = "Nodebot, MQTT to Discord proxy.";
  inputs = { nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable"; };
  outputs = { self, nixpkgs }:

    let
      #supportedSystems = [ "x86_64-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];
      supportedSystems = [ "x86_64-linux" ];

      forAllSystems = func: nixpkgs.lib.genAttrs supportedSystems
        (system: func nixpkgs.legacyPackages.${system});

    in
    {
      packages = forAllSystems (pkgs: {
        default = pkgs.callPackage ./package.nix { stdenv = pkgs.clang16Stdenv; };
        clang = pkgs.callPackage ./package.nix { stdenv = pkgs.clang16Stdenv; };
        gcc = pkgs.callPackage ./package.nix { stdenv = pkgs.gccStdenv; };

      dockerImage = pkgs.dockerTools.buildImage rec {
          name = "NodeBot";
          created = "now";
          tag = "latest";
          copyToRoot = pkgs.buildEnv {
            name = "nodebot-root";
            paths = with pkgs; [
              dockerTools.fakeNss
              dockerTools.caCertificates
              # How to refer to packages.default here?
              # Is not in scope NodeBotApp
              self.packages
            ];
            pathsToLink = [ "/etc" "/var" "/bin" "/lib" "/include" ];
          };
          config = {
            Cmd = [ "bin/NodeBotApp" ];
            Env = [ ];
          };
        };

      });

    };
}
