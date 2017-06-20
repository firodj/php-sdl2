#ifndef HAVE_PHP_SDL_SURFACE_H
#define HAVE_PHP_SDL_SURFACE_H

extern zend_class_entry *sdlSurface_ce;

typedef struct _php_sdl_surface_t {
  SDL_Surface *surface;
  zend_object std;
} php_sdl_surface_t;

#define php_sdl_surface_from_obj(o) ((php_sdl_surface_t*) ((char*) o - XtOffsetOf(php_sdl_surface_t, std)))
#define php_sdl_surface_fetch(z) php_sdl_surface_from_obj(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(SDL_Surface);

#endif
