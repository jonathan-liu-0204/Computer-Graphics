project(HW0 C CXX)

set(HW0_SOURCE
  ${HW0_SOURCE_DIR}/context_manager.cpp
  ${HW0_SOURCE_DIR}/main.cpp
)

add_executable(HW0 ${HW0_SOURCE})

target_include_directories(HW0 PRIVATE ${HW0_SOURCE_DIR}/../include)

add_dependencies(HW0 glad glfw)
# Can include glfw and glad in arbitrary order
target_compile_definitions(HW0 PRIVATE GLFW_INCLUDE_NONE)
# More warnings
if (NOT MSVC)
  target_compile_options(HW0
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
    PRIVATE "-Wpedantic"
  )
endif()
# Use std c++ 17
set_target_properties(HW0 PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED TRUE
  CXX_EXTENSIONS OFF
)

target_link_libraries(HW0
  PRIVATE glad
  PRIVATE glfw
)
