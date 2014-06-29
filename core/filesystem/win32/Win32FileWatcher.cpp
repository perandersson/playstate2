#include "../../Memory.h"
#include "Win32FileWatcher.h"
#include "Win32File.h"
#include "Win32FileSystem.h"
#include <chrono>
#include <algorithm>

using namespace core;

Win32FileWatcher::Win32FileWatcher(Win32FileSystem* fileSystem, const std::string& rootDirPath)
: mFileSystem(fileSystem), mRootDirHandle(INVALID_HANDLE_VALUE), mRootDirPath(rootDirPath)
{
}

Win32FileWatcher::~Win32FileWatcher()
{
	if (mRootDirHandle != INVALID_HANDLE_VALUE) {
		HANDLE oldHandle = mRootDirHandle;
		mRootDirHandle = INVALID_HANDLE_VALUE;
		CloseHandle(oldHandle);
		mThread.join();
	}

	FileChangedListeners::iterator it = mFileChangedListeners.begin();
	FileChangedListeners::const_iterator end = mFileChangedListeners.end();
	for(; it != end; ++it) {
		delete *it;
	}
	mFileChangedListeners.clear();
}

void Win32FileWatcher::Start()
{
	mRootDirHandle = CreateFile(mRootDirPath.c_str(), GENERIC_READ,
		FILE_LIST_DIRECTORY | FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
	if (mRootDirHandle != INVALID_HANDLE_VALUE) {
		mThread = std::thread(&Win32FileWatcher::RunFromThread, this);
	}
}

void Win32FileWatcher::Stop()
{
	if (mRootDirHandle != INVALID_HANDLE_VALUE) {
		HANDLE oldHandle = mRootDirHandle;
		mRootDirHandle = INVALID_HANDLE_VALUE;
		CloseHandle(oldHandle);
		mThread.join();
	}
}

void Win32FileWatcher::AddListener(const std::regex& regex, IFileChangedListener* listener)
{
	std::lock_guard<std::mutex> lg(mListenersLock);

	Win32FileListener* fileListener = new Win32FileListener();
	fileListener->callback = listener;
	fileListener->regex = regex;
	mFileChangedListeners.push_back(fileListener);
}

void Win32FileWatcher::RemoveListener(IFileChangedListener* listener)
{
	std::lock_guard<std::mutex> lg(mListenersLock);

	FileChangedListeners::iterator it = mFileChangedListeners.begin();
	FileChangedListeners::const_iterator end = mFileChangedListeners.end();
	std::vector<FileChangedListeners::iterator> listenersToRemove;
	for(; it != end; ++it) {
		Win32FileListener* fileListener = (*it);
		if(fileListener->callback == listener) {
			listenersToRemove.push_back(it);
			delete fileListener;
		}
	}
	
	for(unsigned int i = 0; i < listenersToRemove.size(); ++i) {
		mFileChangedListeners.erase(listenersToRemove[i]);
	}
}

void Win32FileWatcher::RunFromThread()
{
	BYTE bytes[32 * 1024];
	DWORD bytesret;
	TCHAR szFile[MAX_PATH];

	std::string prevFile;
	uint32 timestamp = GetTickCount();
	
	while (ReadDirectoryChangesW(mRootDirHandle, bytes, sizeof(bytes),
		TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE, &bytesret, NULL, NULL) == TRUE && mRootDirHandle != INVALID_HANDLE_VALUE) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		PFILE_NOTIFY_INFORMATION pNotify;
		DWORD offset = 0;
		do {
			pNotify = (PFILE_NOTIFY_INFORMATION) &bytes[offset];
			offset += pNotify->NextEntryOffset;

			FileChangeAction::Enum action = FileChangeAction::MODIFIED;
			if(pNotify->Action == FILE_ACTION_ADDED) {
				action = FileChangeAction::ADDED;
			} else if(pNotify->Action == FILE_ACTION_REMOVED) {
				action = FileChangeAction::DELETED;
			}

			int count = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR),
										szFile, MAX_PATH - 1, NULL, NULL);
			szFile[count] = 0;
			std::string fileName = "/";
			fileName += szFile;
			std::replace(fileName.begin(), fileName.end(), '\\', '/');

			// Add code to prevent the same file from being notified multiple times.
			uint32 now = GetTickCount();
			if(prevFile == fileName) {
				if(timestamp + 1000 > now) {
					continue;
				}
			}

			timestamp = now;
			prevFile = fileName;

			std::lock_guard<std::mutex> lg(mListenersLock);
			auto it = mFileChangedListeners.begin();
			auto end = mFileChangedListeners.end();
			for(;it != end; ++it) {
				Win32FileListener* listener = *it;
				if (std::regex_match(fileName, listener->regex)) {
					std::lock_guard<std::mutex> lg(mFileEventsLock);
					Win32FileEvent* fileEvent = new Win32FileEvent();
					fileEvent->action = action;
					fileEvent->callback = listener->callback;
					fileEvent->fileName = fileName;
					mFileEvents.push_back(fileEvent);
				}
			}
		} while(pNotify->NextEntryOffset != 0);
	}
}

void Win32FileWatcher::LookForChanges()
{
	FileEvents events;
	GetAndClearEvents(events);
	if (!events.empty()) {
		FileEvents::size_type size = events.size();
		for (FileEvents::size_type i = 0; i < size; ++i) {
			auto fileEvent = events[i];
			auto file = mFileSystem->OpenFile(fileEvent->fileName);
			fileEvent->callback->OnFileChanged(file.get(), fileEvent->action);
			delete fileEvent;
		}
	}
}

void Win32FileWatcher::GetAndClearEvents(FileEvents& events)
{
	if (mFileEvents.empty())
		return;

	std::lock_guard<std::mutex> lg(mFileEventsLock);

	events = FileEvents(mFileEvents);
	mFileEvents.clear();
}
