#ifndef _VERLET_INTEGRATION_ENGINE_H_
#define _VERLET_INTEGRATION_ENGINE_H_ 1



#define ENGINE_FLAG_LEFT 1
#define ENGINE_FLAG_MIDDLE 2
#define ENGINE_FLAG_RIGHT 4
#define ENGINE_FLAG_SCROLL_UP 8
#define ENGINE_FLAG_SCROLL_DOWN 16
#define ENGINE_FLAG_SIZE 32

#define	ENGINE_KEY_BACKSPACE 8
#define	ENGINE_KEY_TAB 9
#define	ENGINE_KEY_ENTER 10
#define	ENGINE_KEY_ESCAPE 27
#define	ENGINE_KEY_SPACE 32
#define	ENGINE_KEY_APOSTROPHE 39
#define	ENGINE_KEY_COMMA 44
#define	ENGINE_KEY_MINUS 45
#define	ENGINE_KEY_PERIOD 46
#define	ENGINE_KEY_SLASH 47
#define	ENGINE_KEY_0 48
#define	ENGINE_KEY_1 49
#define	ENGINE_KEY_2 50
#define	ENGINE_KEY_3 51
#define	ENGINE_KEY_4 52
#define	ENGINE_KEY_5 53
#define	ENGINE_KEY_6 54
#define	ENGINE_KEY_7 55
#define	ENGINE_KEY_8 56
#define	ENGINE_KEY_9 57
#define	ENGINE_KEY_SEMICOLON 59
#define	ENGINE_KEY_EQUAL 61
#define	ENGINE_KEY_A 65
#define	ENGINE_KEY_B 66
#define	ENGINE_KEY_C 67
#define	ENGINE_KEY_D 68
#define	ENGINE_KEY_E 69
#define	ENGINE_KEY_F 70
#define	ENGINE_KEY_G 71
#define	ENGINE_KEY_H 72
#define	ENGINE_KEY_I 73
#define	ENGINE_KEY_J 74
#define	ENGINE_KEY_K 75
#define	ENGINE_KEY_L 76
#define	ENGINE_KEY_M 77
#define	ENGINE_KEY_N 78
#define	ENGINE_KEY_O 79
#define	ENGINE_KEY_P 80
#define	ENGINE_KEY_Q 81
#define	ENGINE_KEY_R 82
#define	ENGINE_KEY_S 83
#define	ENGINE_KEY_T 84
#define	ENGINE_KEY_U 85
#define	ENGINE_KEY_V 86
#define	ENGINE_KEY_W 87
#define	ENGINE_KEY_X 88
#define	ENGINE_KEY_Y 89
#define	ENGINE_KEY_Z 90
#define	ENGINE_KEY_LEFT_BRACKET 91
#define	ENGINE_KEY_BACKSLASH 92
#define	ENGINE_KEY_RIGHT_BRACKET 93
#define	ENGINE_KEY_GRAVE 96

#define	ENGINE_KEY_ALT 128
#define	ENGINE_KEY_CAPS_LOCK 129
#define	ENGINE_KEY_CTRL 130
#define	ENGINE_KEY_DELETE 131
#define	ENGINE_KEY_DOWN 132
#define	ENGINE_KEY_END 133
#define	ENGINE_KEY_F1 134
#define	ENGINE_KEY_F10 135
#define	ENGINE_KEY_F11 136
#define	ENGINE_KEY_F12 137
#define	ENGINE_KEY_F13 138
#define	ENGINE_KEY_F14 139
#define	ENGINE_KEY_F15 140
#define	ENGINE_KEY_F16 141
#define	ENGINE_KEY_F17 142
#define	ENGINE_KEY_F18 143
#define	ENGINE_KEY_F19 144
#define	ENGINE_KEY_F2 145
#define	ENGINE_KEY_F20 146
#define	ENGINE_KEY_F21 147
#define	ENGINE_KEY_F22 148
#define	ENGINE_KEY_F23 149
#define	ENGINE_KEY_F24 150
#define	ENGINE_KEY_F25 151
#define	ENGINE_KEY_F3 152
#define	ENGINE_KEY_F4 153
#define	ENGINE_KEY_F5 154
#define	ENGINE_KEY_F6 155
#define	ENGINE_KEY_F7 156
#define	ENGINE_KEY_F8 157
#define	ENGINE_KEY_F9 158
#define	ENGINE_KEY_HOME 159
#define	ENGINE_KEY_INSERT 160
#define	ENGINE_KEY_KP_0 161
#define	ENGINE_KEY_KP_1 162
#define	ENGINE_KEY_KP_2 163
#define	ENGINE_KEY_KP_3 164
#define	ENGINE_KEY_KP_4 165
#define	ENGINE_KEY_KP_5 166
#define	ENGINE_KEY_KP_6 167
#define	ENGINE_KEY_KP_7 168
#define	ENGINE_KEY_KP_8 169
#define	ENGINE_KEY_KP_9 170
#define	ENGINE_KEY_KP_ADD 171
#define	ENGINE_KEY_KP_DECIMAL 172
#define	ENGINE_KEY_KP_DIVIDE 173
#define	ENGINE_KEY_KP_ENTER 174
#define	ENGINE_KEY_KP_EQUAL 175
#define	ENGINE_KEY_KP_MULTIPLY 176
#define	ENGINE_KEY_KP_SUBTRACT 177
#define	ENGINE_KEY_LEFT 178
#define	ENGINE_KEY_MENU 179
#define	ENGINE_KEY_NUM_LOCK 180
#define	ENGINE_KEY_PAGE_DOWN 181
#define	ENGINE_KEY_PAGE_UP 182
#define	ENGINE_KEY_PAUSE 183
#define	ENGINE_KEY_PRINT_SCREEN 184
#define	ENGINE_KEY_RIGHT 185
#define	ENGINE_KEY_SCROLL_LOCK 186
#define	ENGINE_KEY_SHIFT 187
#define	ENGINE_KEY_SUPER 188
#define	ENGINE_KEY_UP 189



extern float engine_mouse_x;
extern float engine_mouse_y;
extern unsigned int engine_flags;
extern unsigned int engine_enabled_flags;
extern unsigned int engine_disabled_flags;
extern unsigned int engine_width;
extern unsigned int engine_height;
extern float engine_aspect_ratio;
extern _Bool engine_mouse_on_screen;
extern unsigned long long int engine_key_states[4];
extern unsigned long long int engine_enabled_key_states[4];
extern unsigned long long int engine_disabled_key_states[4];



static inline _Bool engine_is_key_pressed(unsigned char key){
	return !!(engine_key_states[key>>6]&(1ull<<(key&63)));
}



static inline _Bool engine_is_key_enabled(unsigned char key){
	return !!(engine_enabled_key_states[key>>6]&(1ull<<(key&63)));
}



static inline _Bool engine_is_key_disabled(unsigned char key){
	return !!(engine_disabled_key_states[key>>6]&(1ull<<(key&63)));
}



void engine_init(void);



void engine_run(void);



#endif
