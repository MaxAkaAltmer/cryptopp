cmake_minimum_required(VERSION 3.10)
project(CryptoPP VERSION 8.9.0 LANGUAGES CXX ASM)

# C++17, стандарт
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции
option(BUILD_SHARED_LIBS "Build shared library" OFF)

option(CRYPTOPP_DISABLE_ASM "Disable assembly optimizations" ON)
option(CRYPTOPP_ENABLE_AVX "Enable AVX instructions" OFF)

# Автоматически находим все .cpp и .asm файлы
file(GLOB CRYPTOPP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.asm"
)

list(FILTER CRYPTOPP_SOURCES EXCLUDE REGEX "/(test|validat|bench|dlltest|fipstest|cryptest)\\.cpp$")

# Удаляем ASM-файлы, если отключено
if(CRYPTOPP_DISABLE_ASM)
    list(FILTER CRYPTOPP_SOURCES EXCLUDE REGEX "\\.asm$")
endif()

# Проверяем, что остались файлы
if(NOT CRYPTOPP_SOURCES)
    message(FATAL_ERROR "Не найдено ни одного исходного файла Crypto++. Убедитесь, что CMake запускается в корне папки с исходниками.")
endif()

# Определяем имя выходной библиотеки
set(CRYPTOPP_OUTPUT_NAME cryptopp)

# Создаём статическую библиотеку
add_library(cryptopp_static STATIC ${CRYPTOPP_SOURCES})
set_target_properties(cryptopp_static PROPERTIES
    OUTPUT_NAME ${CRYPTOPP_OUTPUT_NAME}
    POSITION_INDEPENDENT_CODE ON
)

if(CRYPTOPP_DISABLE_ASM)
    target_compile_definitions(cryptopp_static PUBLIC
        CRYPTOPP_DISABLE_ASM
        CRYPTOPP_DISABLE_SSE2
    )
endif()

# Подключаем заголовки
target_include_directories(cryptopp_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Компиляционные определения
target_compile_definitions(cryptopp_static PRIVATE
    CRYPTOPP_NO_GLOBAL_ASM_KEYWORD
)

# Поддержка AVX (опционально)
if(CRYPTOPP_ENABLE_AVX AND NOT MSVC)
    target_compile_options(cryptopp_static PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-mavx>)
endif()

# Флаги компилятора
target_compile_options(cryptopp_static PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
        -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-type-limits>
    $<$<CXX_COMPILER_ID:MSVC>:
        /W3 /wd4267 /wd4244 /wd4297 /wd4103>
)

# Для shared библиотеки (если BUILD_SHARED_LIBS=ON)
if(BUILD_SHARED_LIBS)
    add_library(cryptopp_shared SHARED ${CRYPTOPP_SOURCES})
    set_target_properties(cryptopp_shared PROPERTIES OUTPUT_NAME ${CRYPTOPP_OUTPUT_NAME})
    target_include_directories(cryptopp_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(cryptopp_shared PUBLIC CRYPTOPP_DLL=1)
    target_compile_definitions(cryptopp_shared PRIVATE CRYPTOPP_EXPORTS)

    # Флаги для shared
    target_compile_options(cryptopp_shared PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -fPIC>
    )
endif()

# Выбираем целевую библиотеку по умолчанию
if(BUILD_SHARED_LIBS)
    set(CRYPTOPP_TARGET cryptopp_shared)
else()
    set(CRYPTOPP_TARGET cryptopp_static)
endif()

# Установка
install(TARGETS ${CRYPTOPP_TARGET}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/cryptopp
    FILES_MATCHING PATTERN "*.h"
)


