add_library(engine 
    Engine.cpp
    Sprite.cpp
    Texture.cpp
    Shader.cpp
    Camera.cpp
    KeyInput.cpp
    Window.cpp
    FileManager.cpp
    Random.cpp
    Physics.cpp
    GameObject.cpp
    GameTime.cpp
    UITextRenderer.cpp
    UIText.cpp
    TextFont.cpp
    Animation.cpp
    SpriteRenderer.cpp
    MouseInput.cpp
    UIButton.cpp
    UISprite.cpp
    Scene.cpp
    UIObject.cpp
    )

target_include_directories( engine
                            PUBLIC 
                            ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/include)

target_link_libraries(engine ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/lib/GLEW/x64/glew32s.lib)
target_link_libraries(engine ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/lib/GLFW/glfw3.lib)

target_link_libraries(engine ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/lib/FreeType/win64/freetype.lib)

target_link_libraries(engine opengl32)
target_link_libraries(engine gdi32)

# This causes any target that links to this (engine) target
# automatically include ${CMAKE_CURRENT_SOURCE_DIR} - dir where all engine files are
target_include_directories(engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})