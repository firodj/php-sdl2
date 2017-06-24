#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./rect.h"

static zend_object_handlers php_sdl_rect_handlers;

zend_class_entry *sdlRect_ce;

/* {{{ php_sdl_rect_create */
static zend_object* php_sdl_rect_create(zend_class_entry *ce)
{
	php_sdl_rect_t *rt = 
		(php_sdl_rect_t*) ecalloc(1, sizeof(php_sdl_rect_t) + zend_object_properties_size(ce));

	zend_object_std_init(&rt->std, ce);
	object_properties_init(&rt->std, ce);
	rt->std.handlers = &php_sdl_rect_handlers;

	return &rt->std;
} /* }}} */

/* {{{ php_sdl_rect_dtor_storage */
static void php_sdl_rect_dtor_storage(zend_object *object)
{
	php_sdl_rect_t *rt = php_sdl_rect_from_obj(object);
} /* }}} */

/* {{{ php_sdl_rect_free_storage */
static void php_sdl_rect_free_storage(zend_object *object)
{
	php_sdl_rect_t *rt = php_sdl_rect_from_obj(object);

	zend_object_std_dtor(&rt->std);
	efree(rt);
} /* }}} */

/* {{{ php_sdl_rect_read_property */
zval* php_sdl_rect_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv)
{
	zval *retval;

	php_sdl_rect_t *rt = php_sdl_rect_fetch(object);

	retval = rv;

	if (strcmp(Z_STRVAL_P(member), "x") == 0) {
		ZVAL_LONG(retval, rt->rect.x);
	} else if (strcmp(Z_STRVAL_P(member), "y") == 0) {
		ZVAL_LONG(retval, rt->rect.y);
	} else if (strcmp(Z_STRVAL_P(member), "w") == 0) {
		ZVAL_LONG(retval, rt->rect.w);
	} else if (strcmp(Z_STRVAL_P(member), "h") == 0) {
		ZVAL_LONG(retval, rt->rect.h);
	} else {
		/* didn't find any */
		retval = (zend_get_std_object_handlers())->read_property(object, member, type, cache_slot, rv);
	}

	return retval;
} /* }}} */

/* {{{ php_sdl_rect_write_property */
void php_sdl_rect_write_property(zval *object, zval *member, zval *value, void **cache_slot)
{
	php_sdl_rect_t *rt = php_sdl_rect_fetch(object);

	if (strcmp(Z_STRVAL_P(member), "x") == 0) {
		rt->rect.x = zval_get_long(value);
	} else if (strcmp(Z_STRVAL_P(member), "y") == 0) {
		rt->rect.y = zval_get_long(value);
	} else if (strcmp(Z_STRVAL_P(member), "w") == 0) {
		rt->rect.w = zval_get_long(value);
	} else if (strcmp(Z_STRVAL_P(member), "h") == 0) {
		rt->rect.h = zval_get_long(value);
	} else {
		/* didn't find any */
		(zend_get_std_object_handlers())->write_property(object, member, value, cache_slot);
	}
}
/* }}} */

/* {{{ proto Rect Rect::__construct(int x, int y, int w, int h) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_rect___construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, w, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, h, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Rect, __construct)
{
	php_sdl_rect_t *rt = php_sdl_rect_fetch(getThis());

	zend_long x;
	zend_long y;
	zend_long w;
	zend_long h;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|llll", &x, &y, &w, &h) != SUCCESS) {
		return;
	}

	rt->rect.x = x;
	rt->rect.y = y;
	rt->rect.w = w;
	rt->rect.h = h;
} /* }}} */

/* {{{ php_sdl_rect_methods */
const zend_function_entry php_sdl_rect_methods[] = {
	PHP_ME(Rect, __construct, php_sdl_rect___construct_info, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

PHP_MINIT_FUNCTION(SDL_Rect) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Rect", php_sdl_rect_methods);
	sdlRect_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlRect_ce->ce_flags |= ZEND_ACC_FINAL;
	sdlRect_ce->create_object = php_sdl_rect_create;
	sdlRect_ce->serialize = zend_class_serialize_deny;
	sdlRect_ce->unserialize = zend_class_unserialize_deny;

	memcpy(&php_sdl_rect_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_sdl_rect_handlers.free_obj = php_sdl_rect_free_storage;
	php_sdl_rect_handlers.dtor_obj = php_sdl_rect_dtor_storage;
	php_sdl_rect_handlers.clone_obj = NULL;
	php_sdl_rect_handlers.read_property = php_sdl_rect_read_property;
	php_sdl_rect_handlers.write_property = php_sdl_rect_write_property;

	php_sdl_rect_handlers.offset = XtOffsetOf(php_sdl_rect_t, std);

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
