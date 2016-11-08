#ifndef HAVE_PHP_SDL_EXCEPTIONS
#define HAVE_PHP_SDL_EXCEPTIONS

#include "php.h"
#include "ext/spl/spl_exceptions.h"

#include <classes/common.h>

zend_class_entry *sdlRuntimeException_ce;

PHP_MINIT_FUNCTION(SDL_Exceptions)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "RuntimeException", NULL);
	sdlRuntimeException_ce = zend_register_internal_class_ex(&ce, spl_ce_RuntimeException);

	return SUCCESS;
}

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
