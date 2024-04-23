add_test(NAME Test1 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../${PROJECT_NAME}
    -i ${CMAKE_CURRENT_SOURCE_DIR}/data/sample-input-SQ24.txt
    -o ${CMAKE_CURRENT_BINARY_DIR}/test_output.txt)

add_test(NAME Test2 COMMAND ${CMAKE_COMMAND}
    -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/test_output.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/data/sample-output.txt)