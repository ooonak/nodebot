{
  description = "Nodebot, MQTT to Discord proxy.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = inputs@{ flake-parts, ... }:
  flake-parts.lib.mkFlake { inherit inputs; } {

    #systems = [ "x86_64-linux" "aarch64-linux" "aarch64-darwin" "x86_64-darwin" ];
    systems = [ "x86_64-linux" ];
    perSystem = { config, self', inputs', pkgs, system, ... }: {

      packages = {
        default = pkgs.callPackage ./package.nix { };
        clang = pkgs.callPackage ./package.nix { stdenv = pkgs.clang16Stdenv; };
        gcc = pkgs.callPackage ./package.nix { stdenv = pkgs.gccStdenv; };

        dockerImage = pkgs.dockerTools.buildImage {
          name = "Nodebot";
          created = "now";
          tag = builtins.substring 0 9 (self'.rev or "dev");
          copyToRoot = pkgs.buildEnv {
            paths = with pkgs; [
              self'.packages.default
              #cacert
              #coreutils
              #bash
            ];
            name = "nodebot-root";
            pathsToLink = [ "/bin" "/lib" "/include" ];
          };
          config = {
            Cmd = [ "bin/NodeBotApp" ];
            Env = [
              #"SSL_CERT_FILE=${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt"
              #"SYSTEM_CERTIFICATE_PATH=${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt"
            ];
          };
        };

      } // pkgs.lib.optionalAttrs (system != "x86_64-linux") {
        crossIntel = pkgs.pkgsCross.gnu64.callPackage ./package.nix {
          enableTests = false;
        };
      } // pkgs.lib.optionalAttrs (system != "aarch64-linux") {
        crossAarch64 = pkgs.pkgsCross.aarch64-multiplatform.callPackage ./package.nix {
          enableTests = false;
        };
      };

      checks = config.packages // {
        clang = config.packages.default.override {
          stdenv = pkgs.clang16Stdenv;
        };
        gcc = config.packages.default.override {
          stdenv = pkgs.gccStdenv;
        };
      };

    };

  };
}
