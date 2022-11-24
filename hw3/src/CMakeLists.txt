project(HW3 C CXX)

set(HW3_SOURCE
  ${HW3_SOURCE_DIR}/camera.cpp
  ${HW3_SOURCE_DIR}/gl_helper.cpp
  ${HW3_SOURCE_DIR}/main.cpp
  ${HW3_SOURCE_DIR}/model.cpp
  ${HW3_SOURCE_DIR}/opengl_context.cpp
  ${HW3_SOURCE_DIR}/Programs/program.cpp
  ${HW3_SOURCE_DIR}/Programs/light.cpp
  ${HW3_SOURCE_DIR}/Programs/filter.cpp
  ${HW3_SOURCE_DIR}/Programs/shadow.cpp
  ${HW3_SOURCE_DIR}/Programs/shadowLight.cpp
  ${HW3_SOURCE_DIR}/Programs/skybox.cpp
)

set(HW3_HEADER
  ${HW3_SOURCE_DIR}/../include/camera.h
  ${HW3_SOURCE_DIR}/../include/context.h
  ${HW3_SOURCE_DIR}/../include/gl_helper.h
  ${HW3_SOURCE_DIR}/../include/model.h
  ${HW3_SOURCE_DIR}/../include/opengl_context.h
  ${HW3_SOURCE_DIR}/../include/program.h
  ${HW3_SOURCE_DIR}/../include/utils.h
)
add_executable(HW3 ${HW3_SOURCE} ${HW3_HEADER})
target_include_directories(HW3 PRIVATE ${HW3_SOURCE_DIR}/../include)

add_dependencies(HW3 glad glfw glm stb)
# Can include glfw and glad in arbitrary order
target_compile_definitions(HW3 PRIVATE GLFW_INCLUDE_NONE)
# More warnings
if (NOT MSVC)
  target_compile_options(HW3
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
    PRIVATE "-Wpedantic"
  )
endif()
# Prefer std c++20, at least need c++17 to compile
set_target_properties(HW3 PROPERTIES
  CXX_STANDARD 20
  CXX_EXTENSIONS OFF
)

target_link_libraries(HW3
  PRIVATE glad
  PRIVATE glfw
  PRIVATE stb
)

if (TARGET glm::glm_shared)
  target_link_libraries(HW3 PRIVATE glm::glm_shared)
elseif(TARGET glm::glm_static)
  target_link_libraries(HW3 PRIVATE glm::glm_static)
else()
  target_link_libraries(HW3 PRIVATE glm::glm)
endif()
