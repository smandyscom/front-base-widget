#include "mainwindowcommon.h"

MainWindowCommon::MainWindowCommon(QWidget *parent)
	: QMainWindow(parent),
	m_isQSSInitialized(false)
{
	m_watcher = new QFileSystemWatcher(this);
	connect(m_watcher, &QFileSystemWatcher::fileChanged, this, &MainWindowCommon::onReloadQss);

	onLinkQSSFile();
	connect(this, &MainWindowCommon::objectNameChanged, this, &MainWindowCommon::onLinkQSSFile);

}

MainWindowCommon::~MainWindowCommon()
{
}

void MainWindowCommon::onLinkQSSFile()
{
	m_watcher->removePaths(m_watcher->files());
	m_watcher->addPath(QString("%1.qss").arg(objectName()));
}

void MainWindowCommon::onReloadQss()
{
	CommonHelper::setStyle(QString("%1.qss").arg(objectName()), this);
}

void MainWindowCommon::showEvent(QShowEvent* event)
{
	if (!m_isQSSInitialized)
	{
		onReloadQss();
		m_isQSSInitialized = true;
	}

	QMainWindow::showEvent(event);
}