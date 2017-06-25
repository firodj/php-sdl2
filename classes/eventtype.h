#ifndef HAVE_PHP_SDL_EVENTTYPE_H
#define HAVE_PHP_SDL_EVENTTYPE_H

extern zend_class_entry *sdlEventType_ce;

#define SDL_EVENTTYPE_CONST(name) zend_declare_class_constant_long(sdlEventType_ce, ZEND_STRL(#name), SDL_##name);

PHP_MINIT_FUNCTION(SDL_EventType);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
