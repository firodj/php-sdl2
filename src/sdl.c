#ifndef HAVE_PHP_SDL_SDL
#define HAVE_PHP_SDL_SDL

#include "php.h"

#include <SDL2/SDL.h>

#include <src/sdl.h>

/* {{{ proto bool SDL\init(int flags) */
PHP_FUNCTION(SDL_init)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
    RETVAL_BOOL(SDL_Init(flags) == 0);
}
/* }}} */

/* {{{ proto bool SDL\init_subsystem(int flags) */
PHP_FUNCTION(SDL_init_subsystem)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
    RETVAL_BOOL(SDL_Init(flags) == 0);
}
/* }}} */

/* {{{ proto void SDL\quit_subsystem(int flags) */
PHP_FUNCTION(SDL_quit_subsystem)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
    SDL_QuitSubSystem(flags);
}
/* }}} */

/* {{{ proto int SDL\was_init(int flags) */
PHP_FUNCTION(SDL_was_init)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
    RETVAL_LONG(SDL_WasInit(flags));
}
/* }}} */

/* {{{ proto void SDL\quit() */
PHP_FUNCTION(SDL_quit)
{
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "") != SUCCESS) {
		return;
	}
	
    SDL_Quit();
}
/* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(SDL_SDL) 
{
    SDL_LONG_CONSTANT(INIT_TIMER);
    SDL_LONG_CONSTANT(INIT_AUDIO);
    SDL_LONG_CONSTANT(INIT_VIDEO);
    SDL_LONG_CONSTANT(INIT_JOYSTICK);
    SDL_LONG_CONSTANT(INIT_HAPTIC);
    SDL_LONG_CONSTANT(INIT_GAMECONTROLLER);
    SDL_LONG_CONSTANT(INIT_EVENTS);
    SDL_LONG_CONSTANT(INIT_NOPARACHUTE);
    SDL_LONG_CONSTANT(INIT_EVERYTHING);

	return SUCCESS;
}
/* }}} */

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
