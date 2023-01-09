#! func for create static lib
# \flag:LOG         - [optional]    if set func print other data 
# \param:LNAME      - [optional]    static lib name, if not defined set like folder name
# \group:SRCS       - [must]        source of lib
# \group:INCLUDE    - [optional]    path to headers
# \group:PRIV_REQ   - [optional]    private libs req
# \group:PUB_REQ    - [optional]    public libs req
#
function(SLIB_INIT)
    cmake_parse_arguments(
        slib
        "LOG"
        "LNAME"
        "SRCS;INCLUDE;PRIV_REQ;PUB_REQ"
        ${ARGN}
    )
    # check LNAME and SRCS exist
    if(NOT DEFINED slib_LNAME)
        get_filename_component(slib_LNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
        # message(FATAL_ERROR "lib name must be defined")
    endif()
    if(NOT DEFINED slib_SRCS)
        message(FATAL_ERROR "${slib_LNAME} lib sources must be defined")
    endif()

    if(slib_LOG)
        message("\n")
        message("LNAME:\t" ${slib_LNAME})
        message("SRCS:\t" ${slib_SRCS})
        message("INCLUDE:\t" ${slib_INCLUDE})
        message("PRIV_REQ:\t" ${slib_PRIV_REQ})
        message("PUB_REQ:\t" ${slib_PUB_REQ})
        message("\n")
    endif()

    add_library(${slib_LNAME} STATIC ${slib_SRCS})# Create static lib
    if(DEFINED slib_INCLUDE)
        # add public include dir
        target_include_directories(${slib_LNAME} 
            PUBLIC ${slib_INCLUDE}
        )
    endif()
    # link private and public libs
    target_link_libraries(${slib_LNAME} 
        PRIVATE ${slib_PRIV_REQ}
        PUBLIC ${slib_PUB_REQ}
    )
endfunction(SLIB_INIT)

#! func for multi add subdirs
# \group:ARGN       - [must]        paths to dirs with subdirs
#
function(ADD_SUBDIRS)
    foreach(spath IN ITEMS ${ARGN})
        file(GLOB subsubdirs RELATIVE_PATH ${spath}/*)
        foreach(subd IN ITEMS ${subsubdirs})
            if(IS_DIRECTORY ${subd})
                add_subdirectory(${subd})
            endif()
        endforeach()
    endforeach()
endfunction(ADD_SUBDIRS)
