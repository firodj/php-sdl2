#include "php.h"
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

/* {{{ zval_to_sdl_rect */
zend_bool zval_to_sdl_rect(zval *value, SDL_Rect *rect TSRMLS_DC)
{
	if (Z_TYPE_P(value) == IS_OBJECT && Z_OBJCE_P(value) == sdlRect_ce) {
		zval *val, rv;

		val = zend_read_property(sdlRect_ce, value, "x", 1, 0, &rv TSRMLS_CC);
		convert_to_long(val);
		Z_LVAL_P(val) = rect->x = (int)Z_LVAL_P(val);

		val = zend_read_property(sdlRect_ce, value, "y", 1, 0, &rv TSRMLS_CC);
		convert_to_long(val);
		Z_LVAL_P(val) = rect->y = (int)Z_LVAL_P(val);

		val = zend_read_property(sdlRect_ce, value, "w", 1, 0, &rv TSRMLS_CC);
		convert_to_long(val);
		Z_LVAL_P(val) = rect->w = (int)Z_LVAL_P(val);

		val = zend_read_property(sdlRect_ce, value, "h", 1, 0, &rv TSRMLS_CC);
		convert_to_long(val);
		Z_LVAL_P(val) = rect->h = (int)Z_LVAL_P(val);

		return 1;
	}
	/* create an empty rect */
	memset(rect, 0, sizeof(SDL_Rect));
	return 0;
} /* }}} */

/* {{{ sdl_rect_to_zval */
zend_bool sdl_rect_to_zval(SDL_Rect *rect, zval *value TSRMLS_DC)
{
	if (rect) {
		object_init_ex(value, sdlRect_ce);
		zend_update_property_long(sdlRect_ce, value, "x", 1, rect->x TSRMLS_CC);
		zend_update_property_long(sdlRect_ce, value, "y", 1, rect->y TSRMLS_CC);
		zend_update_property_long(sdlRect_ce, value, "w", 1, rect->w TSRMLS_CC);
		zend_update_property_long(sdlRect_ce, value, "h", 1, rect->h TSRMLS_CC);

		return 1;
	}
	ZVAL_NULL(value);
	return 0;
} /* }}} */

/* {{{ proto Rect Rect::__construct(int x, int y, int w, int h) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_rect___construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, w, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, h, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Rect, __construct)
{
	zend_long x;
	zend_long y;
	zend_long w;
	zend_long h;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|llll", &x, &y, &w, &h) != SUCCESS) {
		return;
	}

	zend_update_property_long(sdlRect_ce, getThis(), "x", 1, x TSRMLS_CC);
	zend_update_property_long(sdlRect_ce, getThis(), "y", 1, y TSRMLS_CC);
	zend_update_property_long(sdlRect_ce, getThis(), "w", 1, w TSRMLS_CC);
	zend_update_property_long(sdlRect_ce, getThis(), "h", 1, h TSRMLS_CC);
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

	memcpy(&php_sdl_rect_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	zend_declare_property_long(sdlRect_ce, "x", 1, 0, ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_long(sdlRect_ce, "y", 1, 0, ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_long(sdlRect_ce, "w", 1, 0, ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_long(sdlRect_ce, "h", 1, 0, ZEND_ACC_PUBLIC TSRMLS_CC);

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
