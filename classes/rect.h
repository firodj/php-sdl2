#ifndef HAVE_PHP_SDL_RECT_H
#define HAVE_PHP_SDL_RECT_H

#ifdef  __cplusplus
extern "C" {
#endif

extern zend_class_entry *sdlRect_ce;

typedef struct _php_sdl_rect_t {
	zend_object std;
} php_sdl_rect_t;

zend_bool sdl_rect_to_zval(SDL_Rect *rect, zval *value TSRMLS_DC);
zend_bool zval_to_sdl_rect(zval *value, SDL_Rect *rect);

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
