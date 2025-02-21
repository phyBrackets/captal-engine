
set(CATCH_USE_VALGRIND      OFF CACHE INTERNAL "")
set(CATCH_BUILD_TESTING     OFF CACHE INTERNAL "")
set(CATCH_BUILD_EXAMPLES    OFF CACHE INTERNAL "")
set(CATCH_BUILD_EXTRA_TESTS OFF CACHE INTERNAL "")
set(CATCH_ENABLE_COVERAGE   OFF CACHE INTERNAL "")
set(CATCH_ENABLE_WERROR     OFF CACHE INTERNAL "")
set(CATCH_INSTALL_DOCS      OFF CACHE INTERNAL "")
set(CATCH_INSTALL_HELPERS   OFF CACHE INTERNAL "")

captal_download_submodule(external/catch TRUE)
add_subdirectory(external/catch EXCLUDE_FROM_ALL)

unset(CATCH_USE_VALGRIND      CACHE)
unset(CATCH_BUILD_TESTING     CACHE)
unset(CATCH_BUILD_EXAMPLES    CACHE)
unset(CATCH_BUILD_EXTRA_TESTS CACHE)
unset(CATCH_ENABLE_COVERAGE   CACHE)
unset(CATCH_ENABLE_WERROR     CACHE)
unset(CATCH_INSTALL_DOCS      CACHE)
unset(CATCH_INSTALL_HELPERS   CACHE)
