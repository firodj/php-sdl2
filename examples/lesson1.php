<?php

SDL\init(SDL\INIT_VIDEO | SDL\INIT_TIMER | SDL\INIT_EVENTS);

$window = new SDL\Window("Wew", SDL\WindowPos::UNDEFINED, SDL\WindowPos::UNDEFINED, 800, 600, SDL\WindowFlags::SHOWN);
$renderer = new SDL\Renderer($window, -1, SDL\RendererFlags::ACCELERATED);
$surface = SDL\Surface::loadBMP(__DIR__.'/RyuHayabusa.bmp');

$sprite = SDL\Texture::createFromSurface($renderer, $surface);
/*$sprite = new SDL\Texture($renderer, SDL\PixelFormat::ARGB8888,
    SDL\TextureAccess::STATIC, 16, 16);
 */

$renderer->setDrawColor(0x00, 0x00, 0xff);
$renderer->clear();
$renderer->copy($sprite);
$renderer->present();

class Waktu extends SDL\Timer
{
    public function run(int $ticks) {
        printf("(got:%d)", $ticks);
    }
}

$timer = new Waktu();
$timer->start(33);

$timeout = SDL\Timer::getTicks() + 3000;

do {
    while($e = SDL\Event::poll()) {
        if (is_object($e)) {
            printf("[type:%d]", $e->type);
        }
    }
} while (SDL\Timer::getTicks() < $timeout);


$timer->stop();
echo SDL\Timer::getTicks();

SDL\quit();


