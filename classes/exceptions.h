#ifndef HAVE_PHP_SDL_EXCEPTIONS_H
#define HAVE_PHP_SDL_EXCEPTIONS_H

#include "zend_exceptions.h"

extern zend_class_entry *sdlError_ce;

#define php_sdl_error(message, ...) zend_throw_exception_ex(sdlError_ce, 0, message, ##__VA_ARGS__)

PHP_MINIT_FUNCTION(SDL_Exceptions);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
