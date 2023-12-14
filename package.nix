{ pkgs ? import <nixpkgs> {}
, lib
, stdenv
, cmake
, clang-tools_16
, gtest
, mosquitto
, nlohmann_json
, ninja
, openssl
, pkg-config
, spdlog
, toml11
, zlib
, enableTests ? true
}:

stdenv.mkDerivation rec {
  name = "NodeBotApp";

  src = lib.sourceByRegex ./. [
    "^app.*"
    "^cmake.*"
    "^src.*"
    "^tests.*"
    "CMakeLists.txt"
  ];

  # Distinguishing between native build inputs (runnable on the host at compile time)
  # And normal build inputs (runnable on target platform at run time) is important for cross compilation.
  nativeBuildInputs = [
    cmake
    clang-tools_16
    pkg-config
    ninja
  ];
  buildInputs = [
    mosquitto
    nlohmann_json
    openssl
    spdlog
    toml11
    zlib
    (pkgs.callPackage ./dependencies/DPP.nix {})
  ];
  checkInputs = [
    gtest
  ];

  doCheck = enableTests;
  cmakeFlags = lib.optional (!enableTests) "-DTESTING=off";
}
