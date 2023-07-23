#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <verlet_integration/engine.h>
#include <verlet_integration/renderer.h>
#include <verlet_integration/shader.h>



#define TARGET_FPS 60



static const unsigned char _engine_key_map[]={
	[GLFW_KEY_0]=ENGINE_KEY_0,
	[GLFW_KEY_1]=ENGINE_KEY_1,
	[GLFW_KEY_2]=ENGINE_KEY_2,
	[GLFW_KEY_3]=ENGINE_KEY_3,
	[GLFW_KEY_4]=ENGINE_KEY_4,
	[GLFW_KEY_5]=ENGINE_KEY_5,
	[GLFW_KEY_6]=ENGINE_KEY_6,
	[GLFW_KEY_7]=ENGINE_KEY_7,
	[GLFW_KEY_8]=ENGINE_KEY_8,
	[GLFW_KEY_9]=ENGINE_KEY_9,
	[GLFW_KEY_A]=ENGINE_KEY_A,
	[GLFW_KEY_APOSTROPHE]=ENGINE_KEY_APOSTROPHE,
	[GLFW_KEY_B]=ENGINE_KEY_B,
	[GLFW_KEY_BACKSLASH]=ENGINE_KEY_BACKSLASH,
	[GLFW_KEY_BACKSPACE]=ENGINE_KEY_BACKSPACE,
	[GLFW_KEY_C]=ENGINE_KEY_C,
	[GLFW_KEY_CAPS_LOCK]=ENGINE_KEY_CAPS_LOCK,
	[GLFW_KEY_COMMA]=ENGINE_KEY_COMMA,
	[GLFW_KEY_D]=ENGINE_KEY_D,
	[GLFW_KEY_DELETE]=ENGINE_KEY_DELETE,
	[GLFW_KEY_DOWN]=ENGINE_KEY_DOWN,
	[GLFW_KEY_E]=ENGINE_KEY_E,
	[GLFW_KEY_END]=ENGINE_KEY_END,
	[GLFW_KEY_ENTER]=ENGINE_KEY_ENTER,
	[GLFW_KEY_EQUAL]=ENGINE_KEY_EQUAL,
	[GLFW_KEY_ESCAPE]=ENGINE_KEY_ESCAPE,
	[GLFW_KEY_F10]=ENGINE_KEY_F10,
	[GLFW_KEY_F11]=ENGINE_KEY_F11,
	[GLFW_KEY_F12]=ENGINE_KEY_F12,
	[GLFW_KEY_F13]=ENGINE_KEY_F13,
	[GLFW_KEY_F14]=ENGINE_KEY_F14,
	[GLFW_KEY_F15]=ENGINE_KEY_F15,
	[GLFW_KEY_F16]=ENGINE_KEY_F16,
	[GLFW_KEY_F17]=ENGINE_KEY_F17,
	[GLFW_KEY_F18]=ENGINE_KEY_F18,
	[GLFW_KEY_F19]=ENGINE_KEY_F19,
	[GLFW_KEY_F1]=ENGINE_KEY_F1,
	[GLFW_KEY_F20]=ENGINE_KEY_F20,
	[GLFW_KEY_F21]=ENGINE_KEY_F21,
	[GLFW_KEY_F22]=ENGINE_KEY_F22,
	[GLFW_KEY_F23]=ENGINE_KEY_F23,
	[GLFW_KEY_F24]=ENGINE_KEY_F24,
	[GLFW_KEY_F25]=ENGINE_KEY_F25,
	[GLFW_KEY_F2]=ENGINE_KEY_F2,
	[GLFW_KEY_F3]=ENGINE_KEY_F3,
	[GLFW_KEY_F4]=ENGINE_KEY_F4,
	[GLFW_KEY_F5]=ENGINE_KEY_F5,
	[GLFW_KEY_F6]=ENGINE_KEY_F6,
	[GLFW_KEY_F7]=ENGINE_KEY_F7,
	[GLFW_KEY_F8]=ENGINE_KEY_F8,
	[GLFW_KEY_F9]=ENGINE_KEY_F9,
	[GLFW_KEY_F]=ENGINE_KEY_F,
	[GLFW_KEY_G]=ENGINE_KEY_G,
	[GLFW_KEY_GRAVE_ACCENT]=ENGINE_KEY_GRAVE,
	[GLFW_KEY_H]=ENGINE_KEY_H,
	[GLFW_KEY_HOME]=ENGINE_KEY_HOME,
	[GLFW_KEY_I]=ENGINE_KEY_I,
	[GLFW_KEY_INSERT]=ENGINE_KEY_INSERT,
	[GLFW_KEY_J]=ENGINE_KEY_J,
	[GLFW_KEY_K]=ENGINE_KEY_K,
	[GLFW_KEY_KP_0]=ENGINE_KEY_KP_0,
	[GLFW_KEY_KP_1]=ENGINE_KEY_KP_1,
	[GLFW_KEY_KP_2]=ENGINE_KEY_KP_2,
	[GLFW_KEY_KP_3]=ENGINE_KEY_KP_3,
	[GLFW_KEY_KP_4]=ENGINE_KEY_KP_4,
	[GLFW_KEY_KP_5]=ENGINE_KEY_KP_5,
	[GLFW_KEY_KP_6]=ENGINE_KEY_KP_6,
	[GLFW_KEY_KP_7]=ENGINE_KEY_KP_7,
	[GLFW_KEY_KP_8]=ENGINE_KEY_KP_8,
	[GLFW_KEY_KP_9]=ENGINE_KEY_KP_9,
	[GLFW_KEY_KP_ADD]=ENGINE_KEY_KP_ADD,
	[GLFW_KEY_KP_DECIMAL]=ENGINE_KEY_KP_DECIMAL,
	[GLFW_KEY_KP_DIVIDE]=ENGINE_KEY_KP_DIVIDE,
	[GLFW_KEY_KP_ENTER]=ENGINE_KEY_KP_ENTER,
	[GLFW_KEY_KP_EQUAL]=ENGINE_KEY_KP_EQUAL,
	[GLFW_KEY_KP_MULTIPLY]=ENGINE_KEY_KP_MULTIPLY,
	[GLFW_KEY_KP_SUBTRACT]=ENGINE_KEY_KP_SUBTRACT,
	[GLFW_KEY_L]=ENGINE_KEY_L,
	[GLFW_KEY_LEFT]=ENGINE_KEY_LEFT,
	[GLFW_KEY_LEFT_ALT]=ENGINE_KEY_ALT,
	[GLFW_KEY_LEFT_BRACKET]=ENGINE_KEY_LEFT_BRACKET,
	[GLFW_KEY_LEFT_CONTROL]=ENGINE_KEY_CTRL,
	[GLFW_KEY_LEFT_SHIFT]=ENGINE_KEY_SHIFT,
	[GLFW_KEY_LEFT_SUPER]=ENGINE_KEY_SUPER,
	[GLFW_KEY_M]=ENGINE_KEY_M,
	[GLFW_KEY_MENU]=ENGINE_KEY_MENU,
	[GLFW_KEY_MINUS]=ENGINE_KEY_MINUS,
	[GLFW_KEY_N]=ENGINE_KEY_N,
	[GLFW_KEY_NUM_LOCK]=ENGINE_KEY_NUM_LOCK,
	[GLFW_KEY_O]=ENGINE_KEY_O,
	[GLFW_KEY_P]=ENGINE_KEY_P,
	[GLFW_KEY_PAGE_DOWN]=ENGINE_KEY_PAGE_DOWN,
	[GLFW_KEY_PAGE_UP]=ENGINE_KEY_PAGE_UP,
	[GLFW_KEY_PAUSE]=ENGINE_KEY_PAUSE,
	[GLFW_KEY_PERIOD]=ENGINE_KEY_PERIOD,
	[GLFW_KEY_PRINT_SCREEN]=ENGINE_KEY_PRINT_SCREEN,
	[GLFW_KEY_Q]=ENGINE_KEY_Q,
	[GLFW_KEY_R]=ENGINE_KEY_R,
	[GLFW_KEY_RIGHT]=ENGINE_KEY_RIGHT,
	[GLFW_KEY_RIGHT_ALT]=ENGINE_KEY_ALT,
	[GLFW_KEY_RIGHT_BRACKET]=ENGINE_KEY_RIGHT_BRACKET,
	[GLFW_KEY_RIGHT_CONTROL]=ENGINE_KEY_CTRL,
	[GLFW_KEY_RIGHT_SHIFT]=ENGINE_KEY_SHIFT,
	[GLFW_KEY_RIGHT_SUPER]=ENGINE_KEY_SUPER,
	[GLFW_KEY_S]=ENGINE_KEY_S,
	[GLFW_KEY_SCROLL_LOCK]=ENGINE_KEY_SCROLL_LOCK,
	[GLFW_KEY_SEMICOLON]=ENGINE_KEY_SEMICOLON,
	[GLFW_KEY_SLASH]=ENGINE_KEY_SLASH,
	[GLFW_KEY_SPACE]=ENGINE_KEY_SPACE,
	[GLFW_KEY_T]=ENGINE_KEY_T,
	[GLFW_KEY_TAB]=ENGINE_KEY_TAB,
	[GLFW_KEY_U]=ENGINE_KEY_U,
	[GLFW_KEY_UP]=ENGINE_KEY_UP,
	[GLFW_KEY_V]=ENGINE_KEY_V,
	[GLFW_KEY_W]=ENGINE_KEY_W,
	[GLFW_KEY_X]=ENGINE_KEY_X,
	[GLFW_KEY_Y]=ENGINE_KEY_Y,
	[GLFW_KEY_Z]=ENGINE_KEY_Z,
};

float engine_mouse_x=0.0f;
float engine_mouse_y=0.0f;
unsigned int engine_flags=0;
unsigned int engine_enabled_flags=0;
unsigned int engine_disabled_flags=0;
unsigned int engine_width=0;
unsigned int engine_height=0;
float engine_aspect_ratio=1.0f;
_Bool engine_mouse_on_screen=0;
unsigned long long int engine_key_states[4]={0,0,0,0};
unsigned long long int engine_enabled_key_states[4]={0,0,0,0};
unsigned long long int engine_disabled_key_states[4]={0,0,0,0};

static GLFWwindow* _engine_window;
static unsigned int _engine_last_flags=0;
static unsigned long long int _engine_last_key_states[4]={0,0,0,0};



static void _window_key_callback(GLFWwindow* window,int key,int scancode,int action,int mods){
	if (key==GLFW_KEY_UNKNOWN){
		return;
	}
	key=_engine_key_map[key];
	unsigned long long int mask=1ull<<(key&63);
	if (action==GLFW_RELEASE){
		engine_key_states[key>>6]&=~mask;
	}
	else{
		engine_key_states[key>>6]|=mask;
	}
}



static void _window_resize_callback(GLFWwindow* window,int width,int height){
	glViewport(0,0,width,height);
	engine_width=width;
	engine_height=height;
	engine_aspect_ratio=((float)engine_width)/engine_height;
	engine_flags|=ENGINE_FLAG_SIZE;
}



static void _window_cursor_position_callback(GLFWwindow* window,double x,double y){
	engine_mouse_x=x;
	engine_mouse_y=y;
}



static void _window_cursor_enter_callback(GLFWwindow* window,int enter){
	engine_mouse_on_screen=!!enter;
}



static void _window_mouse_button_callback(GLFWwindow* window,int button,int action,int mods){
	unsigned int flag=0;
	switch (button){
		case GLFW_MOUSE_BUTTON_LEFT:
			flag=ENGINE_FLAG_LEFT;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			flag=ENGINE_FLAG_MIDDLE;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			flag=ENGINE_FLAG_RIGHT;
			break;
		default:
			return;
	}
	if (action==GLFW_PRESS){
		engine_flags|=flag;
	}
	else{
		engine_flags&=~flag;
	}
}



static void _window_scroll_callback(GLFWwindow* window,double dx,double dy){
	if (!dy){
		return;
	}
	engine_flags|=(dy>0?ENGINE_FLAG_SCROLL_UP:ENGINE_FLAG_SCROLL_DOWN);
}



void engine_init(void){
	gl_init();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES,2);
	glfwWindowHint(GLFW_RESIZABLE,0);
	_engine_window=glfwCreateWindow(1024,1024,"Verlet Integration",NULL,NULL);
	glfwMakeContextCurrent(_engine_window);
	glfwSetKeyCallback(_engine_window,_window_key_callback);
	glfwSetFramebufferSizeCallback(_engine_window,_window_resize_callback);
	glfwSetCursorPosCallback(_engine_window,_window_cursor_position_callback);
	glfwSetCursorEnterCallback(_engine_window,_window_cursor_enter_callback);
	glfwSetMouseButtonCallback(_engine_window,_window_mouse_button_callback);
	glfwSetScrollCallback(_engine_window,_window_scroll_callback);
	_window_resize_callback(_engine_window,1024,1024);
	shader_init();
	renderer_init();
}



void engine_run(void){
	float last_time=(float)glfwGetTime();
	while (!glfwWindowShouldClose(_engine_window)){
		if (glfwGetKey(_engine_window,GLFW_KEY_ESCAPE)==GLFW_PRESS&&glfwGetKey(_engine_window,GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS){
			glfwSetWindowShouldClose(_engine_window,1);
		}
		float time=glfwGetTime();
		unsigned int toggled_flags=engine_flags^_engine_last_flags;
		engine_enabled_flags=toggled_flags&engine_flags;
		engine_disabled_flags=toggled_flags&(~engine_flags);
		for (unsigned int i=0;i<4;i++){
			unsigned long long int toggled_keys=engine_key_states[i]^_engine_last_key_states[i];
			engine_enabled_key_states[i]=toggled_keys&engine_key_states[i];
			engine_disabled_key_states[i]=toggled_keys&(~engine_key_states[i]);
		}
		renderer_tick(time-last_time);
		engine_flags&=~(ENGINE_FLAG_SCROLL_UP|ENGINE_FLAG_SCROLL_DOWN|ENGINE_FLAG_SIZE);
		_engine_last_flags=engine_flags;
		for (unsigned int i=0;i<4;i++){
			_engine_last_key_states[i]=engine_key_states[i];
		}
		glfwSwapBuffers(_engine_window);
		float delta_time=1/TARGET_FPS-time+last_time;
		last_time=time;
		if (delta_time>0){
			usleep((unsigned int)(delta_time*1e6f));
		}
		glfwPollEvents();
	}
	renderer_deinit();
	shader_deinit();
	glfwTerminate();
}
