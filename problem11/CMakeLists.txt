project(problem11)

file(GLOB src *.cpp *.hpp *.h)


set( CMAKE_CXX_FLAGS "-std=c++1y -stdlib=libc++" )

add_executable(problem11 ${src})

target_link_libraries( problem11
)

