#ifndef HAVE_PHP_SDL_TIMER_H
#define HAVE_PHP_SDL_TIMER_H

extern zend_class_entry *sdlTimer_ce;

typedef struct _php_sdl_timer_t {
	SDL_TimerID id;
	zend_object std;
} php_sdl_timer_t;

struct _php_sdl_globals_t {
	struct _strings {
		zend_string *run;
	} strings;
};

extern struct _php_sdl_globals_t php_sdl_globals;

#define php_sdl_timer_from_obj(o) ((php_sdl_timer_t*) ((char*) o - XtOffsetOf(php_sdl_timer_t, std)))
#define php_sdl_timer_fetch(z) php_sdl_timer_from_obj(Z_OBJ_P(z))
#define SDL_TIMER_CONST(name) zend_declare_class_constant_long(sdlTimer_ce, ZEND_STRL(#name), SDL_TIMER_##name);
#define SDL_G(v) php_sdl_globals.v
#define SDL_TIMER_CALLBACK 1001

int php_sdl_timer_call_run(Uint32, php_sdl_timer_t *);
PHP_MINIT_FUNCTION(SDL_Timer);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
