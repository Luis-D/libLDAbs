#Luis Delgado. 2020

#CMake file

#To Build shared libraries call cmake with -DBUILD_SHARED_LIBS=1
#To NOT Build includes call cmake with -DNOT_INCLUDES=1



cmake_minimum_required(VERSION 3.0)


SET(PROJECTNAME LDAbs )

project(lib${PROJECTNAME}
    VERSION 2.0.1
    DESCRIPTION "Abstract Data Types for C programming language"
    HOMEPAGE_URL https://github.com/Luis-D/libLDAbs
    LANGUAGES "C"
    )


SET(SRCSDIR ${PROJECT_SOURCE_DIR}/src)


IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE Release)
ENDIF()


set (srcs 
	${SRCSDIR}/DoublyLinkedList.c
	${SRCSDIR}/SinglyLinkedList.c
	${SRCSDIR}/GeneralLinkedList.c
	${SRCSDIR}/GeneralLinkedList.c
	${SRCSDIR}/ObjectPool.c
	${SRCSDIR}/SinglyLinkedList.c
	${SRCSDIR}/Weak_Stack_Queue.c
	${SRCSDIR}/ThreadPool.c
    )


set (LIB_INSTALL ${CMAKE_INSTALL_PREFIX}/lib)
set (INC_INSTALL ${CMAKE_INSTALL_PREFIX}/include)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_library(${PROJECTNAME}  ${srcs} ${SRCSDIR}/ThreadPool/PThreadpool.c)

IF ( NOT NOT_INCLUDES)


    add_custom_command(
	TARGET ${PROJECTNAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory include/${PROJECTNAME}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SRCSDIR}/*.h include/${PROJECTNAME})

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECTNAME} DESTINATION ${INC_INSTALL})
ENDIF()

install(TARGETS ${PROJECTNAME} DESTINATION ${LIB_INSTALL})


find_package(Doxygen)
if(${DOXYGEN_FOUND} STREQUAL "YES")
    
    set(DOXYGEN_GENERATE_MAN NO)    
    set(DOXYGEN_GENERATE_LATEX NO)    
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
    set(DOXYGEN_LAYOUT_FILE "${PROJECT_SOURCE_DIR}/docs/libldabsdocslayout")
    set(DOXYGEN_PROJECT_NAME ${PROJECTNAME})
    set(DOXYGEN_HIDE_UNDOC_MEMBERS YES)
    set(DOXYGEN_HIDE_UNDOC_CLASSES YES)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_REFERENCES_LINK_SOURCE NO)
    set(DOXYGEN_SOURCE_TOOLTIPS NO)
    set(DOXYGEN_VERBATIM_HEADERS NO)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(DOXYGEN_RECURSIVE NO)
    set(DOXYGEN_DOT_MULTI_TARGETS NO)

    doxygen_add_docs(docs ${PROJECT_SOURCE_DIR}/README.md "${SRCSDIR}"  COMMENT "Building docs" )
ENDIF()
