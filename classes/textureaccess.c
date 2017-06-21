#include "php.h"
#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./textureaccess.h"

zend_class_entry *sdlTextureAccess_ce;

PHP_MINIT_FUNCTION(SDL_TextureAccess) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "TextureAccess", NULL);
	sdlTextureAccess_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlTextureAccess_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_TEXTUREACCESS_CONST(STATIC);
	SDL_TEXTUREACCESS_CONST(STREAMING);
	SDL_TEXTUREACCESS_CONST(TARGET);

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
