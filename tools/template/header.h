#ifndef HAVE_PHP_SDL_<?= strtoupper($className) ?>_H
#define HAVE_PHP_SDL_<?= strtoupper($className) ?>_H

extern zend_class_entry *sdl<?= $className ?>_ce;

typedef struct _php_sdl_<?= strtolower($className) ?>_t {
	SDL_<?= $className ?> *<?= strtolower($className) ?>;
	zend_object std;
} php_sdl_<?= strtolower($className) ?>_t;

#define php_sdl_<?= strtolower($className) ?>_from_obj(o) ((php_sdl_<?= strtolower($className) ?>_t*) ((char*) o - XtOffsetOf(php_sdl_<?= strtolower($className) ?>_t, std)))
#define php_sdl_<?= strtolower($className) ?>_fetch(z) php_sdl_<?= strtolower($className) ?>_from_obj(Z_OBJ_P(z))
#define SDL_<?= strtoupper($className) ?>_CONST(name) zend_declare_class_constant_long(sdl<?= $className ?>_ce, ZEND_STRL(#name), SDL_<?= strtoupper($className) ?>_##name);

PHP_MINIT_FUNCTION(SDL_<?= $className ?>);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
