find_package(LibClang REQUIRED)

include_directories(${LIBCLANG_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
link_directories(${LIBCLANG_LIBRARIES})
  
add_executable(check-override
	src/main.cc
	src/clang-data.cc
	src/method.cc
	src/report.cc

	src/Cursor.cc
	src/Diagnostic.cc
	src/DiagnosticSet.cc
	src/Index.cc
	src/String.cc
	src/TranslationUnit.cc

	src/clang-data.hh
	src/cursor_type_x.inl
	src/method.hh
	src/report.hh

	src/clang-cc-base.hh
	src/Cursor.hh
	src/Diagnostic.hh
	src/DiagnosticSet.hh
	src/Index.hh
	src/String.hh
	src/TranslationUnit.hh
	)
	
target_link_libraries(check-override ${LIBCLANG_LIBRARY})
