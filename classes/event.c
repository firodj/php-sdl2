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

/* {{{ proto Event Event::__get(string key) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_event___get_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Event, __get)
{
	php_sdl_event_t *et = php_sdl_event_fetch(getThis());

	zend_string *key;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &key) != SUCCESS) {
		return;
	}

	if (strcmp(ZSTR_VAL(key), "type") == 0) {
		RETURN_LONG(et->event.type);
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
	}
} /* }}} */

/* {{{ php_sdl_event_methods */
const zend_function_entry php_sdl_event_methods[] = {
	PHP_ME(Event, __construct, php_sdl_event___construct_info, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Event, __get, php_sdl_event___get_info, ZEND_ACC_PUBLIC)
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
