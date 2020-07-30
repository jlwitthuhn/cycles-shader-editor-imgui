#include "platform.h"

#ifdef _WIN32

#include <array>
#include <fstream>
#include <sstream>

#include <ShObjIdl.h>

// Filters for the save dialog
static const COMDLG_FILTERSPEC rg_spec[] =
{
	{ L"Shader File", L"*.shader" },
	{ L"Text File", L"*.txt" },
	{ L"All Files", L"*.*" },
};

bool cse::Platform::save_graph_dialog(const std::string graph)
{
	bool result{ false };

#pragma warning( push )
#pragma warning( disable: 4456 )

	IFileDialog* file_dlg{ nullptr };
	const HRESULT hr{ CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&file_dlg)) };
	if (SUCCEEDED(hr)) {
		file_dlg->SetFileName(L"shader.shader");
		file_dlg->SetFileTypes(3, rg_spec);

		const HRESULT hr{ file_dlg->Show(nullptr) };
		if (SUCCEEDED(hr)) {
			IShellItem* save_item = nullptr;

			const HRESULT hr = file_dlg->GetResult(&save_item);
			if (SUCCEEDED(hr)) {
				std::array<wchar_t, MAX_PATH + 1> wstr_path;
				wstr_path.fill(L'\0');

				wchar_t* wstr_ptr{ wstr_path.data() };
				const HRESULT hr = save_item->GetDisplayName(SIGDN_FILESYSPATH, &wstr_ptr);
				if (SUCCEEDED(hr)) {
					std::ofstream file_stream{ wstr_ptr, std::ofstream::trunc };
					file_stream << graph;
					file_stream.close();
					result = true;
				}

				save_item->Release();
			}
		}
		file_dlg->Release();
	}

#pragma warning( pop )

	return result;
}

boost::optional<std::string> cse::Platform::load_graph_dialog()
{
	boost::optional<std::string> result;

#pragma warning( push )
#pragma warning( disable: 4456 )

	IFileDialog* file_dlg{ nullptr };
	HRESULT hr{ CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&file_dlg)) };

	if (SUCCEEDED(hr)) {
		hr = file_dlg->SetFileTypes(3, rg_spec);
		hr = file_dlg->Show(nullptr);

		if (SUCCEEDED(hr)) {
			IShellItem* shell_item = nullptr;
			hr = file_dlg->GetResult(&shell_item);

			if (SUCCEEDED(hr)) {
				PWSTR file_path;
				hr = shell_item->GetDisplayName(SIGDN_FILESYSPATH, &file_path);

				if (SUCCEEDED(hr)) {
					std::stringstream result_stream;
					std::ifstream input_file(file_path, std::ifstream::in);

					while (true) {
						const int c{ input_file.get() };
						if (c != EOF) {
							result_stream << static_cast<char>(c);
						}
						else {
							break;
						}
					}
					result = result_stream.str();
				}
				shell_item->Release();
			}
		}
		file_dlg->Release();
	}
#pragma warning( pop )

	return result;
}

#else

bool cse::Platform::save_graph_dialog(std::string)
{
	return false;
}

boost::optional<std::string> cse::Platform::load_graph_dialog()
{
	return boost::none;
}

#endif
