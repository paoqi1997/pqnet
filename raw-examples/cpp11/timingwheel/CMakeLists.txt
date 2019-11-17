cmake_minimum_required (VERSION 3.4)
project (timingwheel)

aux_source_directory (. SRCS)
add_executable (app ${SRCS})
