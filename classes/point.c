#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./point.h"

static zend_object_handlers php_sdl_point_handlers;

zend_class_entry *sdlPoint_ce;

/* {{{ php_sdl_point_create */
static zend_object* php_sdl_point_create(zend_class_entry *ce)
{
	php_sdl_point_t *pt = 
		(php_sdl_point_t*) ecalloc(1, sizeof(php_sdl_point_t) + zend_object_properties_size(ce));

	zend_object_std_init(&pt->std, ce);
	object_properties_init(&pt->std, ce);
	pt->std.handlers = &php_sdl_point_handlers;

	return &pt->std;
} /* }}} */

/* {{{ php_sdl_point_dtor_storage */
static void php_sdl_point_dtor_storage(zend_object *object)
{
	php_sdl_point_t *pt = php_sdl_point_from_obj(object);
} /* }}} */

/* {{{ php_sdl_point_free_storage */
static void php_sdl_point_free_storage(zend_object *object)
{
	php_sdl_point_t *pt = php_sdl_point_from_obj(object);

	zend_object_std_dtor(&pt->std);
	efree(pt);
} /* }}} */

/* {{{ php_sdl_point_read_property */
zval* php_sdl_point_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv)
{
	zval *retval;

	php_sdl_point_t *pt = php_sdl_point_fetch(object);

	retval = rv;

	if (strcmp(Z_STRVAL_P(member), "x") == 0) {
		ZVAL_LONG(retval, pt->point.x);
	} else if (strcmp(Z_STRVAL_P(member), "y") == 0) {
		ZVAL_LONG(retval, pt->point.y);
	} else {
		/* didn't find any */
		retval = (zend_get_std_object_handlers())->read_property(object, member, type, cache_slot, rv);
	}

	return retval;
} /* }}} */

/* {{{ php_sdl_point_write_property */
void php_sdl_point_write_property(zval *object, zval *member, zval *value, void **cache_slot)
{
	php_sdl_point_t *pt = php_sdl_point_fetch(object);

	if (strcmp(Z_STRVAL_P(member), "x") == 0) {
		pt->point.x = zval_get_long(value);
	} else if (strcmp(Z_STRVAL_P(member), "y") == 0) {
		pt->point.y = zval_get_long(value);
	} else {
		/* didn't find any */
		(zend_get_std_object_handlers())->write_property(object, member, value, cache_slot);
	}
}
/* }}} */

/* {{{ proto Point Point::__construct(string text, int number) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_point___construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Point, __construct)
{
	php_sdl_point_t *pt = php_sdl_point_fetch(getThis());

	zend_long x;
	zend_long y;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|ll", &x, &y) != SUCCESS) {
		return;
	}

	pt->point.x = x;
	pt->point.y = y;
} /* }}} */

/* {{{ php_sdl_point_methods */
const zend_function_entry php_sdl_point_methods[] = {
	PHP_ME(Point, __construct, php_sdl_point___construct_info, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

PHP_MINIT_FUNCTION(SDL_Point) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Point", php_sdl_point_methods);
	sdlPoint_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlPoint_ce->ce_flags |= ZEND_ACC_FINAL;
	sdlPoint_ce->create_object = php_sdl_point_create;
	sdlPoint_ce->serialize = zend_class_serialize_deny;
	sdlPoint_ce->unserialize = zend_class_unserialize_deny;

	memcpy(&php_sdl_point_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_sdl_point_handlers.free_obj = php_sdl_point_free_storage;
	php_sdl_point_handlers.dtor_obj = php_sdl_point_dtor_storage;
	php_sdl_point_handlers.clone_obj = NULL;
	php_sdl_point_handlers.read_property = php_sdl_point_read_property;
	php_sdl_point_handlers.write_property = php_sdl_point_write_property;

	php_sdl_point_handlers.offset = XtOffsetOf(php_sdl_point_t, std);

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
