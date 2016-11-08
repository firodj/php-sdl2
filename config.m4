PHP_ARG_WITH(sdl2, for sdl2 support,
[  --with-sdl2             Include sdl2 support])

if test "$PHP_SDL2" != "no"; then
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/SDL2/SDL.h"

  if test -r $PHP_SDL2/$SEARCH_FOR; then
    SDL2_DIR=$PHP_SDL2
  else
    AC_MSG_CHECKING([for sdl2 files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        SDL2_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  
  if test -z "$SDL2_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the sdl2 distribution])
  fi
  
  PHP_ADD_INCLUDE($SDL2_DIR/include)
  
  LIBNAME=SDL2
  LIBSYMBOL=SDL_HasAVX

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SDL2_DIR/$PHP_LIBDIR, SDL2_SHARED_LIBADD)
    AC_DEFINE(HAVE_SDL2LIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong sdl2 lib version or lib not found])
  ],[
    -L$SDL2_DIR/$PHP_LIBDIR -lm
  ])
  
  PHP_SUBST(SDL2_SHARED_LIBADD)

  PHP_NEW_EXTENSION(sdl2, \
    sdl2.c \
    classes/exceptions.c \
    , $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
