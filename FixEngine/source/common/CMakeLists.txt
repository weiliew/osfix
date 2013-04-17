 #  Created on: 1 Jan 2013
 #      Author: Wei Liew [wei.liew@outlook.com]
 #
 #  Copyright Wei Liew 2012 - 2013.
 #  Distributed under the Boost Software License, Version 1.0.
 #  (See http://www.boost.org/LICENSE_1_0.txt)
 
#
# Automatgically include all the directories to be built
#
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
foreach(subdir ${SUBDIRS})
    # Build and set include for sub dir - if it has a CMakeList.txt file
    EXECUTE_PROCESS(
        COMMAND find ${CMAKE_CURRENT_SOURCE_DIR}/${subdir} -name CMakeLists.txt
        OUTPUT_VARIABLE CMAKE_EXIST)
            
    if (CMAKE_EXIST)
        MESSAGE("Adding common sub directory [${subdir}]")
        include_directories (${subdir})
        add_subdirectory (${subdir})
    endif (CMAKE_EXIST)
ENDFOREACH(subdir)


 