set (BIN_NAME fix)
MESSAGE("Processing product build [${CMAKE_CURRENT_SOURCE_DIR}]")

EXECUTE_PROCESS(
    COMMAND find ${CMAKE_CURRENT_SOURCE_DIR} -maxdepth 10 -name *.cpp
    OUTPUT_VARIABLE MY_SRC_FILES)
string(REPLACE "\n" ";" MY_ALL_SRC ${MY_SRC_FILES})

MESSAGE("File list [${MY_ALL_SRC}]")

add_executable (${BIN_NAME} 
                ${MY_ALL_SRC})
                 
target_link_libraries(${BIN_NAME} ${MY_COMMON_LIBS} ${MY_BOOST_LIBS})
install (TARGETS ${BIN_NAME} DESTINATION ${PROD_INSTALL_DIR}/bin)

