{ lib
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

stdenv.mkDerivation {
  name = "NodeBot";

  # good source filtering is important for caching of builds.
  # It's easier when subprojects have their own distinct subfolders.
  src = lib.sourceByRegex ./. [
    "^app.*"
    "^cmake.*"
    "^src.*"
    "^tests.*"
    "CMakeLists.txt"
  ];

  # Distinguishing between native build inputs (runnable on the host
  # at compile time) and normal build inputs (runnable on target
  # platform at run time) is important for cross compilation.
  nativeBuildInputs = [
    cmake
    clang-tools_16
    ninja
  ];
  buildInputs = [
    mosquitto
    nlohmann_json
    openssl
    pkg-config
    spdlog
    toml11
    zlib
  ];
  checkInputs = [
    gtest
  ];

  doCheck = enableTests;
  cmakeFlags = lib.optional (!enableTests) "-DTESTING=off";
}
