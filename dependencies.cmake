macro(EnableUseFetchContent)

    include(FetchContent)

    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0
    )

    FetchContent_Declare(
        toml
        GIT_REPOSITORY https://github.com/ToruNiina/toml11.git
        GIT_TAG v3.7.1
    )

    FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
    )

    FetchContent_Declare(
      DPP
      GIT_REPOSITORY https://github.com/brainboxdotcc/DPP.git
      GIT_TAG v10.0.23
    )

    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG release-1.12.1
    )

    FetchContent_MakeAvailable(spdlog toml json DPP googletest)

endmacro(EnableUseFetchContent)
