find_package(LibClang REQUIRED)

include_directories(${LIBCLANG_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${LIBCLANG_LIBRARIES})
  
add_executable(check-override
	main.cc
	clang-data.cc
	method.cc
	report.cc
	printer.cc

	Cursor.cc
	Diagnostic.cc
	DiagnosticSet.cc
	Index.cc
	String.cc
	TranslationUnit.cc

	clang-data.hh
	cursor_type_x.inl
	method.hh
	report.hh
	printer.hh

	clang-cc-base.hh
	Cursor.hh
	Diagnostic.hh
	DiagnosticSet.hh
	Index.hh
	String.hh
	TranslationUnit.hh
	)
	
target_link_libraries(check-override ${LIBCLANG_LIBRARY})
set_property(TARGET check-override PROPERTY CXX_STANDARD 14)
