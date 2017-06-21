#ifndef HAVE_PHP_SDL_TEXTUREACCESS_H
#define HAVE_PHP_SDL_TEXTUREACCESS_H

#define SDL_TEXTUREACCESS_CONST(name) zend_declare_class_constant_long(sdlTextureAccess_ce, ZEND_STRL(#name), SDL_TEXTUREACCESS_##name);

PHP_MINIT_FUNCTION(SDL_TextureAccess);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
