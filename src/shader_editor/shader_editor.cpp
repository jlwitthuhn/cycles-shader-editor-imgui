#include "shader_editor.h"

#include <string>

#include "shader_editor_impl.h"

cse::ShaderGraphEditor::ShaderGraphEditor() : impl{ std::make_unique<ShaderGraphEditorImpl>() }
{
}

cse::ShaderGraphEditor::~ShaderGraphEditor()
{
}

bool cse::ShaderGraphEditor::running() const
{
	return impl->running();
}

bool cse::ShaderGraphEditor::open_window()
{
	return impl->open_window();
}

void cse::ShaderGraphEditor::wait()
{
	impl->wait();
}

bool cse::ShaderGraphEditor::has_new_data()
{
	return impl->has_new_data();
}

std::string cse::ShaderGraphEditor::get_serialized_graph()
{
	return impl->get_serialized_graph();
}

void cse::ShaderGraphEditor::force_close()
{
	impl->force_close();
}
