#ifndef HAVE_PHP_SDL_PIXELFORMAT_H
#define HAVE_PHP_SDL_PIXELFORMAT_H

extern zend_class_entry *sdlPixelFormat_ce;

#define SDL_PIXELFORMAT_CONST(name) zend_declare_class_constant_long(sdlPixelFormat_ce, ZEND_STRL(#name), SDL_PIXELFORMAT_##name);

PHP_MINIT_FUNCTION(SDL_PixelFormat);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
