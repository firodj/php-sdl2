#ifndef HAVE_PHP_SDL_WINDOW_H
#define HAVE_PHP_SDL_WINDOW_H

extern zend_class_entry *sdlWindow_ce;

typedef struct _php_sdl_window_t {
	SDL_Window *window;
	zend_object std;
} php_sdl_window_t;

#define php_sdl_window_from(o) ((php_sdl_window_t*) ((char*) o - XtOffsetOf(php_sdl_window_t, std)))
#define php_sdl_window_fetch(z) php_sdl_window_from(Z_OBJ_P(z))
#define SDL_WINDOW_CONST(name) zend_declare_class_constant_long(sdlWindow_ce, ZEND_STRL(#name), SDL_WINDOW_##name);

PHP_MINIT_FUNCTION(SDL_Window);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
