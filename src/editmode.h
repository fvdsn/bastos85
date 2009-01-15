#ifndef __2D_EDIT_MODE_H__
#define __2D_EDIT_MODE_H__

typedef enum command_e {
	GRAB,
	ROTATE,
	SCALE,
	DUPLICATE,
	DELETE,
	SELECT,
	SELECT_ALL,
	SELECT_ZONE
}command_t;

void edit_mode_init(world_t*w);
void edit_mode_toggle(void);
void edit_command_begin(command_t c);
void edit_command_end(void);
void edit_mode_draw(void);

#endif

