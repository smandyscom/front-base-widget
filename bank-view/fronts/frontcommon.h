#ifndef FRONTCOMMON_H
#define FRONTCOMMON_H

#include <QtWidgets>
#include <QtEvents>
#include <qfilesystemwatcher.h>

#include <commonhelper.h>
//#include <controllerbase.h>

#include <propertyportcommon.h>

class FrontCommon : public QWidget
{
    Q_OBJECT
public:
	enum CommandProperty
	{
		PROP_BLINK = 0xeeff10,
	};
	Q_ENUM(CommandProperty)
    //!
    //! \brief FrontCommon
    //! \param parent
    //! Defualt constructor , able to use "Promote" on QtUi designer
    explicit FrontCommon(QWidget *parent = nullptr);

    //!
    //! \brief event
    //! \param event
    //! \return
    //! Handling DynamicProperty
    bool event(QEvent* event) Q_DECL_OVERRIDE;

	QObject* port() const;


	void activateBlink();

signals:

public slots:
protected slots:
    void onCustomContextMenuShowed(const QPoint position);
    void onReloadQss();
	void onLinkQSSFile();

	virtual void onPropertyChanged(QVariant key, QVariant value);
	virtual void onUpdate();
protected:
	void LinkController(QObject* controller);
	

    QList<QWidget*> m_widgetsPolish;
    //!
    //! \brief m_controller
    //! Auto find-out linked controller
    PropertyPortCommon* m_port;
    //!
    //! \brief property
    //! \param key
    //! \return
    //! wrapper function
    QVariant property(QVariant key) const;

    virtual void dynamicPropertyChanged(QString key,QVariant value){}
    virtual void dynamicPropertyChanged(int key,QVariant value){}
private:
    bool m_isQSSInitialized;
	QFileSystemWatcher* m_watcher;
	QTimer* m_blinkTimer;

	QTimer* m_updateTimer;
};

#endif // FRONTCOMMON_H
