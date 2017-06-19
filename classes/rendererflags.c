#ifndef HAVE_PHP_SDL_RENDERERFLAGS
#define HAVE_PHP_SDL_RENDERERFLAGS

#include "php.h"
#include <SDL2/SDL.h>

#include <classes/common.h>
#include <classes/rendererflags.h>

zend_class_entry *sdlRendererFlags_ce;

PHP_MINIT_FUNCTION(SDL_RendererFlags) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "RendererFlags", NULL);
	sdlRendererFlags_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlRendererFlags_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_RENDERERFLAGS_CONST(SOFTWARE);
	SDL_RENDERERFLAGS_CONST(ACCELERATED);
	SDL_RENDERERFLAGS_CONST(PRESENTVSYNC);
	SDL_RENDERERFLAGS_CONST(TARGETTEXTURE);

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
