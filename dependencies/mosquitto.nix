{ 
  lib, 
  stdenv, 
  fetchFromGitHub,
  cmake, 
  ninja, 
  openssl
}:

stdenv.mkDerivation rec {
  pname = "mosquitto";
  version = "2.0.18";
  src = fetchFromGitHub {
    owner = "eclipse";
    repo = "mosquitto";
    rev = "v2.0.18";
    hash = "sha256-Vs0blV2IhnlEAm0WtOartz+0vLesJfp78FNJCivRxHk=";
  };

  nativeBuildInputs = [ cmake ninja ];
  buildInputs = [ openssl ];

  cmakeFlags = [
    "-DENABLE_TESTING=OFF"
    "-DWITH_DLT=OFF"
    "-DWITH_CJSON=OFF"
    "-DWITH_SOCKS=OFF"
    "-DWITH_CLIENTS=OFF"
    "-DWITH_BROKER=OFF"
    "-DWITH_APPS=OFF"
    "-DWITH_PLUGINS=OFF"
    "-DDOCUMENTATION=OFF"
  ];

  meta = with lib; {
    description = "Eclipse Mosquitto - An open source MQTT broker";
    homepage = "https://github.com/eclipse/mosquitto";
    license = "Eclipse Public License 2.0";
    maintainers = with maintainers; [ ];
    platforms = platforms.linux;
  };
}
