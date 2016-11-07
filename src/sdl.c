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
PHP_FUNCTION(SDL_initSubSystem)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
    RETVAL_BOOL(SDL_Init(flags) == 0);
}
/* }}} */

/* {{{ proto void SDL\quit_subsystem(int flags) */
PHP_FUNCTION(SDL_quitSubSystem)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
    SDL_QuitSubSystem(flags);
}
/* }}} */

/* {{{ proto int SDL\was_init(int flags) */
PHP_FUNCTION(SDL_wasInit)
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
    SDL_CONST_LONG(INIT_TIMER);
    SDL_CONST_LONG(INIT_AUDIO);
    SDL_CONST_LONG(INIT_VIDEO);
    SDL_CONST_LONG(INIT_JOYSTICK);
    SDL_CONST_LONG(INIT_HAPTIC);
    SDL_CONST_LONG(INIT_GAMECONTROLLER);
    SDL_CONST_LONG(INIT_EVENTS);
    SDL_CONST_LONG(INIT_NOPARACHUTE);
    SDL_CONST_LONG(INIT_EVERYTHING);

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
