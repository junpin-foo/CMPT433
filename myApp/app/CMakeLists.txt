# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable(myapp ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(myapp LINK_PRIVATE hal)

# Copy executable to final location (change `myapp` to project name as needed)
add_custom_command(TARGET myapp POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:myapp>"
     "~/cmpt433/public/myapp" 
  COMMENT "Copying ARM executable to public NFS directory")
