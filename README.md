# nodebot
## Build, test & run
### Only Fetch_Content
```bash
$ cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_PREFIX=$HOME/usr/local
$ cmake --build build
$ ctest --test-dir build

# Or access artifacts directly
$ ./build/tests/NodeBot 
$ ./build/example/NodeBot

# Install
$ cmake --install build

# Pack
$ cd build; cpack -G TGZ 
```

### With Conan
```bash 
$ conan profile detect --force

$ conan install .  --profile default --output-folder=build --build=missing --settings=build_type=<Release/Debug>
$ cmake -B build -G Ninja --preset conan-<Release/Debug>
$ cmake --build build

# Same without presets
cmake -S. -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=./build/build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=<Release/Debug>
cmake --build build
```

If CLion Enable conan preset profile under Settings -> Build, Execution, Deployment -> CMake.
Then select the "conan-<release/debug>" build profile.

## Get token and create configuration
https://discord.com/api/oauth2/authorize?client_id=<client-id>&permissions=<permissions>&scope=bot%20applications.commands

```bash
$ more nodebot.conf
[config]
token = "<your top secret token>"
```

