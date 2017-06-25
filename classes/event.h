#ifndef HAVE_PHP_SDL_EVENT_H
#define HAVE_PHP_SDL_EVENT_H

extern zend_class_entry *sdlEvent_ce;

typedef struct _php_sdl_event_t {
	SDL_Event event;
	zend_object std;
} php_sdl_event_t;

#define php_sdl_event_from_obj(o) ((php_sdl_event_t*) ((char*) o - XtOffsetOf(php_sdl_event_t, std)))
#define php_sdl_event_fetch(z) php_sdl_event_from_obj(Z_OBJ_P(z))
#define SDL_EVENT_CONST(name) zend_declare_class_constant_long(sdlEvent_ce, ZEND_STRL(#name), SDL_##name);

PHP_MINIT_FUNCTION(SDL_Event);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
