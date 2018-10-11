#pragma once

#include <qmainwindow.h>

#include <qfilesystemwatcher.h>
#include <commonhelper.h>

class MainWindowCommon : public QMainWindow
{
	Q_OBJECT

public:
	MainWindowCommon(QWidget *parent);
	~MainWindowCommon();

	void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

protected slots:
	void onLinkQSSFile();
	void onReloadQss();
private:
	QFileSystemWatcher* m_watcher;
	bool m_isQSSInitialized;
};
