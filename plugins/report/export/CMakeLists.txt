# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

add_subdirectory(csvTxt)
add_subdirectory(html)

find_package(Quazip)
if (QUAZIP_FOUND)
    add_subdirectory(ods)
endif(QUAZIP_FOUND)
