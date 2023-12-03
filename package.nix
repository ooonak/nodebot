{ lib
, stdenv
, cmake
, gtest
, mosquitto
, ninja
, nlohmann_json
, spdlog
, toml11
, zlib
, enableTests ? true
}:

stdenv.mkDerivation {
  name = "cpp-nix";

  # good source filtering is important for caching of builds.
  # It's easier when subprojects have their own distinct subfolders.
  src = lib.sourceByRegex ./. [
    "^src.*"
    "^tests.*"
    "CMakeLists.txt"
  ];

  # Distinguishing between native build inputs (runnable on the host
  # at compile time) and normal build inputs (runnable on target
  # platform at run time) is important for cross compilation.
  nativeBuildInputs = [ cmake ninja ];
  buildInputs = [
    mosquitto
    nlohmann_json
    spdlog
    toml11
    zlib
  ];
  checkInputs = [ gtest ];

  doCheck = enableTests;
  cmakeFlags = lib.optional (!enableTests) "-DTESTING=off";
}
