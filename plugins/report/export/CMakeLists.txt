# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

add_subdirectory(csvTxt)
add_subdirectory(html)

find_package(Quazip)
if (QUAZIP_FOUND)
    add_subdirectory(ods)
else(QUAZIP_FOUND)
    message("** Quazip was not found in your system, if you want to use ods export plugin, consider to download and install Quazip from \"http://sourceforge.net/projects/quazip/\" ")
endif(QUAZIP_FOUND)
