cmake_minimum_required(VERSION 2.8)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/dm  ${PROJECT_SOURCE_DIR}/src/dmu ${PROJECT_SOURCE_DIR}/src/dmgl)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)

SET(CRANK_SRC  crank.cpp )
ADD_EXECUTABLE(crank ${CRANK_SRC})
TARGET_LINK_LIBRARIES(crank ${GLUT_LIBRARY} ${OPENGL_LIBRARY} dm dmu dmgl)

SET(CARTS_SRC  carts.cpp )
ADD_EXECUTABLE(carts ${CARTS_SRC})
TARGET_LINK_LIBRARIES(carts ${GLUT_LIBRARY} ${OPENGL_LIBRARY} dm dmu dmgl)

SET(DT_SRC  dynamic_cast_test.cpp )
ADD_EXECUTABLE(dctest ${DT_SRC})
TARGET_LINK_LIBRARIES(dctest ${GLUT_LIBRARY} ${OPENGL_LIBRARY} dm dmu)

# set(DEMOS simple start pendulum tree carts wire ring )
# set(CORELIBS ${GLUT_LIBRARY} ${OPENGL_LIBRARY} dm dmu dmgl)

# foreach(demo ${DEMOS}
#   add_executable(${demo} ${demo}.cpp)
#   target_link_libraries(${demo} ${CORELIBS})
# endforeach(demo)



