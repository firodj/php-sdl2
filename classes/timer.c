#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./timer.h"

static zend_object_handlers php_sdl_timer_handlers;

zend_class_entry *sdlTimer_ce;

struct _php_sdl_globals_t php_sdl_globals;

/* {{{ php_sdl_timer_create */
static zend_object* php_sdl_timer_create(zend_class_entry *ce)
{
	php_sdl_timer_t *tmt = 
		(php_sdl_timer_t*) ecalloc(1, sizeof(php_sdl_timer_t) + zend_object_properties_size(ce));

	zend_object_std_init(&tmt->std, ce);
	object_properties_init(&tmt->std, ce);
	tmt->std.handlers = &php_sdl_timer_handlers;

	return &tmt->std;
} /* }}} */

static int _php_sdl_timer_stop(php_sdl_timer_t *tmt)
{
	int ret = -1;
	if (tmt->id) {
		ret = SDL_RemoveTimer(tmt->id);
		tmt->id = 0;
	}
	return ret;
}

/* {{{ php_sdl_timer_dtor_storage */
static void php_sdl_timer_dtor_storage(zend_object *object)
{
	php_sdl_timer_t *tmt = php_sdl_timer_from_obj(object);
	_php_sdl_timer_stop(tmt);
} /* }}} */

/* {{{ php_sdl_timer_free_storage */
static void php_sdl_timer_free_storage(zend_object *object)
{
	php_sdl_timer_t *tmt = php_sdl_timer_from_obj(object);

	zend_object_std_dtor(&tmt->std);
	efree(tmt);
} /* }}} */

/* {{{ _php_sdl_timer_callback */
Uint32 _php_sdl_timer_callback(Uint32 interval, void *param)
{
	int retval = interval;
	php_sdl_timer_t *tmt = (php_sdl_timer_t*)param;
	fprintf(stderr, "!");
	return retval;
}
/* }}} */

/* {{{ proto Timer Timer::__construct() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_timer___construct_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Timer, __construct)
{
	php_sdl_timer_t *tmt = php_sdl_timer_fetch(getThis());

	zend_string *text;
	zend_long number;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "", &text, &number) != SUCCESS) {
		return;
	}

	tmt->id = 0;
} /* }}} */

/* {{{ proto void Timer::delay(int ms) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_timer_delay_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, ms, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Timer, delay)
{
	zend_long ms;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &ms) != SUCCESS) {
		return;
	}
	
	SDL_Delay(ms);
}
/* }}} */

/* {{{ proto int Timer::getTicks() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_timer_getTicks_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Timer, getTicks)
{
	zend_long ms;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "") != SUCCESS) {
		return;
	}

	ms = SDL_GetTicks();
	RETURN_LONG(ms);
}
/* }}} */

/* {{{ proto Timer::run() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_timer_run_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Timer, run) {} /* }}} */

/* {{{ proto Timer::start(int interval) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_timer_start_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, interval, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Timer, start)
{
	zend_long interval;
	php_sdl_timer_t *tmt = php_sdl_timer_fetch(getThis());

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &interval) != SUCCESS) {
		return;
	}

	if (tmt->id) {
		php_error_docref(NULL, E_WARNING, "Timer already started");
		RETURN_FALSE;
	}

	tmt->id = SDL_AddTimer(interval, _php_sdl_timer_callback, (void*)tmt);

	if (tmt->id == 0) {
		php_sdl_error(SDL_GetError());
		RETURN_NULL();
	}

	RETURN_TRUE;
} /* }}} */

/* {{{ proto Timer::stop() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_timer_stop_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Timer, stop) {
	php_sdl_timer_t *tmt = php_sdl_timer_fetch(getThis());

	if (tmt->id == 0) {
		php_error_docref(NULL, E_WARNING, "Timer is not started");
		RETURN_FALSE;
	}

	int ret = _php_sdl_timer_stop(tmt);
	if (ret == 0) {
		RETURN_FALSE;
	}
	else if (ret == 1) RETURN_TRUE;
} /* }}} */

/* {{{ php_sdl_timer_methods */
const zend_function_entry php_sdl_timer_methods[] = {
	PHP_ME(Timer, __construct, php_sdl_timer___construct_info, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Timer, delay, php_sdl_timer_delay_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Timer, getTicks, php_sdl_timer_getTicks_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Timer, start, php_sdl_timer_start_info, ZEND_ACC_PUBLIC)
	PHP_ME(Timer, run, php_sdl_timer_run_info, ZEND_ACC_PUBLIC)
	PHP_ME(Timer, stop, php_sdl_timer_stop_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

PHP_MINIT_FUNCTION(SDL_Timer) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Timer", php_sdl_timer_methods);
	sdlTimer_ce = zend_register_internal_class_ex(&ce, NULL);
	//sdlTimer_ce->ce_flags |= ZEND_ACC_FINAL;
	sdlTimer_ce->create_object = php_sdl_timer_create;
	sdlTimer_ce->serialize = zend_class_serialize_deny;
	sdlTimer_ce->unserialize = zend_class_unserialize_deny;

	memcpy(&php_sdl_timer_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_sdl_timer_handlers.free_obj = php_sdl_timer_free_storage;
	php_sdl_timer_handlers.dtor_obj = php_sdl_timer_dtor_storage;
	php_sdl_timer_handlers.clone_obj = NULL;

	php_sdl_timer_handlers.offset = XtOffsetOf(php_sdl_timer_t, std);

	SDL_G(strings).run = zend_string_init(ZEND_STRL("run"), 1);
	GC_REFCOUNT(SDL_G(strings).run)++;

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
