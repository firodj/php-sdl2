#ifndef HAVE_PHP_SDL_WINDOWPOS_H
#define HAVE_PHP_SDL_WINDOWPOS_H

extern zend_class_entry *sdlWindowPos_ce;

#define SDL_WINDOWPOS_CONST(name) zend_declare_class_constant_long(sdlWindowPos_ce, ZEND_STRL(#name), SDL_WINDOWPOS_##name);

PHP_MINIT_FUNCTION(SDL_WindowPos);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
