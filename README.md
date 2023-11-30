# nodebot
### Build, test & run
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

## Get token and create configuration
https://discord.com/api/oauth2/authorize?client_id=<client-id>&permissions=<permissions>&scope=bot%20applications.commands

```bash
$ more nodebot.conf
[config]
token = "<your top secret token>"
```

