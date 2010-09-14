# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

add_subdirectory(csvTxt)
add_subdirectory(html)

find_package(Quazip)
if (QUAZIP_FOUND AND NOT APPLE)
    add_subdirectory(ods)
else(QUAZIP_FOUND AND NOT APPLE)
    message("** Quazip was not found in your system, if you want to use ods export plugin, consider to download and install Quazip from it's original site \"http://sourceforge.net/projects/quazip/\" or a custom cmaked version from \"http://code.google.com/p/exaro/downloads/list\" ")
endif(QUAZIP_FOUND AND NOT APPLE)
