# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

add_subdirectory(arc)
add_subdirectory(charts)
add_subdirectory(field)
add_subdirectory(picture)
add_subdirectory(rectangle)
add_subdirectory(text)
add_subdirectory(chord)
add_subdirectory(ellipse)
add_subdirectory(htmlscript)
add_subdirectory(line)
add_subdirectory(pie)
add_subdirectory(script)

find_package(Zint)
if (ZINT_FOUND)
    add_subdirectory(barcode)
endif(ZINT_FOUND)
