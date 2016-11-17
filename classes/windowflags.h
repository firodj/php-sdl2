#ifndef HAVE_PHP_SDL_WINDOWFLAGS_H
#define HAVE_PHP_SDL_WINDOWFLAGS_H

extern zend_class_entry *sdlWindowFlags_ce;

#define SDL_WINDOWFLAGS_CONST(name) zend_declare_class_constant_long(sdlWindowFlags_ce, ZEND_STRL(#name), SDL_WINDOW_##name);

PHP_MINIT_FUNCTION(SDL_WindowFlags);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
