#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./surface.h"
#include "./rect.h"

static zend_object_handlers php_sdl_surface_handlers;

zend_class_entry *sdlSurface_ce;

/* {{{ php_sdl_surface_create */
static zend_object* php_sdl_surface_create(zend_class_entry *ce)
{
  php_sdl_surface_t *st =
    (php_sdl_surface_t*) ecalloc(1, sizeof(php_sdl_surface_t) + zend_object_properties_size(ce));

  zend_object_std_init(&st->std, ce);
  object_properties_init(&st->std, ce);
  st->std.handlers = &php_sdl_surface_handlers;

  return &st->std;
} /* }}} */

/* {{{ php_sdl_surface_dtor_storage */
static void php_sdl_surface_dtor_storage(zend_object *object)
{
	php_sdl_surface_t *st = php_sdl_surface_from_obj(object);

	SDL_FreeSurface(st->surface);
} /* }}} */

/* {{{ php_sdl_surface_free_storage */
static void php_sdl_surface_free_storage(zend_object *object)
{
	php_sdl_surface_t *st = php_sdl_surface_from_obj(object);

	zend_object_std_dtor(&st->std);
	efree(st);
} /* }}} */

/* {{{ php_sdl_surface_write_property */
void php_sdl_surface_write_property(zval *object, zval *member, zval *value, void **cache_slot)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(object);

	/* didn't find any */
	(zend_get_std_object_handlers())->write_property(object, member, value, cache_slot);
}
/* }}} */

/* {{{ php_sdl_surface_read_property */
zval* php_sdl_surface_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv)
{
	zval *retval;

	php_sdl_surface_t *st = php_sdl_surface_fetch(object);

	retval = rv;

	if (strcmp(Z_STRVAL_P(member), "w") == 0) {
		ZVAL_LONG(retval, st->surface->w);
	} else if (strcmp(Z_STRVAL_P(member), "h") == 0) {
		ZVAL_LONG(retval, st->surface->h);
	} else if (strcmp(Z_STRVAL_P(member), "pitch") == 0) {
		ZVAL_LONG(retval, st->surface->pitch);
	} else if (strcmp(Z_STRVAL_P(member), "refcount") == 0) {
		ZVAL_LONG(retval, st->surface->refcount);
	} else if (strcmp(Z_STRVAL_P(member), "clip_rect") == 0) {
		object_init_ex(retval, sdlRect_ce);
		php_sdl_rect_t *rt = php_sdl_rect_fetch(retval);
		rt->rect = st->surface->clip_rect;
	} else {
		/* didn't find any */
		retval = (zend_get_std_object_handlers())->read_property(object, member, type, cache_slot, rv);
	}

	return retval;
} /* }}} */

/* {{{ proto Surface Surface::__construct(int width, int height[, int depth = 32 [, int format = SDL\PixelFormat::RGBA32]]) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface___construct_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, format, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, __construct)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());

	zend_long width;
	zend_long height;
	zend_long depth = 32;
	zend_long format = SDL_PIXELFORMAT_RGBA32;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ll|l", &width, &height, &depth, &format) != SUCCESS) {
		return;
	}

	st->surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, format);

	if (st->surface == NULL) {
		php_sdl_error(SDL_GetError());
	}
}
/* }}} */

/* {{{ proto Surface Surface::loadBMP(string filename) */
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(php_sdl_surface_loadBMP_info, 0, 1, SDL\Surface, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_sdl_surface_loadBMP_info, 0, 1, IS_OBJECT, ZEND_NS_NAME(SDL_NS, "Surface"), 0)
#endif
	ZEND_ARG_TYPE_INFO(0, file, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, loadBMP)
{
	php_sdl_surface_t *st;

	zend_string *filename;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &filename) != SUCCESS) {
		return;
	}

	object_init_ex(return_value, sdlSurface_ce);
	st = php_sdl_surface_fetch(return_value);

	st->surface = SDL_LoadBMP(ZSTR_VAL(filename));

	if (st->surface == NULL) {
		zval_ptr_dtor(return_value);
		php_sdl_error(SDL_GetError());
	}
}
/* }}} */

/* {{{ proto Surface::blitFromSurface(Surface src, Rect srcrect, Rect dstrect) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface_blitFromSurface_info, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, src, SDL\\Surface, 0)
	ZEND_ARG_OBJ_INFO(0, srcrect, SDL\\Rect, 0)
	ZEND_ARG_OBJ_INFO(0, dstrect, SDL\\Rect, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, blitFromSurface)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());

	zval *surface;
	zval *srcrect;
	zval *dstrect = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO|O", &surface, sdlSurface_ce,
			&srcrect, sdlRect_ce, &dstrect, sdlRect_ce) != SUCCESS) {
		return;
	}

	SDL_Rect *dstR = NULL;
	SDL_Rect *srcR = &php_sdl_rect_fetch(srcrect)->rect;
	SDL_Surface *srcS = php_sdl_surface_fetch(surface)->surface;
	int err;

	if (dstrect) {
		dstR = &php_sdl_rect_fetch(dstrect)->rect;
	} else dstR = srcR;

	err = SDL_BlitSurface(srcS, srcR, st->surface, dstR);

	if (err) {
		php_sdl_error(SDL_GetError());
	}
}
/* }}} */

/* {{{ proto Surface::setColorKey(bool flag, int key) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface_setColorKey_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, enable, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, setColorKey)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());

	zend_long key;
	zend_bool enable;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "bl", &enable, &key) != SUCCESS) {
		return;
	}

	int err = SDL_SetColorKey(st->surface, enable, key);
	if (err) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto int Surface::getColorKey() */
PHP_METHOD(Surface, getColorKey)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());
	Uint32 key;

	int err = SDL_GetColorKey(st->surface, &key);
	if (err == -1) RETURN_NULL();
	if (err) {
		php_sdl_error(SDL_GetError());
	}

	RETURN_LONG(key);
} /* }}} */

/* {{{ proto int Surface::mapRGB(int r, int g, int b) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface_mapRGB_info, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, r, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, mapRGB)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());

	zend_long r;
	zend_long g;
	zend_long b;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lll", &r, &g, &b) != SUCCESS) {
		return;
	}

	Uint32 key = SDL_MapRGB(st->surface->format, r, g, b);
	RETURN_LONG(key);
}
/* }}} */

/* {{{ proto int Surface::mapRGBA(int r, int g, int b, int a) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface_mapRGBA_info, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, r, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, mapRGBA)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());

	zend_long r;
	zend_long g;
	zend_long b;
	zend_long a;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "llll", &r, &g, &b, &a) != SUCCESS) {
		return;
	}

	Uint32 key = SDL_MapRGBA(st->surface->format, r, g, b, a);
	RETURN_LONG(key);
}
/* }}} */

/* {{{ proto stdObject Surface::getRGB(int pixel) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface_getRGB_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pixel, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, getRGB)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());

	zend_long pixel;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &pixel) != SUCCESS) {
		return;
	}

	Uint8 r, g, b;

	SDL_GetRGB(pixel, st->surface->format, &r, &g, &b);

	object_init(return_value);
	add_property_long(return_value, "r", r);
	add_property_long(return_value, "g", g);
	add_property_long(return_value, "b", b);
} /* }}} */

/* {{{ proto stdObject Surface::getRGBA(int pixel) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface_getRGBA_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pixel, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, getRGBA)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());

	zend_long pixel;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &pixel) != SUCCESS) {
		return;
	}

	Uint8 r, g, b, a;

	SDL_GetRGBA(pixel, st->surface->format, &r, &g, &b, &a);

	object_init(return_value);
	add_property_long(return_value, "r", r);
	add_property_long(return_value, "g", g);
	add_property_long(return_value, "b", b);
	add_property_long(return_value, "a", a);
} /* }}} */

/* {{{ proto Surface::setAlphaMod(int alpha) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface_setAlphaMod_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, alpha, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, setAlphaMod)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());

	zend_long alpha;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &alpha) != SUCCESS) {
		return;
	}

	int err = SDL_SetSurfaceAlphaMod(st->surface, alpha);

	if (err) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto Surface::setBlendMod(int alpha) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface_setBlendMode_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, blendMode, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, setBlendMode)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());

	zend_long blendMode;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &blendMode) != SUCCESS) {
		return;
	}

	int err = SDL_SetSurfaceBlendMode(st->surface, blendMode);

	if (err) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ php_sdl_surface_methods */
const zend_function_entry php_sdl_surface_methods[] = {
	PHP_ME(Surface, __construct, php_sdl_surface___construct_info, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Surface, loadBMP, php_sdl_surface_loadBMP_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Surface, blitFromSurface, php_sdl_surface_blitFromSurface_info, ZEND_ACC_PUBLIC)
	PHP_ME(Surface, setColorKey, php_sdl_surface_setColorKey_info, ZEND_ACC_PUBLIC)
	PHP_ME(Surface, getColorKey, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Surface, mapRGB, php_sdl_surface_mapRGB_info, ZEND_ACC_PUBLIC)
	PHP_ME(Surface, mapRGBA, php_sdl_surface_mapRGBA_info, ZEND_ACC_PUBLIC)
	PHP_ME(Surface, getRGB, php_sdl_surface_getRGB_info, ZEND_ACC_PUBLIC)
	PHP_ME(Surface, getRGBA, php_sdl_surface_getRGBA_info, ZEND_ACC_PUBLIC)
	PHP_ME(Surface, setAlphaMod, php_sdl_surface_setAlphaMod_info, ZEND_ACC_PUBLIC)
	PHP_ME(Surface, setBlendMode, php_sdl_surface_setBlendMode_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

PHP_MINIT_FUNCTION(SDL_Surface) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Surface", php_sdl_surface_methods);
	sdlSurface_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlSurface_ce->ce_flags |= ZEND_ACC_FINAL;
	sdlSurface_ce->create_object = php_sdl_surface_create;
	sdlSurface_ce->serialize = zend_class_serialize_deny;
	sdlSurface_ce->unserialize = zend_class_unserialize_deny;

	memcpy(&php_sdl_surface_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_sdl_surface_handlers.free_obj = php_sdl_surface_free_storage;
	php_sdl_surface_handlers.dtor_obj = php_sdl_surface_dtor_storage;
	php_sdl_surface_handlers.clone_obj = NULL;
	php_sdl_surface_handlers.read_property = php_sdl_surface_read_property;
	php_sdl_surface_handlers.write_property = php_sdl_surface_write_property;

	php_sdl_surface_handlers.offset = XtOffsetOf(php_sdl_surface_t, std);

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
