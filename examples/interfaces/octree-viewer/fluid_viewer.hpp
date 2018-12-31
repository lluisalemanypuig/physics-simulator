#pragma once

void __fv_exit_func();
void __fv_initGL(int argc, char *argv[]);
void __fv_refresh();
void __fv_timed_refresh(int value);
void __fv_reshape(int w, int h);
void __fv_mouse_click_event(int button, int state, int x, int y);
void __fv_mouse_movement(int x, int y);
void __fv_mouse_drag_event(int x, int y);
void __fv_special_keys(int key, int x, int y);
void __fv_keyboard_event(unsigned char c, int x, int y);

void __fv_interface(int argc, char *argv[]);
