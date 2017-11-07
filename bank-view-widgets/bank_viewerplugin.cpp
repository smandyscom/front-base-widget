#include "bank_viewer.h"
#include "bank_viewerplugin.h"

#include <QtPlugin>

bank_viewerPlugin::bank_viewerPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void bank_viewerPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool bank_viewerPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *bank_viewerPlugin::createWidget(QWidget *parent)
{
    return new bank_viewer(parent);
}

QString bank_viewerPlugin::name() const
{
    return QLatin1String("bank_viewer");
}

QString bank_viewerPlugin::group() const
{
    return QLatin1String("");
}

QIcon bank_viewerPlugin::icon() const
{
    return QIcon();
}

QString bank_viewerPlugin::toolTip() const
{
    return QLatin1String("");
}

QString bank_viewerPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool bank_viewerPlugin::isContainer() const
{
    return false;
}

QString bank_viewerPlugin::domXml() const
{
    return QLatin1String("<widget class=\"bank_viewer\" name=\"bank_viewer\">\n</widget>\n");
}

QString bank_viewerPlugin::includeFile() const
{
    return QLatin1String("bank_viewer.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(bank_viewerplugin, bank_viewerPlugin)
#endif // QT_VERSION < 0x050000
