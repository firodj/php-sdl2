#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./eventtype.h"

zend_class_entry *sdlEventType_ce;

PHP_MINIT_FUNCTION(SDL_EventType) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "EventType", NULL);
	sdlEventType_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlEventType_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_EVENTTYPE_CONST(KEYUP);
	SDL_EVENTTYPE_CONST(KEYDOWN);
	SDL_EVENTTYPE_CONST(QUIT);
	SDL_EVENTTYPE_CONST(MOUSEMOTION);
	SDL_EVENTTYPE_CONST(MOUSEBUTTONDOWN);
	SDL_EVENTTYPE_CONST(MOUSEBUTTONUP);
	SDL_EVENTTYPE_CONST(MOUSEWHEEL);
	SDL_EVENTTYPE_CONST(FINGERMOTION);
	SDL_EVENTTYPE_CONST(FINGERDOWN);
	SDL_EVENTTYPE_CONST(FINGERUP);
	

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
