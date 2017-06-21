<?php

SDL\init(SDL\INIT_VIDEO);

$window = new SDL\Window("Wew", 0, 0, 800, 600, SDL\WindowFlags::SHOWN);
$renderer = new SDL\Renderer($window, -1, SDL\RendererFlags::ACCELERATED);
$sprite = new SDL\Texture($renderer, SDL\PixelFormat::ARGB8888,
    SDL\TextureAccess::STATIC, 16, 16);
$renderer->setDrawColor(0x00, 0x00, 0xff);
$renderer->clear();
$renderer->present();

sleep(3);

SDL\quit();

