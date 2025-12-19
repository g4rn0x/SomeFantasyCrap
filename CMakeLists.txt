cmake_minimum_required(VERSION 3.16)
project(MyGame LANGUAGES CXX)

# Create directory for MOC output
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/MyGame_autogen)


# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set Qt6 path (adjust the version number if needed)
set(Qt6_DIR "D:/JetBrains/Qt/6.10.1/mingw_64/lib/cmake/Qt6")
set(CMAKE_PREFIX_PATH ${Qt6_DIR} ${CMAKE_PREFIX_PATH})

# Find required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)


# Add resource file
qt6_add_resources(RESOURCE_FILES
    src/resources.qrc
)

# Add executable
add_executable(MyGame
        src/main.cpp
        src/core/GameEngine.cpp
        src/core/GameEngine.h
        src/core/GameState.h
        src/core/GameState.cpp
        src/core/Types.h
        src/core/Constants.h
        src/utils/RandomGenerator.cpp
        src/utils/RandomGenerator.h
        src/database/DatabaseManager.h
        src/database/DatabaseManager.cpp
        src/database/DatabaseConnection.h
        src/database/DatabaseConnection.cpp
        src/ui/MainWindow.h
        src/ui/MainWindow.cpp
        src/ui/GameWidget.h
        src/ui/GameWidget.cpp
        src/ui/InventoryWidget.h
        src/ui/InventoryWidget.cpp
        ${RESOURCE_FILES}
        src/utils/TypeWriter.h
        src/utils/TypeWriter.cpp
        src/utils/TextGenerator.h
        src/utils/TextGenerator.cpp
        src/ui/RiddleDialog.h
        src/ui/RiddleDialog.cpp
        src/ui/NotesDialog.cpp
        src/ui/NotesDialog.h
        src/ui/InventoryPanel.h
        src/ui/InventoryPanel.cpp
)

# Link Qt libraries
target_link_libraries(MyGame PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
)

# Copy database file to the build directory
add_custom_command(TARGET MyGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/game_database.sql
    $<TARGET_FILE_DIR:MyGame>)

# Copy assets directory to the build directory
add_custom_command(TARGET MyGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/assets
    $<TARGET_FILE_DIR:MyGame>/assets)

# Include directories
target_include_directories(MyGame PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/src/database
        ${CMAKE_CURRENT_SOURCE_DIR}/src/models
)

# Copy required Qt DLLs to build directory on Windows
if(WIN32)
    add_custom_command(TARGET MyGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:MyGame>/../bin"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:MyGame>" "$<TARGET_FILE_DIR:MyGame>/../bin/"
        COMMAND ${CMAKE_COMMAND} -E copy "${Qt6_DIR}/../../../bin/Qt6Core.dll" "$<TARGET_FILE_DIR:MyGame>/../bin/"
        COMMAND ${CMAKE_COMMAND} -E copy "${Qt6_DIR}/../../../bin/Qt6Gui.dll" "$<TARGET_FILE_DIR:MyGame>/../bin/"
        COMMAND ${CMAKE_COMMAND} -E copy "${Qt6_DIR}/../../../bin/Qt6Widgets.dll" "$<TARGET_FILE_DIR:MyGame>/../bin/"
    )
endif()
