#include "php.h"
#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./pixelformat.h"

zend_class_entry *sdlPixelFormat_ce;

PHP_MINIT_FUNCTION(SDL_PixelFormat) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "PixelFormat", NULL);
	sdlPixelFormat_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlPixelFormat_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_PIXELFORMAT_CONST(ARGB8888);
	SDL_PIXELFORMAT_CONST(RGBA32);

	return SUCCESS;
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
