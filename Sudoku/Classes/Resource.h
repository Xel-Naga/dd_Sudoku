#ifndef __Resources_H__
#define __Resources_H__


#define SUDOKU_GRID (9)

//各个对象的位置关系，数值大的覆盖在数值小的上面
//先建立
#define Z_BACKGROUND  0
#define Z_GAME_LAYER 10 //游戏画面层
#define Z_HUD_LAYER 30  //用户界面层
#define Z_GRID_NUMBER 30
#define Z_INPUTPAD    31
#define Z_INPUT_GRID_NUMBER 32
#define Z_WIN_FIREWORK  33
#define Z_TOP_LAYER 50  //最顶层
#define Z_LOG         99

//表格原点坐标
#define O_GRID_X (30)
#define O_GRID_Y (132)

//日志信息坐标
#define O_LOG_X (140)
#define O_LOG_Y (20)

#define VALID_SUM (45)

#define MUSIC_FILE "sound/sudoku.mp3"
#define CLICK_FILE "sound/pickup_coin.wav"

static const int ROW_NUM = 3;
static const int COL_NUM = 3;

static const char s_MainBkg[]               = "Images/main_bkg.png";
static const char s_menu[]                  = "Images/menu.png";
static const char s_BackGround[]            = "Images/background1.png";
static const char s_Grid[]                  = "Images/grid_down.png";
static const char s_GridUp[]                = "Images/grid_up.png";
static const char s_FpsImage[]              = "fonts/fps_images.png";
static const char s_InputPad[]              = "Images/background_input.png";
static const char s_stars1[]                = "Images/stars.png";
static const char s_SpinPea[]               = "Images/SpinningPeas.png";

static const char s_BtnCloseNormal[]   		= "Images/CloseNormal.png";
static const char s_BtnCloseSelected[]   	= "Images/CloseSelected.png";

static const char s_BtnLevelNormal[]   	= "Images/star_0.png";
static const char s_lock_0[]   	= "Images/lock0.png";

static const char s_back[] = "Images/back.png";
static const char s_back_sel[] = "Images/back_sel.png";
static const char s_play[] = "Images/play.png";
static const char s_play_sel[] = "Images/play_sel.png";
static const char s_pause[] = "Images/pause.png";
static const char s_pause_sel[] = "Images/pause_sel.png";
static const char s_home[] = "Images/home.png";
static const char s_home_sel[] = "Images/home_sel.png";

static const char s_grid_frame[] = "Images/grid_frame.png";


static const char s_TestChs[]               = "测试";
static const std::wstring s_LoadingChs               = L"测试一下加载界面...";



#endif