#pragma once

namespace core
{
	class IFile;

	/*!
		\brief 
	*/
	class FileChangeAction
	{
	public:
		enum Enum {
			ADDED = 0,
			DELETED,
			MODIFIED
		};
	};

	/*!
		\brief Implement this interface so that you can receive notifications whenever files change

		Example:
		{@code
			using namespace core;
			class LuaFileChangedListener : public IFileChangedListener {
			public:
				virtual void OnFileChanged(const IFile* file, FileChangeAction::Enum action) {
					if(action == FileChangeAction::MODIFIER) {
						// Update classes in the running executable
					}
				}
			};

			// Listen for changes on the file system for all files ending with ".lua" 
			FileSystem::AddFileChangedListener(std::regex("[^\\s]+\\.lua$"), new LuaFileChangedListener());
		}
	*/
	class IFileChangedListener
	{
	public:
		virtual ~IFileChangedListener() {}

	public:
		/*!
			\brief Method called whenever something has changed in the filesystem
			
			\param file
						The file that was changed.
			\param action
						Whatever happened to the file in the filesystem.
		*/
		virtual void OnFileChanged(const IFile* file, FileChangeAction::Enum action) = 0;
	};
}

