#ifndef HAVE_PHP_SDL_SDL
#define HAVE_PHP_SDL_SDL

#include "php.h"

#include <SDL2/SDL.h>

#include <src/exceptions.h>
#include <src/sdl.h>

/* {{{ proto void SDL\init(int flags) */
SDL_FUNC(init)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
	if (SDL_Init(flags) != 0) {
		php_sdl_exception(SDL_GetError());
	}
}
/* }}} */

/* {{{ proto void SDL\initSubSystem(int flags) */
SDL_FUNC(initSubSystem)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
	if (SDL_InitSubSystem(flags) != 0) {
		php_sdl_exception(SDL_GetError());
	}
}
/* }}} */

/* {{{ proto void SDL\quitSubSystem(int flags) */
SDL_FUNC(quitSubSystem)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
    SDL_QuitSubSystem(flags);
}
/* }}} */

/* {{{ proto int SDL\wasInit(int flags) */
SDL_FUNC(wasInit)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
    RETVAL_LONG(SDL_WasInit(flags));
}
/* }}} */

/* {{{ proto void SDL\quit() */
SDL_FUNC(quit)
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
