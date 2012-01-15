#include "draw_login.h"

#include "globals.h"
#include "resources/prepared_graphics.h"
#include "sdl_user.h"
#include "widgets/sdl_animate_button.h"
#include "widgets/sdl_input_box.h"

login_ptr::login_ptr(draw_login *bla)
{
	ref = bla;
}

void login_ptr::go()
{
	ref->login();
}

quit_ptr::quit_ptr(draw_login *bla)
{
	ref = bla;
}

void quit_ptr::go()
{
	ref->quit();
}

draw_login::draw_login(sdl_user *self)
	: sdl_drawmode(self)
{	
	//273 - 300 is the lantern animation (img)
	owner->game_music.change_music("sound/music0.mp3");
	pg = new prepared_graphics;
	pg->num_pg = 1;
	pg->pg = new prepared_graphic[1];
	
	pg->pg[0].surf = get_png_image(814, owner->game);
	pg->pg[0].mask = NULL;
	pg->pg[0].position = NULL;
	pg->pg[0].cleanup = false;
	pg->ready = true;
	
	num_widgets = 7;
	widgets = new sdl_widget*[num_widgets];
	
	widgets[0] = new sdl_widget(59, 0x1a9, 0x138, owner->game);
	widgets[1] = new sdl_input_box(12, 0x1fb, 0x14a, owner->game);
	widgets[1]->set_key_focus(true);
	widgets[1]->cursor_on();
	widget_key_focus = 1;
	widgets[2] = new sdl_input_box(13, 0x1fb, 0x160, owner->game);
	widgets[2]->set_key_focus(true);
	widgets[3] = new sdl_plain_button(53, 0x213, 0x183, owner->game, (funcptr*)new login_ptr(this));
	widgets[3]->set_key_focus(true);
	widgets[4] = new sdl_plain_button(65, 0x213, 0x195, owner->game, 0);
	widgets[4]->set_key_focus(true);
	widgets[5] = new sdl_plain_button(55, 0x213, 0x1a8, owner->game, 0);
	widgets[5]->set_key_focus(true);
	widgets[6] = new sdl_plain_button(57, 0x213, 0x1c2, owner->game, (funcptr*)new quit_ptr(this));
	widgets[6]->set_key_focus(true);
//	widgets[7] = new sdl_widget(814, 0x1a, 0x3b, graphx);
		//type 1, null("intro"), px=0xcf, py=0x11a
}

draw_login::~draw_login()
{
}

bool draw_login::mouse_leave()
{
	return false;
}

void draw_login::login()
{
	owner->login();
	//send login packet with username and password
	//clear username and password information
}

void draw_login::quit()
{
	owner->quit_client();
}

void draw_login::mouse_click(SDL_MouseButtonEvent *here)
{
	if (num_widgets > 0)
	{
		int index = get_widget(here->x, here->y);
		if (index != -1)
		{
			widgets[index]->mouse_click(here);
		}
	}
}

void draw_login::mouse_to(SDL_MouseMotionEvent *to)
{
	int which = get_widget(to->x, to->y);
	if (which >= 0)
	{
		widgets[which]->mouse_to(to);
	}
}

void draw_login::mouse_from(SDL_MouseMotionEvent *from)
{
	int which = get_widget(from->x, from->y);
	if (which >= 0)
	{
		widgets[which]->mouse_from(from);
	}
}

void draw_login::mouse_move(SDL_MouseMotionEvent *from, SDL_MouseMotionEvent *to)
{	//TODO: handle click and drag movable widgets
	if (num_widgets > 0)
	{
		int from_w = get_widget(from->x, from->y);
		int to_w = get_widget(to->x, to->y);
		if ((from_w != -1) && (to_w != -1))
		{
			if (from_w != to_w)
			{	//only send events if the widgets are different
				mouse_from(from);
				mouse_to(to);
			}
		}
		if ((from_w == -1) && (to_w != -1))
		{
			mouse_to(to);
		}
		if ((from_w != -1) && (to_w == -1))
		{
			mouse_from(from);
		}
	}
}
