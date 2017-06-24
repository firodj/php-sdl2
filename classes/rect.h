#ifndef HAVE_PHP_SDL_RECT_H
#define HAVE_PHP_SDL_RECT_H

#ifdef  __cplusplus
extern "C" {
#endif

extern zend_class_entry *sdlRect_ce;

typedef struct _php_sdl_rect_t {
	SDL_Rect rect;
	zend_object std;
} php_sdl_rect_t;

#define php_sdl_rect_from_obj(o) ((php_sdl_rect_t*) ((char*) o - XtOffsetOf(php_sdl_rect_t, std)))
#define php_sdl_rect_fetch(z) php_sdl_rect_from_obj(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(SDL_Rect);

#ifdef  __cplusplus
} // extern "C" 
#endif

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
