#ifndef __Resources_H__
#define __Resources_H__


#define SUDOKU_GRID (9)

//各个对象的位置关系，数值大的覆盖在数值小的上面
#define Z_BACKGROUND  0
#define Z_GRID_NUMBER 1
#define Z_INPUTPAD    10
#define Z_INPUT_GRID_NUMBER 11
#define Z_WIN_FIREWORK  20
#define Z_LOG         99

//表格原点坐标
#define O_GRID_X (30)
#define O_GRID_Y (132)

//日志信息坐标
#define O_LOG_X (140)
#define O_LOG_Y (20)

#define VALID_SUM (45)

#define MUSIC_FILE "Resources/sound/sudoku.mp3"
#define CLICK_FILE "Resources/sound/pickup_coin.wav"

static const int ROW_NUM = 3;
static const int COL_NUM = 3;

static const char s_MainBkg[]               = "Resources/Images/main_bkg.png";
static const char s_menu[]                  = "Resources/Images/menu.png";
static const char s_MenuItem[]              = "Resources/Images/menuitemsprite.png";
static const char s_BackGround[]            = "Resources/Images/background1.png";
static const char s_Grid[]                  = "Resources/Images/grid_down.png";
static const char s_GridUp[]                = "Resources/Images/grid_up.png";
static const char s_FpsImage[]              = "Resources/fonts/fps_images.png";
static const char s_InputPad[]              = "Resources/Images/background_input.png";
static const char s_stars1[]                = "Resources/Images/stars.png";
static const char s_SpinPea[]               = "Resources/Images/SpinningPeas.png";

static const char s_BtnCloseNormal[]   		= "Resources/Images/CloseNormal.png";
static const char s_BtnCloseSelected[]   	= "Resources/Images/CloseSelected.png";

static const char s_TestChs[]               = "测试";



#endif