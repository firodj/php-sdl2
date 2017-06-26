<?php

SDL\init(SDL\INIT_VIDEO | SDL\INIT_TIMER | SDL\INIT_EVENTS);
SDL\initIMG(SDL\INIT_PNG);

$window = new SDL\Window("Wew", SDL\WindowPos::UNDEFINED, SDL\WindowPos::UNDEFINED, 800, 600, SDL\WindowFlags::SHOWN);
$renderer = new SDL\Renderer($window, -1, SDL\RendererFlags::ACCELERATED);

/*$sprite = new SDL\Texture($renderer, SDL\PixelFormat::ARGB8888,
    SDL\TextureAccess::STATIC, 16, 16);
 */

$renderer->setDrawColor(0x00, 0x00, 0xff);

class Waktu extends SDL\Timer
{
    private $renderer;
    public $pos;

    public function __construct($renderer) {
        $this->renderer = $renderer;

        $surface = SDL\Surface::loadFromFile(__DIR__.'/RyuHayabusa.png');
        $surface->setColorKey(true, $surface->mapRGB(0xc8, 0xa8, 0xb8));
        $this->sprite = SDL\Texture::createFromSurface($renderer, $surface);

        $this->rect = new SDL\Rect(22, 26, 17, 32);
        $this->pos = new SDL\Rect(0, 0, 17, 32);
    }

    public function run(int $timestamp=null, int $interval=null) {
        $this->renderer->clear();
        $this->renderer->copy($this->sprite, $this->rect, $this->pos, 45);
        $this->renderer->present();

        //printf("(got:%d,%d)", $interval, $timestamp);
    }
}

SDL\Event::setState(SDL\EventType::FINGERMOTION, false);

$timer = new Waktu($renderer);
$timer->start(33);

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
            $timer->pos->x = $e->x;
            $timer->pos->y = $e->y;
            break;
        case SDL\EventType::MOUSEBUTTONDOWN:
            printf("buttondown.%d(%d,%d)", $e->button, $e->x, $e->y);
            break;
        case SDL\EventType::MOUSEBUTTONUP:
            printf("buttonup.%d", $e->button);
            break;
        case SDL\EventType::WINDOWEVENT:
            if ($e->event == SDL\WindowEvent::CLOSE) {
                echo "Quit";
                $quit = true;
            }
            printf("windowevent.%d", $e->event);
            break;
        default:
            //printf("[type:%d]", $e->type);
        }
    }
} while (!$quit);

$timer->stop();
echo "stop:".SDL\Timer::getTicks().PHP_EOL;

SDL\quitIMG();
SDL\quit();
