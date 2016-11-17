#ifndef HAVE_PHP_SDL_WINDOWFLAGS
#define HAVE_PHP_SDL_WINDOWFLAGS

#include "php.h"
#include <SDL2/SDL.h>

#include <classes/common.h>
#include <classes/windowpos.h>

zend_class_entry *sdlWindowPos_ce;

PHP_MINIT_FUNCTION(SDL_WindowPos) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "WindowPos", NULL);
	sdlWindowPos_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlWindowPos_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_WINDOWPOS_CONST(UNDEFINED);
	SDL_WINDOWPOS_CONST(CENTERED);

	return SUCCESS;
} /* }}} */

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
