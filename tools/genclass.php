<?php

if (count($argv) <= 1) {
    echo $argv[0] . " <ClassName>\n";
    exit(1);
}

function abbr($str)
{
    return strtolower(preg_replace('/[^A-Z0-9]/', '', $str));
}

$className = ucfirst($argv[1]);

ob_start();
include(__DIR__.'/template/header.h');
file_put_contents(__DIR__.'/output/'.strtolower($className).'.h', ob_get_contents());
ob_end_clean();

ob_start();
include(__DIR__.'/template/source.c');
file_put_contents(__DIR__.'/output/'.strtolower($className).'.c', ob_get_contents());
ob_end_clean();
