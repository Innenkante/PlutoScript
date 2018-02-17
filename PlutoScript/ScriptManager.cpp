#include "stdafx.h"
#include <filesystem>

std::vector<Script> ScriptManager::Scripts;

namespace ScriptManager
{
	namespace Internal
	{
		std::experimental::filesystem::path GetScriptFiles()
		{
			if (std::experimental::filesystem::exists("PlutoScript"))
				return std::experimental::filesystem::path("PlutoScript");

			std::experimental::filesystem::create_directory("PlutoScript");
			return std::experimental::filesystem::path("PlutoScript");
		}

		void LoadScripts()
		{
			auto scripts = GetScriptFiles();

			for (auto &script : std::experimental::filesystem::directory_iterator(scripts))
			{
				if (script.path().extension() == ".dll")
				{
					Scripts.push_back(Script(script.path().filename().string(), LoadLibrary(script.path().c_str())));
				}
			}
		}

		void GetScriptEntryPoints()
		{
			for (auto &script : Scripts)
			{
				script.Entry = reinterpret_cast<Script::EntryPoint>(GetProcAddress(script.Module, "Entry"));
			}
		}

		void ExecuteScripts()
		{
			for (auto &script : Scripts)
			{
				MessageBoxA(NULL, (std::string("Executes ") + script.Name).c_str(), "", 0);

				if (script.Entry == nullptr)
				{
					MessageBoxA(NULL, "Could not find kanker entry point", "", 0);
					return;
				}
				script.Execute();
			}
		}
	}

	void Initialize()
	{
		Internal::LoadScripts();
		Internal::GetScriptEntryPoints();
		Internal::ExecuteScripts();
	}
}