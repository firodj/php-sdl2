#ifndef HAVE_PHP_SDL_SDL_H
#define HAVE_PHP_SDL_SDL_H

#include <src/common.h>

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_sdl_init_info, 0, 1, _IS_BOOL, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_sdl_initSubSystem_info, 0, 1, _IS_BOOL, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_sdl_quitSubSystem_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_sdl_wasInit_info, 0, 1, IS_LONG, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_sdl_quit_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(SDL_init);
PHP_FUNCTION(SDL_initSubSystem);
PHP_FUNCTION(SDL_quitSubSystem);
PHP_FUNCTION(SDL_wasInit);
PHP_FUNCTION(SDL_quit);

#define SDL_SDL_FE \
    SDL_FE(init) \
    SDL_FE(initSubSystem) \
    SDL_FE(quitSubSystem) \
    SDL_FE(wasInit) \
    SDL_FE(quit)

PHP_MINIT_FUNCTION(SDL_SDL);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
