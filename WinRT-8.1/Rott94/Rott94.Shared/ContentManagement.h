#pragma once

extern "C" void EnsureContentAvailable();
extern "C" const wchar_t * GetDataBasePathWinRT();

ref class ContentManagement sealed
{
public:
	void ObtainGameContent();
private:
	void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView ^sender, Windows::ApplicationModel::Activation::IActivatedEventArgs ^args);
	bool FilesFound();
	volatile bool isCopyFinished = false;
};

