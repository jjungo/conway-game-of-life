
function(find_catch2)
	set(Catch2_FOUND Catch2-NOTFOUND PARENT_SCOPE)
	set(catch_cmake_lists ${PROJECT_SOURCE_DIR}/external/Catch2/CMakeLists.txt)
	if (NOT EXISTS ${catch_cmake_lists})
		find_package(Git)
		if (NOT Git_FOUND)
			MESSAGE(WARNING "git not found! ")
			return()
		endif()
		execute_process(
				COMMAND ${GIT_EXECUTABLE} submodule update --init -- Catch2
				WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external
				RESULT_VARIABLE git_submodule_error
				ERROR_QUIET)
		if (git_submodule_error OR NOT EXISTS ${catch_cmake_lists})
			return()
		endif()
	endif()
	add_subdirectory(external/Catch2)
	list(APPEND CMAKE_MODULE_PATH
			${PROJECT_SOURCE_DIR}/external/Catch2/contrib)
	set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
	set(Catch2_FOUND 1 PARENT_SCOPE)

endfunction()