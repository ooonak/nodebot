{ 
  lib, 
  stdenv, 
  fetchFromGitHub, 
  clang-tools_16, 
  cmake, 
  ninja, 
  zlib,
  openssl
}:

clang-tools_16.stdenv.mkDerivation rec {
  pname = "DPP";
  version = "10.0.29";
  src = fetchFromGitHub {
    owner = "brainboxdotcc";
    repo = "DPP";
    rev = "v10.0.29";
    hash = "sha256-BJMg3MLSfb9x/2lPHITeI3SWwW1OZVUUMVltTWUcw9I=";
  };

  nativeBuildInputs = [ cmake ninja ];
  buildInputs = [ zlib openssl ];

  preConfigure = ''
    cmakeFlagsArray+=(
    "-DBUILD_VOICE_SUPPORT=OFF" "-DRUN_LDCONFIG=OFF" "-DDPP_BUILD_TEST=OFF" "-DDPP_NO_VCPKG=ON"
  )
  '';

  meta = with lib; {
    description = "C++ Discord API Bot Library - D++ is Lightweight and scalable for small and huge bots!";
    homepage = "https://github.com/brainboxdotcc/DPP";
    license = "Apache-2.0";
    maintainers = with maintainers; [ ];
    platforms = platforms.linux;
  };
}
