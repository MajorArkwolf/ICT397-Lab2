
# Add source files.
target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
	engine.cpp
	scene.cpp
	terrain.cpp
	texture.cpp
	Camera.cpp
	indexcalculator.cpp
)

# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})