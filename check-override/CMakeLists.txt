find_package(LibClang REQUIRED)

include_directories(${LIBCLANG_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
link_directories(${LIBCLANG_LIBRARIES})
  
add_executable(check-override
	src/main.cc
	src/Cursor.cc
	src/Diagnostic.cc
	src/DiagnosticSet.cc
	src/Index.cc
	src/String.cc
	src/TranslationUnit.cc
	)
	
target_link_libraries(check-override ${LIBCLANG_LIBRARY})
