/*
Copyright (C) 2014-2015 Tamas Hamor

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "ContentManagement.h"
#include <ppltasks.h>

using namespace concurrency;
using namespace std;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Storage;
using namespace Windows::UI::Popups;

extern "C" void EnsureContentAvailable()
{
	(ref new ContentManagement())->ObtainGameContent();
}

extern "C" const wchar_t * GetDataBasePathWinRT()
{
	return ApplicationData::Current->LocalFolder->Path->Data();
}

void ContentManagement::OnActivated(CoreApplicationView^, IActivatedEventArgs ^args)
{
	if (args->Kind == ActivationKind::File)
	{
		auto e = dynamic_cast<FileActivatedEventArgs^>(args);
		if (e != nullptr)
		{
			auto targetFolder = ApplicationData::Current->LocalFolder;
			vector<task<StorageFile^>> tasks;
			for (size_t i = 0; i < e->Files->Size; i++)
			{
				auto file = dynamic_cast<IStorageFile^>(e->Files->GetAt(i));
				if (file != nullptr)
				{
					tasks.push_back(create_task(file->CopyAsync(targetFolder, file->Name, NameCollisionOption::ReplaceExisting)));
				}
			}
			when_all(begin(tasks), end(tasks)).then([this](vector<StorageFile^>)
			{
				isCopyFinished = true;
			});
		}
	}
}

bool ContentManagement::FilesFound()
{
	struct _stat buf;
	const vector<wstring> files({
#if SHAREWARE
		L"HUNTBGIN.RTC", L"HUNTBGIN.RTL", L"HUNTBGIN.WAD",
#else
		L"DARKWAR.RTC", L"DARKWAR.RTL", L"DARKWAR.WAD",
#endif
		L"REMOTE1.RTS" });

	for (const auto &file : files)
	{
		auto path = ApplicationData::Current->LocalFolder->Path->Data() + (L"\\" + file);
		if (_wstat(path.c_str(), &buf) == -1)
		{
			return false;
		}
	}
	return true;
}

void ContentManagement::ObtainGameContent()
{
	EventRegistrationToken activatedEventRegistrationToken;
	while (!FilesFound())
	{
		if (!isCopyFinished)
		{
			activatedEventRegistrationToken = CoreApplication::MainView->Activated += ref new TypedEventHandler<CoreApplicationView ^, IActivatedEventArgs ^>(this, &ContentManagement::OnActivated);
			auto msgBox = ref new MessageDialog("Please install game content by clicking on the .WAD, .RTS, .RTL, .RTC files");
			msgBox->ShowAsync();
		}
		isCopyFinished = false;
		while (!isCopyFinished)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
		}
	}
	if (isCopyFinished)
	{
		CoreApplication::MainView->Activated -= activatedEventRegistrationToken;
	}
}
