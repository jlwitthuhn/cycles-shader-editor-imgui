#include <shader_editor/shader_editor.h>

int main()
{
	cse::ShaderGraphEditor editor;
	editor.open_window();
	editor.wait();
	return 0;
}
