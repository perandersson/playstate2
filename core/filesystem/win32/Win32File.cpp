#include "../../Memory.h"
#include "Win32File.h"
#include "Win32FileSystem.h"

using namespace core;

Win32File::Win32File(const Win32FileSystem& fileSystem, const std::string& path)
: mFileSystem(fileSystem), mPath(path), mFileSize(0), mFileHandle(NULL)
{
}

Win32File::Win32File(const Win32FileSystem& fileSystem, HANDLE handle, const std::string& path)
:  mFileSystem(fileSystem), mPath(path), mFileSize(0), mFileHandle(handle)
{
	LARGE_INTEGER size;
	GetFileSizeEx(mFileHandle, &size);
	mFileSize = size.LowPart;
}

Win32File::~Win32File()
{
	if(mFileHandle != NULL) {
		CloseHandle(mFileHandle);
		mFileHandle = NULL;
	}

	mPath.clear();
}

std::istringstream Win32File::Read() const
{
	std::istringstream stream;
	if(Exists()) {
		byte* buffer = new byte[mFileSize + 1];
		ReadBytes(buffer, mFileSize);
		buffer[mFileSize] = 0;
		stream = std::istringstream(std::string((char*)buffer));
		delete[] buffer;
	}

	return stream;
}

std::shared_ptr<IFile> Win32File::OpenFile(const std::string& path) const
{
	std::string fullPath = path;
	if(Win32FileSystem::IsRelative(path)) {
		std::string::size_type pos = mPath.find_last_of('/');
		std::string pathToDir = mPath.substr(0, pos + 1);
		fullPath = pathToDir + path;
	}

	return mFileSystem.OpenFile(fullPath);
}

size_t Win32File::GetFileSize() const
{
	return mFileSize;
}

size_t Win32File::ReadBytes(byte* buffer, size_t offset, size_t bufferSize) const
{
	if(!Exists())
		return 0;

	DWORD bytesRead = 0;
	OVERLAPPED ol = {0};
	ol.Offset = offset;
	//ol.OffsetHigh = 0; Assume smaller than 4 gb
	if(ReadFile(mFileHandle, buffer, bufferSize, &bytesRead, &ol))
		return (uint64)bytesRead;

	return 0;
}

size_t Win32File::ReadBytes(byte* buffer, size_t bufferSize) const
{
	return ReadBytes(buffer, 0, bufferSize);
}

std::shared_ptr<byte> Win32File::ReadBytes() const
{
	const size_t fileSize = GetFileSize();
	std::shared_ptr<byte> bytes(new byte[fileSize], TDeleteArray<byte>());
	ReadBytes(bytes.get(), fileSize);
	return bytes;
}

void Win32File::WriteBytes(byte* bytes, size_t size)
{
	assert_not_null(bytes);
	assert(size > 0);
	if (!Exists()) {
		mFileHandle = CreateFile(mPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (mFileHandle == INVALID_HANDLE_VALUE) {
			THROW_EXCEPTION(FileException, "Could not create file: '%s'", mPath.c_str());
		}
	}

	DWORD bytesWritten = 0;
	BOOL errorFlag = WriteFile(mFileHandle, (void*)bytes, size, &bytesWritten, NULL);
	if (errorFlag == FALSE) {
		THROW_EXCEPTION(FileException, "Could not write file: '%s'", mPath.c_str());
	}

	if (bytesWritten != size) {
		THROW_EXCEPTION(FileException, "Failed to write the entire file: '%s'", mPath.c_str());
	}
}

const std::string& Win32File::GetAbsolutePath() const
{
	return mPath;
}

std::shared_ptr<IDirectory> Win32File::GetDirectory() const
{
	std::string::size_type pos = mPath.find_last_of(L'/');
	std::string pathToDir = mPath.substr(0, pos);
	return mFileSystem.OpenDirectory(pathToDir);
}

bool Win32File::Exists() const
{
	return mFileHandle != NULL;
}
