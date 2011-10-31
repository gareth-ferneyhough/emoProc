include_directories ("${PROJECT_SOURCE_DIR}/third-party/include")
include_directories ("${PROJECT_SOURCE_DIR}/third-party/libs")
include_directories ("${PROJECT_SOURCE_DIR}/src/alg")
include_directories ("${PROJECT_SOURCE_DIR}/src/svm")

file(GLOB_RECURSE emote_SRC
  "*.c*"
)

set (EXTRA_LIBS ${EXTRA_LIBS} boost_thread jack jackcpp aubio)

add_executable (emote ${emote_SRC})

target_link_libraries (emote ${EXTRA_LIBS})

install (TARGETS emote DESTINATION bin)
