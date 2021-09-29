project(firstapp) # set project name
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp) # executable name
enable_abcg(${PROJECT_NAME}) # abcg/cmake/ABCg.cmake