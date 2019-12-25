#ifndef __ModuleFonts_H__
#define __ModuleFonts_H__

#include "j1Module.h"
#include "j1Gui.h"
#include "UI_Elements.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font
{
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	uint rows, len, char_w, char_h, row_chars;
};

class j1Fonts : public UI_Elements
{
public:
	Font	 fonts[MAX_FONTS];
	j1Fonts();
	~j1Fonts() {};

	// Load Font
	int Load(const char* texture_path, const char* characters, uint rows, uint h, uint w, uint rc);
	void UnLoad(int font_id);

	// Create a surface from text
	void BlitText(int x, int y, int bmp_font_id, const char* text, int space = 0) const;

private:


};


#endif // __ModuleFonts_H__