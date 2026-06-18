#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

struct GLFWwindow;

void UI_Init(struct GLFWwindow *window);
void UI_NewFrame(void);
void UI_Render(void);
void UI_Shutdown(void);

void UI_Text(const char *fmt, ...);
int UI_Button(const char *text);
int UI_BeginWindow(const char *name);
void UI_EndWindow(void);
// Draw a minimal overlay HUD. Returns non-zero if the "Reset object 1" button was pressed.
int UI_DrawOverlay(int wasHit);

#ifdef __cplusplus
}
#endif

#endif // UI_H