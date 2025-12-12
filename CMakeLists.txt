cmake_minimum_required(VERSION 3.16)
project(JogoTriatlo)

set(CMAKE_CXX_STANDARD 11)

# Opção 1: Tentar encontrar raylib via find_package (se instalado)
find_package(raylib QUIET)

# Opção 2: Se não encontrou, usar raylib local
if(NOT raylib_FOUND)
    # Caminhos comuns para raylib
    set(RAYLIB_INCLUDE_PATHS
        "${CMAKE_SOURCE_DIR}/raylib/include"
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/src"
        "C:/raylib/include"
        "C:/raylib/src"
    )
    
    set(RAYLIB_LIB_PATHS
        "${CMAKE_SOURCE_DIR}/raylib/lib"
        "${CMAKE_SOURCE_DIR}/lib"
        "C:/raylib/lib"
    )
    
    # Procurar headers
    find_path(RAYLIB_INCLUDE_DIR raylib.h
        PATHS ${RAYLIB_INCLUDE_PATHS}
    )
    
    # Procurar biblioteca
    find_library(RAYLIB_LIBRARY
        NAMES raylib libraylib
        PATHS ${RAYLIB_LIB_PATHS}
    )
    
    if(RAYLIB_INCLUDE_DIR AND RAYLIB_LIBRARY)
        message(STATUS "Raylib encontrado: ${RAYLIB_LIBRARY}")
        set(RAYLIB_FOUND TRUE)
    else()
        message(FATAL_ERROR 
            "Raylib nao encontrado!\n"
            "Por favor, instale o Raylib ou baixe de: https://github.com/raysan5/raylib/releases\n"
            "Extraia em C:/raylib/ ou na pasta do projeto"
        )
    endif()
endif()

# Coleta todos os .cpp em src/ pro executável
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Linkar com raylib
if(raylib_FOUND)
    target_link_libraries(${PROJECT_NAME} raylib)
else()
    target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARY})
endif()

# Linkar com bibliotecas do Windows necessárias para o Raylib
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        winmm      # Windows Multimedia API (para timeBeginPeriod/timeEndPeriod)
        gdi32      # Graphics Device Interface
        opengl32   # OpenGL
    )
endif()

# Copiar DLLs e música para a pasta do executável
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/src/libraylib.dll"
        "${CMAKE_SOURCE_DIR}/src/glfw3.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    
    # Copiar música OGG
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/img/Dave-the-Diver-OST-On-the-boat.ogg"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    
    # Copiar música OGG também para pasta src relativa (caso o executável rode de outra pasta)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_BINARY_DIR}/src"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/audio/Dave-the-Diver-OST-On-the-boat.ogg"
        "${CMAKE_BINARY_DIR}/audio/Dave-the-Diver-OST-On-the-boat.ogg"
    )
    
    # Copiar música da fase de corrida
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/audio/Pizza-Tower-OST-Thousand-March-_WAR_.ogg"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/audio/Pizza-Tower-OST-Thousand-March-_WAR_.ogg"
        "${CMAKE_BINARY_DIR}/audio/Pizza-Tower-OST-Thousand-March-_WAR_.ogg"
    )
endif()

# Copiar sprite do jogador
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/img/player.png"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Copiar também para pasta src
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    "${CMAKE_BINARY_DIR}/src"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/img/player.png"
    "${CMAKE_BINARY_DIR}/img/player.png"
)

# Copiar sprite do rato (obstáculo da fase de corrida)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/img/rato-Sheet.png"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/img/rato-Sheet.png"
    "${CMAKE_BINARY_DIR}/img/rato-Sheet.png"
)

# Copiar sprites da fase de natação
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/img/swimmer.png"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/img/swimmer.png"
    "${CMAKE_BINARY_DIR}/img/swimmer.png"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/img/hai-fin-shadow-Sheet-Sheet.png"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/img/hai-fin-shadow-Sheet-Sheet.png"
    "${CMAKE_BINARY_DIR}/img/hai-fin-shadow-Sheet-Sheet.png"
)

# Copiar música da fase de natação
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/audio/water.ogg"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/audio/water.ogg"
    "${CMAKE_BINARY_DIR}/audio/water.ogg"
)

# Copiar SFX de colisão (punch.ogg)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/audio/punch.ogg"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/audio/punch.ogg"
    "${CMAKE_BINARY_DIR}/audio/punch.ogg"
)

# Copiar SFX de vitória
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/audio/victoryff.swf.ogg"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/audio/victoryff.swf.ogg"
    "${CMAKE_BINARY_DIR}/audio/victoryff.swf.ogg"
)