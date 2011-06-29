include_directories ("${PROJECT_SOURCE_DIR}/third-party/include")
include_directories ("${PROJECT_SOURCE_DIR}/third-party/libs")

file(GLOB emote_SRC
  "*.cpp"
)

set (EXTRA_LIBS ${EXTRA_LIBS} boost_thread jack jackcpp aubio)

add_executable (emote ${emote_SRC})

target_link_libraries (emote ${EXTRA_LIBS})