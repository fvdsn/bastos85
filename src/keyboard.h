#ifndef __2D_KEYBOARD_H__
#define __2D_KEYBOARD_H__
/**
 * Makes the key pressed. It will start to be pressed
 * 'next frame' and will be pressed for at least one frame
 * @param key : the ascii code of the pressed key
 */
void key_down(unsigned char key);
/**
 * Makes the key unpressed as soon as possible, starting next
 * frame.
 */
void key_up(unsigned char key);
/**
 * Gets the keyboard status for this key this frame
 */
int  key_pressed(unsigned char key);
/**
 * Makes the keyboard status update for the current frame
 */
void keyboard_frame(void);
#endif
