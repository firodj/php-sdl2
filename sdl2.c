#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_sdl2.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#ifdef HAVE_SDL2IMG
#ifdef _WIN32
	#include <SDL_image.h>
#else
	#include <SDL2/SDL_image.h>
#endif
#endif

#include "classes/common.h"
#include "classes/const.h"
#include "classes/exceptions.h"
#include "classes/event.h"
#include "classes/point.h"
#include "classes/rect.h"
#include "classes/renderer.h"
#include "classes/surface.h"
#include "classes/texture.h"
#include "classes/timer.h"
#include "classes/window.h"

/* {{{ proto void SDL\init(int flags) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_init_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

SDL_FUNC(init)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
	if (SDL_Init(flags) != 0) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

#ifdef HAVE_SDL2IMG
/* {{{ proto void SDL\initIMG(int flags) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_initIMG_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

SDL_FUNC(initIMG)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
	if (IMG_Init(flags) != flags) {
		php_sdl_error(IMG_GetError());
	}
} /* }}} */
#endif

/* {{{ proto void SDL\initSubSystem(int flags) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_initSubSystem_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

SDL_FUNC(initSubSystem)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
	if (SDL_InitSubSystem(flags) != 0) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto void SDL\quitSubSystem(int flags) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_quitSubSystem_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

SDL_FUNC(quitSubSystem)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
	SDL_QuitSubSystem(flags);
} /* }}} */

/* {{{ proto int SDL\wasInit(int flags) */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_sdl_wasInit_info, 0, 1, IS_LONG, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

SDL_FUNC(wasInit)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &flags) != SUCCESS) {
		return;
	}
	
	RETVAL_LONG(SDL_WasInit(flags));
} /* }}} */

/* {{{ proto void SDL\quit() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_quit_info, 0, 0, 0)
ZEND_END_ARG_INFO()

SDL_FUNC(quit)
{
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "") != SUCCESS) {
		return;
	}
	
	SDL_Quit();
} /* }}} */

#ifdef HAVE_SDL2IMG
/* {{{ proto void SDL\quitIMG() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_quitIMG_info, 0, 0, 0)
ZEND_END_ARG_INFO()

SDL_FUNC(quitIMG)
{
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "") != SUCCESS) {
		return;
	}
	
	IMG_Quit();
} /* }}} */
#endif

PHP_MINIT_FUNCTION(sdl2) /* {{{ */
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

	SDL_CONST_LONG(MAJOR_VERSION);
	SDL_CONST_LONG(MINOR_VERSION);
	SDL_CONST_LONG(PATCHLEVEL);
	SDL_CONST_LONG(COMPILEDVERSION);

#ifdef HAVE_SDL2IMG
	IMG_CONST_LONG(INIT_PNG);
	IMG_CONST_LONG(INIT_JPG);
#endif

	PHP_MINIT(SDL_Const)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(SDL_Exceptions)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(SDL_Event)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(SDL_Point)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(SDL_Rect)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(SDL_Renderer)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(SDL_Surface)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(SDL_Texture)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(SDL_Timer)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(SDL_Window)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
} /* }}} */

PHP_RINIT_FUNCTION(sdl2) /* {{{ */
{
#if defined(COMPILE_DL_SDL2) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
} /* }}} */

/* {{{ sdl2_functions[] */
const zend_function_entry sdl2_functions[] = {
	SDL_FE(init)
#ifdef HAVE_SDL2IMG
	SDL_FE(initIMG)
#endif
	SDL_FE(initSubSystem)
	SDL_FE(quitSubSystem)
	SDL_FE(wasInit)
	SDL_FE(quit)
#ifdef HAVE_SDL2IMG
	SDL_FE(quitIMG)
#endif
	PHP_FE_END
}; /* }}} */

/* {{{ sdl2_module_entry */
zend_module_entry sdl2_module_entry = {
	STANDARD_MODULE_HEADER,
	"SDL2",
	sdl2_functions,
	PHP_MINIT(sdl2),
	NULL,
	PHP_RINIT(sdl2),
	NULL,
	NULL,
	PHP_SDL2_VERSION,
	STANDARD_MODULE_PROPERTIES
}; /* }}} */

#ifdef COMPILE_DL_SDL2
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(sdl2)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
