#include "../Memory.h"
#include "ActiveWindow.h"
#include "../kernel/Kernel.h"
using namespace core;

bool ActiveWindow::IsActive()
{
	return Kernel::GetActiveWindow()->IsActive();
}

void ActiveWindow::SetSize(const Size& size)
{
	Kernel::GetActiveWindow()->SetSize(size);
}

const Size& ActiveWindow::GetSize()
{
	return Kernel::GetActiveWindow()->GetSize();
}

void ActiveWindow::SetTitle(const std::string& title)
{
	Kernel::GetActiveWindow()->SetTitle(title);
}

const std::string& ActiveWindow::GetTitle()
{
	return Kernel::GetActiveWindow()->GetTitle();
}

void ActiveWindow::AddWindowClosedListener(WindowClosedListener* listener)
{
	Kernel::GetActiveWindow()->AddWindowClosedListener(listener);
}

void ActiveWindow::RemoveWindowClosedListener(WindowClosedListener* listener)
{
	Kernel::GetActiveWindow()->RemoveWindowClosedListener(listener);
}

void ActiveWindow::AddWindowResizedListener(WindowResizedListener* listener)
{
	Kernel::GetActiveWindow()->AddWindowResizedListener(listener);
}

void ActiveWindow::RemoveWindowResizedListener(WindowResizedListener* listener)
{
	Kernel::GetActiveWindow()->RemoveWindowResizedListener(listener);
}

void ActiveWindow::SetFullscreen(bool fullscreen)
{
	Kernel::GetActiveWindow()->SetFullscreen(fullscreen);
}

void ActiveWindow::Alert(const std::string& title, const std::string& text, AlertType::Enum type)
{
	Kernel::GetActiveWindow()->Alert(title, text, type);
}

void ActiveWindow::Alert(const std::string& title, const Exception& e, AlertType::Enum type)
{
	Kernel::GetActiveWindow()->Alert(title, e, type);
}
