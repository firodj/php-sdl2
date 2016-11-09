#ifndef HAVE_PHP_SDL_RENDERER_H
#define HAVE_PHP_SDL_RENDERER_H

extern zend_class_entry *sdlRenderer_ce;

typedef struct _php_sdl_renderer_t {
	SDL_Renderer *r;
	zend_object std;
} php_sdl_renderer_t;

#define php_sdl_renderer_from(o) ((php_sdl_renderer_t*) ((char*) o - XtOffsetOf(php_sdl_renderer_t, std)))
#define php_sdl_renderer_fetch(z) php_sdl_renderer_from(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(SDL_Renderer);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
