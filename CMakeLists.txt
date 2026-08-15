cmake_minimum_required(VERSION 3.16)

project(WealthCraft LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64")

find_package(SFML 3 REQUIRED COMPONENTS
    Graphics
    Window
    System
    Audio
    Network
)

# Dear ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# ImGui-SFML
set(IMGUI_SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui-sfml)

set(IMGUI_SFML_SOURCES
    ${IMGUI_SFML_DIR}/imgui-SFML.cpp
)

add_executable(WealthCraft
    src/main.cpp
    src/Asset.cpp
    src/Bond.cpp
    src/Stock.cpp
    src/Portfolio.cpp
    src/Player.cpp
    src/Economy.cpp
    src/EventManager.cpp
    src/Game.cpp
    src/Crypto.cpp
    src/RealEstate.cpp
    src/FixedDeposit.cpp
    src/MutualFund.cpp
    src/Random.cpp
    src/Loan.cpp
    src/GameWindow.cpp

    ${IMGUI_SOURCES}
    ${IMGUI_SFML_SOURCES}
)

target_include_directories(WealthCraft PRIVATE

    include

    ${IMGUI_DIR}
    ${IMGUI_SFML_DIR}
)

target_link_libraries(WealthCraft PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    SFML::Network
    opengl32
)