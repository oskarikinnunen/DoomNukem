#include "doomnukem.h"
#include "inputhelp.h"
#include "file_io.h"
#include "objects.h"
//	Good resource for remembering bitwise operations:
//			https://stackoverflow.com/a/47990/1725220
//	TODO: move playermode events to separate file
/*place holder for inits*/
void	move(t_game *game);

static int key_events(SDL_Event e, t_game *game)
{
	t_vector2 temp;
	if (e.type == SDL_KEYDOWN)
	{
		if (iskey(e, SDLK_ESCAPE))
			return (game_exit);
		if (iskey(e, SDLK_TAB))
			return (game_switchmode);
		game->keystate |= keyismoveleft(e) << KEYS_LEFTMASK;
		game->keystate |= keyismoveright(e) << KEYS_RIGHTMASK;
		game->keystate |= keyismoveup(e) << KEYS_UPMASK;
		game->keystate |= keyismovedown(e) << KEYS_DOWNMASK;
		game->keystate |= iskey(e, SDLK_SPACE) << KEYS_SPACEMASK;
		game->keystate |= iskey(e, SDLK_LCTRL) << KEYS_CTRLMASK;
		game->keystate |= iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK;
		game->keystate |= iskey(e, SDLK_m) << KEYS_MMASK;
		if (iskey(e, SDLK_m))
		{
			game->cam_mode = !game->cam_mode;
		}
	}
	else if(e.type == SDL_KEYUP)
	{
		game->keystate &= ~(keyismoveleft(e) << KEYS_LEFTMASK);
		game->keystate &= ~(keyismoveright(e) << KEYS_RIGHTMASK);
		game->keystate &= ~(keyismoveup(e) << KEYS_UPMASK);
		game->keystate &= ~(keyismovedown(e) << KEYS_DOWNMASK);
		game->keystate &= ~(iskey(e, SDLK_SPACE) << KEYS_SPACEMASK);
		game->keystate &= ~(iskey(e, SDLK_LCTRL) << KEYS_CTRLMASK);
		game->keystate &= ~(iskey(e, SDLK_LSHIFT) << KEYS_SHIFTMASK);
		game->keystate &= ~(iskey(e, SDLK_m) << KEYS_MMASK);
	}
	return (game_continue);
}

//	Updates mouse delta position
static void updatemouse(t_mouse *mouse)
{
	SDL_GetRelativeMouseState(&mouse->delta.x, &mouse->delta.y);
}

static void updateinput(t_input *input, int keystate, t_mouse m, t_controller *controller)
{
	input->move = vector2_zero();

	input->move.x -= (keystate >> KEYS_LEFTMASK) & 1;
	input->move.x += (keystate >> KEYS_RIGHTMASK) & 1;
	input->move.y += (keystate >> KEYS_DOWNMASK) & 1;
	input->move.y -= (keystate >> KEYS_UPMASK) & 1;
	input->crouch = (keystate >> KEYS_CTRLMASK) & 1;
	input->jump = (keystate >> KEYS_SPACEMASK) & 1;
	input->run = (keystate >> KEYS_SHIFTMASK) & 1;
	input->turn = vector2_mul(point_to_vector2(m.delta), MOUSESPEED);
	input->move.x += controller->leftanalog.x;
	input->move.y += controller->leftanalog.y;
	input->crouch += controller->b;
	input->jump += controller->a;
	input->turn = vector2_add(input->turn, vector2_mul(controller->rightanalog, CONTROLLER_SENS));
	input->aim = m.heldstate >> MOUSE_RIGHT & 1;
	input->aim += controller->lefttrigger;
	input->shoot = m.heldstate >> MOUSE_LEFT & 1;
	input->shoot += controller->righttrigger;
}

/*check for keyboard/mouse/joystick input*/
static int handleinput(t_game *game)
{
	static SDL_Event	e;
	t_gamereturn		gr;

	updatemouse(&game->mouse);
	while (SDL_PollEvent(&e))
	{
		mouse_event(e, &game->mouse);
		gr = key_events(e, game);
		if (gr != game_continue)
			return (gr);
		gr = controller_events(e, game);
		if (gr != game_continue)
			return (gr);
	}
	updateinput(&game->input, game->keystate, game->mouse, &game->controller[0]);
	return(game_continue);
}

static void update_render(t_render *render, t_player *player)
{
	t_vector3	right;
	render->lookdir = player->lookdir;
	render->position = player->transform.location;
	//player->gun->transform.location
	//render.
}

static void player_init(t_player *player, t_sdlcontext sdl)
{
	player->transform.location = (t_vector3) {500.0f, 500.0f, 500.0f};
	player->transform.rotation = (t_vector3){-RAD90, -RAD90 * 0.99f, 0.0f};
	player->jump.framerate = 30;
	player->jump.lastframe = 15;
	player->gun = ft_memalloc(sizeof(t_gun));
	player->gun->entity.obj = get_object_by_name(sdl, "Pistol_1.obj");
	player->gun->entity.transform.scale = vector3_one();
	player->gun->entity.transform.parent = &player->transform;
	player->gun->entity.transform.rotation = (t_vector3){0.0f, 0.0f, 0.0f};
	player->gun->holsterpos = (t_vector3){15.0f, 18.0f, -18.0f};
	player->gun->aimpos = (t_vector3){0.0f, 15.0f, -7.0f};
	player->gun->entity.transform.location = player->gun->holsterpos;
}

void	fill_audio(void *udata, Uint8 *stream, int len, Uint32 audio_len, Uint8 *audio_pos)
{
	/* Only play if we have data left */
	if (audio_len == 0)
		return ;
	
	/* Mix as much data as possible */
	if (len > audio_len)
		len = audio_len;
	else
		len = len;
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

void	test_sound(t_sdlcontext *sdl)
{
	SDL_AudioSpec	wanted;
	SDL_AudioSpec	*test;
	extern void	fill_audio(void *udata, Uint8 *strem, int len, Uint32 audio_len, Uint8 *audio_pos);

	SDL_memset(&wanted, 0, sizeof(wanted)); /* or SDL_zero(wanted) */
	wanted.freq = 22050;
	wanted.format = AUDIO_S16;
	wanted.channels = 2;	// 1 = mono, 2 = stereo 
	wanted.samples = 1024;	// good low-latency value for callback
	wanted.callback = fill_audio;
	
	//With SDL >= 2.0.4 you can choose to avoid callbacks and use SDL_QueueAudio() instead, if you like. Just open your audio device with a NULL callback.
	//want.callback = NULL;
	//SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	//if (dev == 0)
	//	error_log(EC_SDL_OPENAUDIODEVICE);

	if (SDL_OpenAudio(&wanted, NULL) < 0)
		error_log(EC_SDL_OPENAUDIODEVICE);



	static Uint8 *audio_chunk;
	static Uint32 audio_len;
	static Uint8 *audio_pos;

	/* The audio function callback takes the following parameters:
       stream:  A pointer to the audio buffer to be filled
       len:     The length (in bytes) of the audio buffer
    */

	/* Load the audio data ... */

	test = SDL_LoadWAV("assets/audio/cloud-10_gunshot.wav", &wanted, audio_chunk, audio_len);

	audio_pos = audio_chunk;

	/* Let the callback function play the audio chunk */
    //SDL_PauseAudio(0);

    /* Do some processing */

    ;;;;;

    /* Wait for sound to complete */
    while ( audio_len > 0 ) {
        SDL_Delay(100);         /* Sleep 1/10 second */
    }
    SDL_CloseAudio();

}




/*main game loop*/
static int gameloop(t_sdlcontext sdl, t_game game)
{
	t_gamereturn	gr;
	t_render		render;

	//alloc_image(&pgraph.image, PERFGRAPH_SAMPLES + 1, PERFGRAPH_SAMPLES + 1);
	gr = game_continue;
	render = init_render(sdl);
	game.world = load_world("world1", sdl);
	player_init(&game.player, sdl);
	game.player.transform.location = (t_vector3) {500.0f, 500.0f, 500.0f};
	game.player.transform.rotation = (t_vector3){-RAD90, -RAD90 * 0.99f, 0.0f}; //TODO: implement transform for player
	game.player.transform.scale = vector3_one();
	initialize_controllers(&game);
	while (gr == game_continue)
	{
		update_deltatime(&game.clock);
		update_deltatime(&game.world.clock);
		gr = handleinput(&game);
		moveplayer(&game);
		update_render(&render, &game.player);
		screen_blank(sdl); //Combine with render_start?
		render_start(&render);
		update_world3d(sdl, &game.world, &render);
		print_text(&sdl, "PLAYMODE", (t_point){5, 5});
		/*game.player.gun->transform.location = vector3_add(game.player.position, (t_vector3){.z = -25.5f});
		game.player.gun->transform.rotation.x = game.player.angle.x + ft_degtorad(100.0f);*/
		render_entity(sdl, render, &game.player.gun->entity);
		//DRAWPERFGRAPH
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
		//gr = game_switchmode;

		if (game.input.shoot)
			play_sound();

	}
	free_render(render);
	if (gr == game_exit)
		quit_game(&sdl);
	return(gr); // for now
}

/*setup and call gameloop*/
int playmode(t_sdlcontext sdl)
{
	t_game			game;
	t_gamereturn	gr;

	bzero(&game, sizeof(t_game));
	//Locks mouse
	if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
		error_log(EC_SDL_SETRELATIVEMOUSEMODE);
	//Do game loop until exit or error
	gr = gameloop(sdl, game);
	//Unlocks mouse
	if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0)
		error_log(EC_SDL_SETRELATIVEMOUSEMODE);
	return (gr);
}
