#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1RectSprites.h"
#include "p2Log.h"
#include "j1Module.h"
#include "UI_Elements.h"


#include<string.h>

// Constructor
j1Fonts::j1Fonts() : UI_Elements()
{}


// Load new texture from file path
int j1Fonts::Load(const char* texture_path, const char* characters, uint rows, uint w, uint h, uint rc)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = App->tex->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 1;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].graphic == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	fonts[id].graphic = tex; // graphic: pointer to the texture
	fonts[id].rows = rows; // rows: rows of characters in the texture
	fonts[id].len = 0; // len: length of the table

	fonts[id].char_h = h;
	fonts[id].char_w = w;
	fonts[id].row_chars = rc;
	for (int i = 0; i < MAX_FONT_CHARS; i++) {
		fonts[id].table[i] = characters[i];
		if (characters[i] == NULL) { break; }
	}
	// TODO 1: Finish storing font data

	// table: array of chars to have the list of characters
	// row_chars: amount of chars per row of the texture
	// char_w: width of each character
	// char_h: height of each character

	LOG("Successfully loaded BMP font from %s", texture_path);

	return id;
}

void j1Fonts::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].graphic != nullptr)
	{
		App->tex->UnLoad(fonts[font_id].graphic);
		fonts[font_id].graphic = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

// Render text using a bitmap font
void j1Fonts::BlitText(int x, int y, int font_id, const char* text, int space) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].graphic == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	RectSprites rect;
	uint len = strlen(text);
	int y2 = y;
	rect.rect.w = font->char_w;
	rect.rect.h = font->char_h;

	for (int i = 0; i < len; ++i)
	{
		for (int j = 0; j < MAX_FONT_CHARS; j++) {
			if (text[i] == font->table[j]) {
				rect.rect.x = j * font->char_w;
				rect.rect.y = 0;
				if (font_id == 0 && j == 36) { y -= 2; }
				App->render->Blit(font->graphic, x, y, &rect.rect, 0, 1, false);
				y = y2;
				x += font->char_w + space;
				break;
			}
			if (j > font->row_chars) { break; }
		}
		// TODO 2: Find the character in the table and its position in the texture, then Blit
	}
}