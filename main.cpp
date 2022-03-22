#include "mbed.h"
#include "neopixel.h"
#include <cstdio>

#define nb_led 15

void init();
void led_controlle(int led, uint32_t color, Pixel *pix);
void led_init(float adj_lum);
void led_cleaner(Pixel *pix);

typedef enum
{
	btn1_1 = 0x27,
    btn1_2 = 0x47,
    btn1_3 = 0x87,
    btn2_1 = 0x2B,
    btn2_2 = 0x4B,
    btn2_3 = 0x8B,
    btn3_1 = 0x2D,
    btn3_2 = 0x4D,
    btn3_3 = 0x8D,
    btn4_1 = 0x2E,
    btn4_2 = 0x4E,
    btn4_3 = 0x8E,
}
ButtonMatrix;

uint32_t rouge          = 0xFF0000;
uint32_t jaune          = 0xFFFF00;
uint32_t vert           = 0x00FF00;
uint32_t bleu           = 0x0000FF;
uint32_t violet         = 0xFF00FF;
uint32_t non_couleur    = 0x000000;

float adj_lum = 0.05f;

PortInOut buttons_col(PortA, 0xE0);
PortInOut buttons_row(PortA, 0xF);

NeoPixelOut neoPixel(PA_12);
Pixel pixels[nb_led];

#define MAXIMUM_BUFFER_SIZE 32
static BufferedSerial serial_port(PA_9, PA_10);

FileHandle *mbed::mbed_override_console(int fd)
{
	return &serial_port;
}

int main()
{

	init();

	serial_port.set_baud(9600);
	serial_port.set_format(8, BufferedSerial::None, 1);

	char buf[MAXIMUM_BUFFER_SIZE] = { 0 };

	buttons_row.mode(PullUp);

	while (1)
	{
		buttons_row.output();
		buttons_col.input();
		unsigned int btnCol = buttons_col.read();
		printf("buttons_col: %x\n", btnCol);

		buttons_row.input();
		buttons_col.output();
		unsigned int btnRow = buttons_row.read();
		printf("buttons_row: %x\n", btnRow);
        
		printf("---------------------\n");

		unsigned int btn = btnCol | btnRow;

		switch (btn)
		{
			case btn1_1:
				led_cleaner(pixels);
				led_controlle(1, jaune, pixels);
				break;
			case btn1_2:
				led_cleaner(pixels);
				led_controlle(2, bleu, pixels);
				break;
			case btn1_3:
				led_cleaner(pixels);
				led_controlle(3, bleu, pixels);
				break;
			case btn2_1:
				led_cleaner(pixels);
				led_controlle(4, violet, pixels);
				break;
			case btn2_2:
				led_cleaner(pixels);
				led_controlle(5, vert, pixels);
				break;
			case btn2_3:
				led_cleaner(pixels);
				led_controlle(6, bleu, pixels);
				break;
			case btn3_1:
				led_cleaner(pixels);
				led_controlle(7, vert, pixels);
				break;
			case btn3_2:
				led_cleaner(pixels);
				led_controlle(8, bleu, pixels);
				break;
			case btn3_3:
				led_cleaner(pixels);
				led_controlle(9, violet, pixels);
				break;
			case btn4_1:
				led_cleaner(pixels);
				led_controlle(10, bleu, pixels);
				break;
			case btn4_2:
				led_cleaner(pixels);
				led_controlle(11, vert, pixels);
				break;
			case btn4_3:
				led_cleaner(pixels);
				led_controlle(12, violet, pixels);
				break;
		}
	}
}

// Инициализация LED ленты
void init()
{
	led_init(adj_lum);
}

void led_cleaner(Pixel *pix)
{
	for (int i = 1; i <= nb_led; i++)
	{
		led_controlle(i, non_couleur, pix);
	}
}

void led_init(float adj_lum)
{
	ThisThread::sleep_for(20ms);
	neoPixel.global_scale = adj_lum;
	neoPixel.normalize = false;
}

void led_controlle(int led, uint32_t color, Pixel *pix)
{
	pix[led - 1].hex = color;
	neoPixel.send(pixels, nb_led);
}