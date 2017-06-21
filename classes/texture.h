#ifndef HAVE_PHP_SDL_TEXTURE_H
#define HAVE_PHP_SDL_TEXTURE_H

extern zend_class_entry *sdlTexture_ce;

typedef struct _php_sdl_texture_t {
	SDL_Texture *texture;
	zend_object std;
} php_sdl_texture_t;

#define php_sdl_texture_from_obj(o) ((php_sdl_texture_t*) ((char*) o - XtOffsetOf(php_sdl_texture_t, std)))
#define php_sdl_texture_fetch(z) php_sdl_texture_from_obj(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(SDL_Texture);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
