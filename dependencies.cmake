macro(EnableUseFetchContent)

    include(FetchContent)

    set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE ALWAYS)

    set(BUILD_VOICE_SUPPORT OFF)
    set(DPP_BUILD_TEST OFF)

    FetchContent_Declare(
            DPP
            GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
            GIT_TAG v10.0.28
    )

    FetchContent_MakeAvailable(DPP)

    unset(DPP_BUILD_TEST)
    unset(BUILD_VOICE_SUPPORT)

   FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.12.0
    )

    FetchContent_Declare(
        toml
        GIT_REPOSITORY https://github.com/ToruNiina/toml11.git
        GIT_TAG v3.7.1
    )

    FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
    )

    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG v1.14.0
    )

    FetchContent_Declare(
      mosquitto
      GIT_REPOSITORY https://github.com/eclipse/mosquitto.git
      GIT_TAG v2.0.18
    )

    FetchContent_MakeAvailable(spdlog toml json googletest mosquitto)

endmacro(EnableUseFetchContent)
