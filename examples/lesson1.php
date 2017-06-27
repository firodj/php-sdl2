<?php

SDL\init(SDL\INIT_VIDEO | SDL\INIT_TIMER | SDL\INIT_EVENTS);
SDL\initIMG(SDL\INIT_PNG);

$window = new SDL\Window("Wew", SDL\WindowPos::UNDEFINED, SDL\WindowPos::UNDEFINED, 800, 600, SDL\WindowFlags::SHOWN);
$renderer = new SDL\Renderer($window, -1, SDL\RendererFlags::ACCELERATED);

// $sprite = new SDL\Texture($renderer, SDL\PixelFormat::ARGB8888, SDL\TextureAccess::STATIC, 16, 16);

$renderer->setDrawColor(0x00, 0x00, 0xff);

SDL\Event::setState(SDL\EventType::FINGERMOTION, false);

$surface = SDL\Surface::loadFromFile(__DIR__.'/RyuHayabusa.png');
$surface->setColorKey($surface->mapRGB(0xc8, 0xa8, 0xb8), true);
$sprite = SDL\Texture::createFromSurface($renderer, $surface);
$rect = new SDL\Rect(22, 26, 17, 32);
$pos = new SDL\Rect(0, 0, 17, 32);

function refresh(int $timestamp=null, int $interval=null) {
    global $pos, $sprite, $rect, $renderer;
    $renderer->clear();
    $renderer->copy($sprite, $rect, $pos, 45);
    $renderer->present();
}

$timer = SDL\Timer::start(33);
printf("ID:%d\n", $timer->id);

$quit = false;
do {
    while($e = SDL\Event::poll()) {
        switch ($e->type) {
        case SDL\EventType::KEYDOWN:
            echo "keydown.".$e->scancode;
            break;
        case SDL\EventType::KEYUP:
            echo "keyup.".$e->scancode;
            break;
        case SDL\EventType::QUIT;
            $quit = true;
            break;
        case SDL\EventType::MOUSEMOTION:
            $pos->x = $e->x;
            $pos->y = $e->y;
            break;
        case SDL\EventType::MOUSEBUTTONDOWN:
            printf("buttondown.%d(%d,%d)", $e->button, $e->x, $e->y);
            break;
        case SDL\EventType::MOUSEBUTTONUP:
            printf("buttonup.%d", $e->button);
            break;
        case SDL\EventType::WINDOWEVENT:
            if ($e->event == SDL\WindowEvent::CLOSE) {
                SDL\Event::push(new SDL\Event(SDL\EventType::QUIT));
            }
            printf("windowevent.%d", $e->event);
            break;
        case SDL\EventType::TIMEREVENT:
            refresh($e->timestamp, $e->interval);
            break;
        default:
        }
        unset($e);
    }
} while (!$quit);

SDL\Timer::stop($timer);
unset($timer);

unset($renderer);
unset($window);

SDL\quitIMG();
SDL\quit();
