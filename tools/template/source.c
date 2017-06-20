#include "php.h"
#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./<?= strtolower($className) ?>.h"

static zend_object_handlers php_sdl_<?= strtolower($className) ?>_handlers;

zend_class_entry *sdl<?= $className ?>_ce;

/* {{{ php_sdl_<?= strtolower($className) ?>_create */
static zend_object* php_sdl_<?= strtolower($className) ?>_create(zend_class_entry *ce)
{
	php_sdl_<?= strtolower($className) ?>_t *<?= abbr($className) ?>t = 
		(php_sdl_<?= strtolower($className) ?>_t*) ecalloc(1, sizeof(php_sdl_<?= strtolower($className) ?>_t) + zend_object_properties_size(ce));

	zend_object_std_init(&<?= abbr($className) ?>t->std, ce);
	object_properties_init(&<?= abbr($className) ?>t->std, ce);
	<?= abbr($className) ?>t->std.handlers = &php_sdl_<?= strtolower($className) ?>_handlers;

	return &<?= abbr($className) ?>t->std;
} /* }}} */

/* {{{ php_sdl_<?= strtolower($className) ?>_dtor_storage */
static void php_sdl_<?= strtolower($className) ?>_dtor_storage(zend_object *object)
{
	php_sdl_<?= strtolower($className) ?>_t *<?= abbr($className) ?>t = php_sdl_<?= strtolower($className) ?>_from_obj(object);

	DESTROY(<?= abbr($className) ?>t-><?= strtolower($className) ?>);
} /* }}} */

/* {{{ php_sdl_<?= strtolower($className) ?>_free_storage */
static void php_sdl_<?= strtolower($className) ?>_free_storage(zend_object *object)
{
	php_sdl_<?= strtolower($className) ?>_t *<?= abbr($className) ?>t = php_sdl_<?= strtolower($className) ?>_from_obj(object);

	zend_object_std_dtor(&<?= abbr($className) ?>t->std);
	efree(<?= abbr($className) ?>t);
} /* }}} */

/* {{{ proto <?= $className ?> <?= $className ?>::__construct(string text, int number) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_<?= strtolower($className) ?>___construct_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(<?= $className ?>, __construct)
{
	php_sdl_<?= strtolower($className) ?>_t *<?= abbr($className) ?>t = php_sdl_<?= strtolower($className) ?>_fetch(getThis());

	zend_string *text;
	zend_long number;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Sl", &text, &number) != SUCCESS) {
		return;
	}

	<?= abbr($className) ?>t-><?= strtolower($className) ?> = CREATE(ZSTR_VAL(text), number);

	if (<?= abbr($className) ?>t-><?= strtolower($className) ?> == NULL) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ php_sdl_<?= strtolower($className) ?>_methods */
const zend_function_entry php_sdl_<?= strtolower($className) ?>_methods[] = {
	PHP_ME(<?= $className ?>, __construct, php_sdl_<?= strtolower($className) ?>___construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

PHP_MINIT_FUNCTION(SDL_<?= $className ?>) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "<?= $className ?>", NULL);
	sdl<?= $className ?>_ce = zend_register_internal_class_ex(&ce, NULL);
	// TODO: For Const
	sdl<?= $className ?>_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_<?= strtoupper($className) ?>_CONST(SOMECONST);

	// TODO: For Class
	sdl<?= $className ?>_ce->ce_flags |= ZEND_ACC_FINAL;
	sdl<?= $className ?>_ce->create_object = php_sdl_<?= strtolower($className) ?>_create;
	sdl<?= $className ?>_ce->serialize = zend_class_serialize_deny;
	sdl<?= $className ?>_ce->unserialize = zend_class_unserialize_deny;

	memcpy(&php_sdl_<?= strtolower($className) ?>_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_sdl_<?= strtolower($className) ?>_handlers.free_obj = php_sdl_<?= strtolower($className) ?>_free_storage;
	php_sdl_<?= strtolower($className) ?>_handlers.dtor_obj = php_sdl_<?= strtolower($className) ?>_dtor_storage;
	php_sdl_<?= strtolower($className) ?>_handlers.clone_obj = NULL;

	php_sdl_<?= strtolower($className) ?>_handlers.offset = XtOffsetOf(php_sdl_<?= strtolower($className) ?>_t, std);

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
