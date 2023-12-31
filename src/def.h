#define FPS_INTERVAL 1.0
#define FPS60 1.0/60.0
#define MAX_KEYBOARD_KEYS 350
#define MAX_LINE_LENGTH 1024
#define GLYPH_HEIGHT 16
#define GLYPH_WIDTH  10
#define FONT_HEIGHT 256
#define FONT_WIDTH 160
#define FONT_BOXW (FONT_WIDTH/GLYPH_WIDTH)
#define FONT_BOXH (FONT_HEIGHT/GLYPH_HEIGHT)

enum
{
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT
};

typedef struct Matrix {
	float m[16];
} Matrix;

typedef struct {
	int x;
	int y;
	int xr;
	int yr;
	int lb;
	int rb;
	int wu;
	int wd;
	int mb;
	int hwu;
	int hwd;
} Mouse;

typedef struct {
	float r;
	float g;
	float b;
} RGBColorf;

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGBColori;
