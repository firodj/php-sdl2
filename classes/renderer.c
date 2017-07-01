#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./renderer.h"
#include "./texture.h"
#include "./window.h"
#include "./rect.h"
#include "./point.h"

static zend_object_handlers php_sdl_renderer_handlers;

zend_class_entry *sdlRenderer_ce;

 /* {{{ php_sdl_renderer_create */
static zend_object* php_sdl_renderer_create(zend_class_entry *ce)
{
	php_sdl_renderer_t *rt = 
		(php_sdl_renderer_t*) ecalloc(1, sizeof(php_sdl_renderer_t) + zend_object_properties_size(ce));

	zend_object_std_init(&rt->std, ce);
	object_properties_init(&rt->std, ce);
	rt->std.handlers = &php_sdl_renderer_handlers;

	return &rt->std;
} /* }}} */

 /* {{{ php_sdl_renderer_dtor_storage */
static void php_sdl_renderer_dtor_storage(zend_object *object)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_from(object);

	SDL_DestroyRenderer(rt->renderer);
	zend_objects_destroy_object(object);
} /* }}} */

/* {{{ php_sdl_renderer_free_storage */
static void php_sdl_renderer_free_storage(zend_object *object)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_from(object);

	zend_object_std_dtor(object);
} /* }}} */

/* {{{ proto Renderer Renderer::__construct(Window window[, int index = -1[, int flags = 0]]) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer___construct_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, window, SDL\\Window, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, __construct) 
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());
	php_sdl_window_t *wt;

	zval *window;
	zend_long index = -1;
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O|ll", &window, sdlWindow_ce, &index, &flags) != SUCCESS) {
		return;
	}

	wt = php_sdl_window_fetch(window);

	rt->renderer = SDL_CreateRenderer(wt->window, index, flags);

	if (rt->renderer == NULL) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto Renderer::clear() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_clear_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, clear)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());

	if (SDL_RenderClear(rt->renderer) != 0) {
		php_sdl_error(SDL_GetError());
	}
}
/* }}} */

/* {{{ proto Renderer::present() */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_present_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, present)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());

	SDL_RenderPresent(rt->renderer);
}
/* }}} */

/* {{{ proto Renderer::setDrawColor(int r, int g, int b[, int a = 0xff]) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_setDrawColor_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, r, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, g, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, setDrawColor)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());

	zend_long r;
	zend_long g;
	zend_long b;
	zend_long a = SDL_ALPHA_OPAQUE;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lll|l", &r, &g, &b, &a) != SUCCESS) {
		return;
	}

	if (SDL_SetRenderDrawColor(rt->renderer, r, g, b, a) != 0) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto Renderer::copy(Texture texture, Rect srcrect, Rect dstrect, double angle, int flip) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_copy_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, texture, SDL\\Texture, 0)
	ZEND_ARG_OBJ_INFO(0, srcrect, SDL\\Rect, 1)
	ZEND_ARG_OBJ_INFO(0, dstrect, SDL\\Rect, 1)
	ZEND_ARG_TYPE_INFO(0, angle, IS_DOUBLE, 1)
	ZEND_ARG_OBJ_INFO(0, center, SDL\\Point, 1)
	ZEND_ARG_TYPE_INFO(0, flip, IS_LONG, 1)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, copy)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());
	php_sdl_texture_t *tt;
	SDL_Rect *srcR = NULL;
	SDL_Rect *dstR = NULL;
	SDL_Point *centerP = NULL;

	zval *texture;
	zval *srcrect = NULL;
	zval *dstrect = NULL;
	zval *center = NULL;
	double angle = 0;
	zend_long flip = SDL_FLIP_NONE;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O|OOdOl", &texture, sdlTexture_ce, &srcrect, sdlRect_ce, &dstrect, sdlRect_ce, &angle, &center, sdlPoint_ce, &flip) != SUCCESS) {
		return;
	}

	if (srcrect) {
		srcR = &php_sdl_rect_fetch(srcrect)->rect;
	}
	if (dstrect) {
		dstR = &php_sdl_rect_fetch(dstrect)->rect;
	}
	if (center) {
		centerP = &php_sdl_point_fetch(center)->point;
	}

	tt = php_sdl_texture_fetch(texture);

	if (SDL_RenderCopyEx(rt->renderer, tt->texture, srcR, dstR, angle, centerP, flip) != 0) {
		php_sdl_error(SDL_GetError());
	}
}
/* }}} */

/* {{{ proto Renderer::drawLine(int x1, int y1, int x2, int y2) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_drawLine_info, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, x1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, x2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y2, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, drawLine)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());

	zend_long x1;
	zend_long y1;
	zend_long x2;
	zend_long y2;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "llll", &x1, &y1, &x2, &y2) != SUCCESS) {
		return;
	}

	if (0 != SDL_RenderDrawLine(rt->renderer, x1, y1, x2, y2)) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto Renderer::drawPoint(int x, int y) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_drawPoint_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, drawPoint)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());

	zend_long x;
	zend_long y;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ll", &x, &y) != SUCCESS) {
		return;
	}

	if (0 != SDL_RenderDrawPoint(rt->renderer, x, y)) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

 /* {{{ proto Renderer::drawRect(Rect rect) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_drawRect_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, rect, SDL\\Rect, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, drawRect)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());

	zval* zrect;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &zrect, sdlRect_ce) != SUCCESS) {
		return;
	}

	php_sdl_rect_t *rct = php_sdl_rect_fetch(zrect);

	if (0 != SDL_RenderDrawRect(rt->renderer, &rct->rect)) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto Renderer::fillRect(Rect rect) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_fillRect_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, rect, SDL\\Rect, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, fillRect)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());

	zval* zrect;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &zrect, sdlRect_ce) != SUCCESS) {
		return;
	}

	php_sdl_rect_t *rct = php_sdl_rect_fetch(zrect);

	if (0 != SDL_RenderFillRect(rt->renderer, &rct->rect)) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto Renderer::drawLines(array<Point> points) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_drawLines_info, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, points, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, drawLines)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());

	zval *zpoints;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "a", &zpoints) != SUCCESS) {
		return;
	}

	SDL_Point *sdl_points = ecalloc(zend_hash_num_elements(Z_ARRVAL_P(zpoints)), sizeof(SDL_Point));
	int i = 0;
	zval *zpoint;

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(zpoints), zpoint) {
		if (Z_TYPE_P(zpoint) == IS_OBJECT && Z_OBJCE_P(zpoint) == sdlPoint_ce) {
			php_sdl_point_t *pt = php_sdl_point_fetch(zpoint);
			sdl_points[i++] = pt->point;
		}
	} ZEND_HASH_FOREACH_END();

	int err = SDL_RenderDrawLines(rt->renderer, sdl_points, i);

	efree(sdl_points);

	if (err) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto Renderer::drawPoints(array<Point> points) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_drawPoints_info, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, points, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, drawPoints)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());

	zval *zpoints;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "a", &zpoints) != SUCCESS) {
		return;
	}

	SDL_Point *sdl_points = ecalloc(zend_hash_num_elements(Z_ARRVAL_P(zpoints)), sizeof(SDL_Point));
	int i = 0;
	zval *zpoint;

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(zpoints), zpoint) {
		if (Z_TYPE_P(zpoint) == IS_OBJECT && Z_OBJCE_P(zpoint) == sdlPoint_ce) {
			php_sdl_point_t *pt = php_sdl_point_fetch(zpoint);
			sdl_points[i++] = pt->point;
		}
	} ZEND_HASH_FOREACH_END();

	int err = SDL_RenderDrawPoints(rt->renderer, sdl_points, i);

	efree(sdl_points);

	if (err) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto Renderer::drawRects(array<Rect> rects) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_drawRects_info, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, rects, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, drawRects)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());
	zval *rects;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "a", &rects) != SUCCESS) {
		return;
	}

	SDL_Rect *sdl_rects = ecalloc(zend_hash_num_elements(Z_ARRVAL_P(rects)), sizeof(SDL_Rect));
	int i = 0;
	zval *zrect;

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(rects), zrect) {
		if (Z_TYPE_P(zrect) == IS_OBJECT && Z_OBJCE_P(zrect) == sdlRect_ce) {
			php_sdl_rect_t *rct = php_sdl_rect_fetch(zrect);
			sdl_rects[i++] = rct->rect;
		}
	} ZEND_HASH_FOREACH_END();

	int err = SDL_RenderDrawRects(rt->renderer, sdl_rects, i);

	efree(sdl_rects);
	
	if (err) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto Renderer::fillRects(array<Rect> rects) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer_fillRects_info, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, rects, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, fillRects)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());
	zval *rects;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "a", &rects) != SUCCESS) {
		return;
	}

	SDL_Rect *sdl_rects = ecalloc(zend_hash_num_elements(Z_ARRVAL_P(rects)), sizeof(SDL_Rect));
	int i = 0;
	zval *zrect;

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(rects), zrect) {
		if (Z_TYPE_P(zrect) == IS_OBJECT && Z_OBJCE_P(zrect) == sdlRect_ce) {
			php_sdl_rect_t *rct = php_sdl_rect_fetch(zrect);
			sdl_rects[i++] = rct->rect;
		}
	} ZEND_HASH_FOREACH_END();

	int err = SDL_RenderFillRects(rt->renderer, sdl_rects, i);

	efree(sdl_rects);
	
	if (err) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ php_sdl_renderer_methods[] */
const zend_function_entry php_sdl_renderer_methods[] = {
	PHP_ME(Renderer, __construct, php_sdl_renderer___construct_info, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, clear, php_sdl_renderer_clear_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, present, php_sdl_renderer_present_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, setDrawColor, php_sdl_renderer_setDrawColor_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, copy, php_sdl_renderer_copy_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, drawLine, php_sdl_renderer_drawLine_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, drawPoint, php_sdl_renderer_drawPoint_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, drawRect, php_sdl_renderer_drawRect_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, fillRect, php_sdl_renderer_fillRect_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, drawLines, php_sdl_renderer_drawLines_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, drawPoints, php_sdl_renderer_drawPoints_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, drawRects, php_sdl_renderer_drawRects_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, fillRects, php_sdl_renderer_fillRects_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

PHP_MINIT_FUNCTION(SDL_Renderer) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Renderer", php_sdl_renderer_methods);
	sdlRenderer_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlRenderer_ce->ce_flags |= ZEND_ACC_FINAL;
	sdlRenderer_ce->create_object = php_sdl_renderer_create;
	sdlRenderer_ce->serialize = zend_class_serialize_deny;
	sdlRenderer_ce->unserialize = zend_class_unserialize_deny;

	memcpy(&php_sdl_renderer_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_sdl_renderer_handlers.dtor_obj = php_sdl_renderer_dtor_storage;
	php_sdl_renderer_handlers.free_obj = php_sdl_renderer_free_storage;
	php_sdl_renderer_handlers.clone_obj = NULL;

	php_sdl_renderer_handlers.offset = XtOffsetOf(php_sdl_renderer_t, std);

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
