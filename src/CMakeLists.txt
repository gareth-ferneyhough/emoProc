include_directories ("${PROJECT_SOURCE_DIR}/src/alg")
include_directories ("${PROJECT_SOURCE_DIR}/src/svm")
link_directories ("${PROJECT_SOURCE_DIR}/src/lib")

file(GLOB_RECURSE emote_SRC
  "*.c*"
)

set (EXTRA_LIBS ${EXTRA_LIBS} boost_thread jack getf0 m sndfile)

add_executable (emote ${emote_SRC})

target_link_libraries (emote ${EXTRA_LIBS})

install (TARGETS emote DESTINATION bin)
