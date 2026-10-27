add_library(libcompressor STATIC
            src/libcompressor.cpp)

set_target_properties(libcompressor PROPERTIES PREFIX "")

target_include_directories(libcompressor
                           PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(libcompressor
                      PRIVATE
                      ZLIB::ZLIB
                      BZip2::BZip2
                      project_warnings)

install(TARGETS libcompressor
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY include/libcompressor
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(tests)