#pragma once

// KEYCODE DEFINITIONS (These were stolen from GLFW)
#define WYVERN_KEY_NONE -1

#define WYVERN_KEY_SPACE              32
#define WYVERN_KEY_APOSTROPHE         39  /* ' */
#define WYVERN_KEY_COMMA              44  /* , */
#define WYVERN_KEY_MINUS              45  /* - */
#define WYVERN_KEY_PERIOD             46  /* . */
#define WYVERN_KEY_SLASH              47  /* / */
#define WYVERN_KEY_0                  48
#define WYVERN_KEY_1                  49
#define WYVERN_KEY_2                  50
#define WYVERN_KEY_3                  51
#define WYVERN_KEY_4                  52
#define WYVERN_KEY_5                  53
#define WYVERN_KEY_6                  54
#define WYVERN_KEY_7                  55
#define WYVERN_KEY_8                  56
#define WYVERN_KEY_9                  57
#define WYVERN_KEY_SEMICOLON          59  /* ; */
#define WYVERN_KEY_EQUAL              61  /* = */
#define WYVERN_KEY_A                  65
#define WYVERN_KEY_B                  66
#define WYVERN_KEY_C                  67
#define WYVERN_KEY_D                  68
#define WYVERN_KEY_E                  69
#define WYVERN_KEY_F                  70
#define WYVERN_KEY_G                  71
#define WYVERN_KEY_H                  72
#define WYVERN_KEY_I                  73
#define WYVERN_KEY_J                  74
#define WYVERN_KEY_K                  75
#define WYVERN_KEY_L                  76
#define WYVERN_KEY_M                  77
#define WYVERN_KEY_N                  78
#define WYVERN_KEY_O                  79
#define WYVERN_KEY_P                  80
#define WYVERN_KEY_Q                  81
#define WYVERN_KEY_R                  82
#define WYVERN_KEY_S                  83
#define WYVERN_KEY_T                  84
#define WYVERN_KEY_U                  85
#define WYVERN_KEY_V                  86
#define WYVERN_KEY_W                  87
#define WYVERN_KEY_X                  88
#define WYVERN_KEY_Y                  89
#define WYVERN_KEY_Z                  90
#define WYVERN_KEY_LEFT_BRACKET       91  /* [ */
#define WYVERN_KEY_BACKSLASH          92  /* \ */
#define WYVERN_KEY_RIGHT_BRACKET      93  /* ] */
#define WYVERN_KEY_GRAVE_ACCENT       96  /* ` */
#define WYVERN_KEY_WORLD_1            161 /* non-US #1 */
#define WYVERN_KEY_WORLD_2            162 /* non-US #2 */

/* Func keys */
#define WYVERN_KEY_ESCAPE             256
#define WYVERN_KEY_ENTER              257
#define WYVERN_KEY_TAB                258
#define WYVERN_KEY_BACKSPACE          259
#define WYVERN_KEY_INSERT             260
#define WYVERN_KEY_DELETE             261
#define WYVERN_KEY_RIGHT              262
#define WYVERN_KEY_LEFT               263
#define WYVERN_KEY_DOWN               264
#define WYVERN_KEY_UP                 265
#define WYVERN_KEY_PAGE_UP            266
#define WYVERN_KEY_PAGE_DOWN          267
#define WYVERN_KEY_HOME               268
#define WYVERN_KEY_END                269
#define WYVERN_KEY_CAPS_LOCK          280
#define WYVERN_KEY_SCROLL_LOCK        281
#define WYVERN_KEY_NUM_LOCK           282
#define WYVERN_KEY_PRINT_SCREEN       283
#define WYVERN_KEY_PAUSE              284
#define WYVERN_KEY_F1                 290
#define WYVERN_KEY_F2                 291
#define WYVERN_KEY_F3                 292
#define WYVERN_KEY_F4                 293
#define WYVERN_KEY_F5                 294
#define WYVERN_KEY_F6                 295
#define WYVERN_KEY_F7                 296
#define WYVERN_KEY_F8                 297
#define WYVERN_KEY_F9                 298
#define WYVERN_KEY_F10                299
#define WYVERN_KEY_F11                300
#define WYVERN_KEY_F12                301
#define WYVERN_KEY_F13                302
#define WYVERN_KEY_F14                303
#define WYVERN_KEY_F15                304
#define WYVERN_KEY_F16                305
#define WYVERN_KEY_F17                306
#define WYVERN_KEY_F18                307
#define WYVERN_KEY_F19                308
#define WYVERN_KEY_F20                309
#define WYVERN_KEY_F21                310
#define WYVERN_KEY_F22                311
#define WYVERN_KEY_F23                312
#define WYVERN_KEY_F24                313
#define WYVERN_KEY_F25                314
#define WYVERN_KEY_KP_0               320
#define WYVERN_KEY_KP_1               321
#define WYVERN_KEY_KP_2               322
#define WYVERN_KEY_KP_3               323
#define WYVERN_KEY_KP_4               324
#define WYVERN_KEY_KP_5               325
#define WYVERN_KEY_KP_6               326
#define WYVERN_KEY_KP_7               327
#define WYVERN_KEY_KP_8               328
#define WYVERN_KEY_KP_9               329
#define WYVERN_KEY_KP_DECIMAL         330
#define WYVERN_KEY_KP_DIVIDE          331
#define WYVERN_KEY_KP_MULTIPLY        332
#define WYVERN_KEY_KP_SUBTRACT        333
#define WYVERN_KEY_KP_ADD             334
#define WYVERN_KEY_KP_ENTER           335
#define WYVERN_KEY_KP_EQUAL           336
#define WYVERN_KEY_LEFT_SHIFT         340
#define WYVERN_KEY_LEFT_CONTROL       341
#define WYVERN_KEY_LEFT_ALT           342
#define WYVERN_KEY_LEFT_SUPER         343
#define WYVERN_KEY_RIGHT_SHIFT        344
#define WYVERN_KEY_RIGHT_CONTROL      345
#define WYVERN_KEY_RIGHT_ALT          346
#define WYVERN_KEY_RIGHT_SUPER        347
#define WYVERN_KEY_MENU               348

#define WYVERN_KEY_LAST               GLFW_KEY_MENU

#define 	WYVERN_MOD_NONE				0x0000
#define 	WYVERN_MOD_SHIFT			0x0001
#define 	WYVERN_MOD_CONTROL			0x0002
#define 	WYVERN_MOD_ALT				0x0004
#define 	WYVERN_MOD_SUPER			0x0008
#define 	WYVERN_MOD_CAPS_LOCK		0x0010
#define 	WYVERN_MOD_NUM_LOCK			0x0020

