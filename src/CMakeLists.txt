# Generic test that uses conan libs
add_library(numbers include/magnitude.hh)
target_sources(numbers PUBLIC include/bignum_template.hh PRIVATE main.cpp)
target_include_directories(numbers PUBLIC include)
target_link_libraries(
        numbers
        PRIVATE project_options
        project_warnings
        CONAN_PKG::docopt.cpp
        CONAN_PKG::fmt
        CONAN_PKG::spdlog)
