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
	//zend_objects_destroy_object(object);
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
	php_sdl_event_t *et = php_sdl_event_fetch(object);

	zval *retval = rv;

	ZVAL_NULL(retval);

	if (strcmp(Z_STRVAL_P(member), "type") == 0) {
		ZVAL_LONG(retval, et->event.type);
	} else if (strcmp(Z_STRVAL_P(member), "timestamp") == 0) {
		ZVAL_LONG(retval, et->event.common.timestamp);
	} else {
		switch (et->event.type) {
			case SDL_KEYUP:
			case SDL_KEYDOWN:
				if (strcmp(Z_STRVAL_P(member), "windowID") == 0) {
					ZVAL_LONG(retval, et->event.key.windowID);
				} else if (strcmp(Z_STRVAL_P(member), "state") == 0) {
					ZVAL_LONG(retval, et->event.key.state);
				} else if (strcmp(Z_STRVAL_P(member), "repeat") == 0) {
					ZVAL_LONG(retval, et->event.key.repeat);
				} else if (strcmp(Z_STRVAL_P(member), "scancode") == 0) {
					ZVAL_LONG(retval, et->event.key.keysym.scancode);
				} else if (strcmp(Z_STRVAL_P(member), "keycode") == 0) {
					ZVAL_LONG(retval, et->event.key.keysym.sym);
				} else if (strcmp(Z_STRVAL_P(member), "mod") == 0) {
					ZVAL_LONG(retval, et->event.key.keysym.mod);
				}
				break;
			case SDL_WINDOWEVENT:
				if (strcmp(Z_STRVAL_P(member), "windowID") == 0) {
					ZVAL_LONG(retval, et->event.window.windowID);
				} else if (strcmp(Z_STRVAL_P(member), "event") == 0) {
					ZVAL_LONG(retval, et->event.window.event);
				} else if (strcmp(Z_STRVAL_P(member), "data1") == 0) {
					ZVAL_LONG(retval, et->event.window.data1);
				} else if (strcmp(Z_STRVAL_P(member), "data2") == 0) {
					ZVAL_LONG(retval, et->event.window.data2);
				}
				break;
			case SDL_MOUSEMOTION:
				if (strcmp(Z_STRVAL_P(member), "windowID") == 0) {
					ZVAL_LONG(retval, et->event.motion.windowID);
				} else if (strcmp(Z_STRVAL_P(member), "which") == 0) {
					ZVAL_LONG(retval, et->event.motion.which);
				} else if (strcmp(Z_STRVAL_P(member), "state") == 0) {
					ZVAL_LONG(retval, et->event.motion.state);
				} else if (strcmp(Z_STRVAL_P(member), "x") == 0) {
					ZVAL_LONG(retval, et->event.motion.x);
				} else if (strcmp(Z_STRVAL_P(member), "y") == 0) {
					ZVAL_LONG(retval, et->event.motion.y);
				} else if (strcmp(Z_STRVAL_P(member), "xrel") == 0) {
					ZVAL_LONG(retval, et->event.motion.xrel);
				} else if (strcmp(Z_STRVAL_P(member), "yrel") == 0) {
					ZVAL_LONG(retval, et->event.motion.yrel);
				}
				break;
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				if (strcmp(Z_STRVAL_P(member), "windowID") == 0) {
					ZVAL_LONG(retval, et->event.button.windowID);
				} else if (strcmp(Z_STRVAL_P(member), "which") == 0) {
					ZVAL_LONG(retval, et->event.button.which);
				} else if (strcmp(Z_STRVAL_P(member), "button") == 0) {
					ZVAL_LONG(retval, et->event.button.button);
				} else if (strcmp(Z_STRVAL_P(member), "state") == 0) {
					ZVAL_LONG(retval, et->event.button.state);
				} else if (strcmp(Z_STRVAL_P(member), "clicks") == 0) {
					ZVAL_LONG(retval, et->event.button.clicks);
				} else if (strcmp(Z_STRVAL_P(member), "x") == 0) {
					ZVAL_LONG(retval, et->event.button.x);
				} else if (strcmp(Z_STRVAL_P(member), "y") == 0) {
					ZVAL_LONG(retval, et->event.button.y);
				}
				break;
			case SDL_MOUSEWHEEL:
				if (strcmp(Z_STRVAL_P(member), "windowID") == 0) {
					ZVAL_LONG(retval, et->event.wheel.windowID);
				} else if (strcmp(Z_STRVAL_P(member), "which") == 0) {
					ZVAL_LONG(retval, et->event.wheel.which);
				} else if (strcmp(Z_STRVAL_P(member), "x") == 0) {
					ZVAL_LONG(retval, et->event.wheel.x);
				} else if (strcmp(Z_STRVAL_P(member), "y") == 0) {
					ZVAL_LONG(retval, et->event.wheel.y);
				} else if (strcmp(Z_STRVAL_P(member), "direction") == 0) {
					ZVAL_LONG(retval, et->event.wheel.direction);
				}
				break;
			case SDL_FINGERMOTION:
			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
				if (strcmp(Z_STRVAL_P(member), "touchID") == 0) {
					ZVAL_LONG(retval, et->event.tfinger.touchId);
				} else if (strcmp(Z_STRVAL_P(member), "fingerID") == 0) {
					ZVAL_LONG(retval, et->event.tfinger.fingerId);
				} else if (strcmp(Z_STRVAL_P(member), "x") == 0) {
					ZVAL_DOUBLE(retval, et->event.tfinger.x);
				} else if (strcmp(Z_STRVAL_P(member), "y") == 0) {
					ZVAL_DOUBLE(retval, et->event.tfinger.y);
				} else if (strcmp(Z_STRVAL_P(member), "dx") == 0) {
					ZVAL_DOUBLE(retval, et->event.tfinger.dx);
				} else if (strcmp(Z_STRVAL_P(member), "dy") == 0) {
					ZVAL_DOUBLE(retval, et->event.tfinger.dy);
				} else if (strcmp(Z_STRVAL_P(member), "pressure") == 0) {
					ZVAL_DOUBLE(retval, et->event.tfinger.pressure);
				}
				break;
			default:
				if (et->event.type == SDL_TIMEREVENT) {
					php_sdl_timer_t *tmt = (php_sdl_timer_t*)et->event.user.data2;

					if (strcmp(Z_STRVAL_P(member), "interval") == 0) {
						Uint32 interval = (Uint32)et->event.user.data1;
						ZVAL_LONG(retval, interval);
					} else if (strcmp(Z_STRVAL_P(member), "timer") == 0) {
						ZVAL_OBJ(retval, &tmt->std);
						GC_REFCOUNT(&tmt->std)++;
					}
				}
				break;
		}
	}

	return retval;
} /* }}} */

/* {{{ proto Event Event::__construct(int type) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_event___construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 1)
ZEND_END_ARG_INFO()

PHP_METHOD(Event, __construct)
{
	php_sdl_event_t *et = php_sdl_event_fetch(getThis());
	zend_long type = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &type) != SUCCESS) {
		return;
	}

	et->event.type = type;
} /* }}} */

/* {{{ proto static void Event::setState(int type, bool enable) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_event_setState_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, enable, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Event, setState)
{
	zend_long type;
	zend_bool enable;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lb", &type, &enable) != SUCCESS) {
		return;
	}

	SDL_EventState(type, enable);
} /* }}} */

/* {{{ proto static bool Event::getState(int type) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_event_getState_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Event, getState)
{
	zend_long type;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &type) != SUCCESS) {
		return;
	}

	RETURN_BOOL(SDL_EventState(type, SDL_QUERY));
} /* }}} */

/* {{{ proto static Event Event::poll() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_event_poll_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Event, poll)
{
	SDL_Event event;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "") != SUCCESS) {
		RETURN_FALSE;
	}

	if (SDL_PollEvent(&event)) {
		object_init_ex(return_value, sdlEvent_ce);
		php_sdl_event_t *et = php_sdl_event_fetch(return_value);
		et->event = event;
		return;
	}

	RETURN_NULL();
} /* }}} */

/* {{{ proto static Event Event::push() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_event_push_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, SDL\\Event, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Event, push)
{
	SDL_Event event;
	zval *zevent;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &zevent, sdlEvent_ce) != SUCCESS) {
		RETURN_FALSE;
	}

	php_sdl_event_t *et = php_sdl_event_fetch(zevent);

	int ok = SDL_PushEvent(&et->event);
	if (ok == -1) {
		php_sdl_error(SDL_GetError());
	}
	RETURN_BOOL(ok);
} /* }}} */

/* {{{ php_sdl_event_methods */
const zend_function_entry php_sdl_event_methods[] = {
	PHP_ME(Event, __construct, php_sdl_event___construct_info, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Event, poll, php_sdl_event_poll_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Event, push, php_sdl_event_push_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Event, setState, php_sdl_event_setState_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Event, getState, php_sdl_event_getState_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
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
