#ifndef HAVE_PHP_SDL_POINT_H
#define HAVE_PHP_SDL_POINT_H

extern zend_class_entry *sdlPoint_ce;

typedef struct _php_sdl_point_t {
	SDL_Point point;
	zend_object std;
} php_sdl_point_t;

#define php_sdl_point_from_obj(o) ((php_sdl_point_t*) ((char*) o - XtOffsetOf(php_sdl_point_t, std)))
#define php_sdl_point_fetch(z) php_sdl_point_from_obj(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(SDL_Point);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
