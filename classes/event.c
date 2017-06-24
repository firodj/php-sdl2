#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./event.h"
#include "./timer.h"

static zend_object_handlers php_sdl_event_handlers;

zend_class_entry *sdlEvent_ce;

/* {{{ php_sdl_event_create */
static zend_object* php_sdl_event_create(zend_class_entry *ce)
{
	php_sdl_event_t *et = 
		(php_sdl_event_t*) ecalloc(1, sizeof(php_sdl_event_t) + zend_object_properties_size(ce));

	zend_object_std_init(&et->std, ce);
	object_properties_init(&et->std, ce);
	et->std.handlers = &php_sdl_event_handlers;

	return &et->std;
} /* }}} */

/* {{{ php_sdl_event_dtor_storage */
static void php_sdl_event_dtor_storage(zend_object *object)
{
	php_sdl_event_t *et = php_sdl_event_from_obj(object);

	//DESTROY(et->event);
} /* }}} */

/* {{{ php_sdl_event_free_storage */
static void php_sdl_event_free_storage(zend_object *object)
{
	php_sdl_event_t *et = php_sdl_event_from_obj(object);

	zend_object_std_dtor(&et->std);
	efree(et);
} /* }}} */

/* {{{ php_sdl_event_read_property */
zval* php_sdl_event_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv)
{
	zval *retval;

	php_sdl_event_t *et = php_sdl_event_fetch(object);

	retval = rv;

	if (strcmp(Z_STRVAL_P(member), "type") == 0) {
		ZVAL_LONG(retval, et->event.type);
	}

	return retval;
} /* }}} */

/* {{{ proto Event Event::__construct() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_event___construct_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Event, __construct)
{
	php_sdl_event_t *et = php_sdl_event_fetch(getThis());

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "") != SUCCESS) {
		return;
	}
} /* }}} */

/* {{{ proto Event Event::poll() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_event_poll_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Event, poll)
{
	php_sdl_event_t *et;
	SDL_Event event;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "") != SUCCESS) {
		RETURN_FALSE;
	}

	if (SDL_PollEvent(&event)) {
		object_init_ex(return_value, sdlEvent_ce);
		et = php_sdl_event_fetch(return_value);
		et->event = event;

		if (event.type == SDL_USEREVENT && event.user.code == SDL_TIMER_CALLBACK) {
			Uint32 ticks = (Uint32)event.user.data1;
			php_sdl_timer_t *tmt = (php_sdl_timer_t*)event.user.data2;
			php_sdl_timer_call_run(ticks, tmt);
		}
		return;
	}

	RETURN_NULL();
} /* }}} */

/* {{{ php_sdl_event_methods */
const zend_function_entry php_sdl_event_methods[] = {
	PHP_ME(Event, __construct, php_sdl_event___construct_info, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Event, poll, php_sdl_event_poll_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

PHP_MINIT_FUNCTION(SDL_Event) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Event", php_sdl_event_methods);
	sdlEvent_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlEvent_ce->ce_flags |= ZEND_ACC_FINAL;
	sdlEvent_ce->create_object = php_sdl_event_create;
	sdlEvent_ce->serialize = zend_class_serialize_deny;
	sdlEvent_ce->unserialize = zend_class_unserialize_deny;

	memcpy(&php_sdl_event_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_sdl_event_handlers.free_obj = php_sdl_event_free_storage;
	php_sdl_event_handlers.dtor_obj = php_sdl_event_dtor_storage;
	php_sdl_event_handlers.clone_obj = NULL;
	php_sdl_event_handlers.read_property = php_sdl_event_read_property;

	php_sdl_event_handlers.offset = XtOffsetOf(php_sdl_event_t, std);

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
