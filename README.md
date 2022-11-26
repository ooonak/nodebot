# nodebot
## Build
```bash
$ mkdir build; cd build
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ ..
$ cmake --build .
```

## Get token and create configuration
https://discord.com/api/oauth2/authorize?client_id=<client-id>&permissions=<permissions>&scope=bot%20applications.commands

```bash
$ more nodebot.conf
[config]
token = "<your top secret token>"
```

