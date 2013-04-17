 #  Created on: 1 Jan 2013
 #      Author: Wei Liew [wei.liew@outlook.com]
 #
 #  Copyright Wei Liew 2012 - 2013.
 #  Distributed under the Boost Software License, Version 1.0.
 #  (See http://www.boost.org/LICENSE_1_0.txt)
 
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

