// 4.16.0 0xc46d1933
// Generated by imageconverter. Please, do not edit!

#include <BitmapDatabase.hpp>
#include <touchgfx/Bitmap.hpp>

extern const unsigned char image_blue_buttons_round_edge_small[]; // BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID = 0, Size: 170x60 pixels
extern const unsigned char image_blue_buttons_round_edge_small_pressed[]; // BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID = 1, Size: 170x60 pixels
extern const unsigned char image_digitalred2[]; // BITMAP_DIGITALRED2_ID = 2, Size: 800x566 pixels
extern const unsigned char image_digitalred3[]; // BITMAP_DIGITALRED3_ID = 3, Size: 400x357 pixels
extern const unsigned char image_imrlogotransparent[]; // BITMAP_IMRLOGOTRANSPARENT_ID = 4, Size: 700x175 pixels

const touchgfx::Bitmap::BitmapData bitmap_database[] =
{
    { image_blue_buttons_round_edge_small, 0, 170, 60, 11, 4, 148, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 50, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_blue_buttons_round_edge_small_pressed, 0, 170, 60, 11, 4, 148, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 50, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_digitalred2, 0, 800, 566, 0, 0, 800, (uint8_t)(touchgfx::Bitmap::RGB888) >> 3, 566, (uint8_t)(touchgfx::Bitmap::RGB888) & 0x7 },
    { image_digitalred3, 0, 400, 357, 0, 0, 400, (uint8_t)(touchgfx::Bitmap::RGB888) >> 3, 357, (uint8_t)(touchgfx::Bitmap::RGB888) & 0x7 },
    { image_imrlogotransparent, 0, 700, 175, 208, 38, 11, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 59, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 }
};

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return bitmap_database;
}

uint16_t getInstanceSize()
{
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
}
}
