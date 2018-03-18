find_path(HUNSPELL_INCLUDE_DIR
  NAMES
    hunspell.h
  PATH_SUFFIXES
    hunspell
)

find_library(HUNSPELL_LIBRARY
  NAMES
    hunspell
    hunspell-1.3
    hunspell-1.4
    hunspell-1.6
    libhunspell-1.6-0
)

find_package_handle_standard_args(HUNSPELL DEFAULT_MSG HUNSPELL_LIBRARY HUNSPELL_INCLUDE_DIR)

if(HUNSPELL_FOUND)
  set(HUNSPELL_INCLUDE_DIRS ${HUNSPELL_INCLUDE_DIR})
  set(HUNSPELL_LIBRARIES ${HUNSPELL_LIBRARY})
else()
  set(HUNSPELL_INCLUDE_DIRS)
  set(HUNSPELL_LIBRARIES)
endif()

mark_as_advanced(HUNSPELL_LIBRARY HUNSPELL_INCLUDE_DIR)