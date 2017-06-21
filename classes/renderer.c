#ifndef HAVE_PHP_SDL_WINDOW
#define HAVE_PHP_SDL_WINDOW

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
#include "./window.h"

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
} /* }}} */

/* {{{ php_sdl_renderer_free_storage */
static void php_sdl_renderer_free_storage(zend_object *object)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_from(object);

	zend_object_std_dtor(&rt->std);
	efree(rt);
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
}

/* }}} */
/* {{{ php_sdl_renderer_methods[] */
const zend_function_entry php_sdl_renderer_methods[] = {
	PHP_ME(Renderer, __construct, php_sdl_renderer___construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, clear, php_sdl_renderer_clear_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, present, php_sdl_renderer_present_info, ZEND_ACC_PUBLIC)
	PHP_ME(Renderer, setDrawColor, php_sdl_renderer_setDrawColor_info, ZEND_ACC_PUBLIC)
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

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
