#ifndef HAVE_PHP_SDL_WINDOWEVENT_H
#define HAVE_PHP_SDL_WINDOWEVENT_H

extern zend_class_entry *sdlWindowEvent_ce;

#define SDL_WINDOWEVENT_CONST(name) zend_declare_class_constant_long(sdlWindowEvent_ce, ZEND_STRL(#name), SDL_WINDOWEVENT_##name);

PHP_MINIT_FUNCTION(SDL_WindowEvent);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
