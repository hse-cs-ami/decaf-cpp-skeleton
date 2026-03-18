cmake_minimum_required(VERSION 3.16)
project(decaf CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------
# ANTLR4 tool (генерация C++-кода из грамматик)
# --------------------------------------------------------------------------

# Путь к antlr4-complete.jar: можно переопределить при вызове cmake:
#   cmake -DANTLR_JAR=/path/to/antlr4.jar ..
if(NOT ANTLR_JAR)
    find_file(ANTLR_JAR_FOUND
        NAMES antlr4-complete.jar antlr4.jar
        PATHS /usr/local/lib /usr/share/java ${CMAKE_SOURCE_DIR}
        NO_DEFAULT_PATH
    )
    if(ANTLR_JAR_FOUND)
        set(ANTLR_JAR ${ANTLR_JAR_FOUND})
    else()
        message(FATAL_ERROR
            "antlr4-complete.jar не найден. "
            "Укажите путь: cmake -DANTLR_JAR=/path/to/antlr4-complete.jar ..")
    endif()
endif()

# Генерация C++-файлов из грамматик.
set(ANTLR_OUT ${CMAKE_CURRENT_BINARY_DIR}/antlr)
file(MAKE_DIRECTORY ${ANTLR_OUT})

# Сначала генерируем лексер (parser зависит от его токенов через tokenVocab).
add_custom_command(
    OUTPUT
        ${ANTLR_OUT}/DecafScanner.cpp ${ANTLR_OUT}/DecafScanner.h
        ${ANTLR_OUT}/DecafScanner.tokens
    COMMAND java -jar ${ANTLR_JAR}
            -Dlanguage=Cpp -o ${ANTLR_OUT}
            ${CMAKE_CURRENT_SOURCE_DIR}/DecafScanner.g4
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/DecafScanner.g4
    COMMENT "Генерация C++ из DecafScanner.g4"
)

# Затем генерируем парсер, передавая папку с .tokens через -lib.
add_custom_command(
    OUTPUT
        ${ANTLR_OUT}/DecafParser.cpp ${ANTLR_OUT}/DecafParser.h
    COMMAND java -jar ${ANTLR_JAR}
            -Dlanguage=Cpp -o ${ANTLR_OUT} -lib ${ANTLR_OUT}
            ${CMAKE_CURRENT_SOURCE_DIR}/DecafParser.g4
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/DecafParser.g4
        ${ANTLR_OUT}/DecafScanner.tokens
    COMMENT "Генерация C++ из DecafParser.g4"
)

# --------------------------------------------------------------------------
# ANTLR4 C++ runtime
# --------------------------------------------------------------------------

find_package(antlr4-runtime REQUIRED CONFIG)

# --------------------------------------------------------------------------
# Цель сборки
# --------------------------------------------------------------------------

add_executable(decaf
    main.cpp
    ${ANTLR_OUT}/DecafScanner.cpp
    ${ANTLR_OUT}/DecafParser.cpp
)

target_include_directories(decaf PRIVATE /usr/include/antlr4-runtime ${ANTLR_OUT})
target_link_libraries(decaf PRIVATE antlr4_static)
