#ifndef MONITORPLUGIN_H
#define MONITORPLUGIN_H

#include "monitorwidget.h"
#include "dde-dock/pluginsiteminterface.h"
#include <QTimer>

class MonitorPlugin : public QObject,PluginsItemInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginsItemInterface)
    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginItemInterface" FILE "libmonitor.json")

public:
    explicit MonitorPlugin(QObject *parent = nullptr);

    const QString pluginName() const override;
    const QString pluginDisplayName() const override;
    void init(PluginProxyInterface *proxyInter) override;

    void pluginStateSwitched() override;
    bool pluginIsAllowDisable() override {return true;}
    bool pluginIsDisable() override;

    int itemSortKey(const QString &itemkey);
    void setSortKey(const QString &itemkey,const int order);

    QWidget *itemWidget(const QString &itemkey) override;

private slots:
    void updateMonitor();

private:
    long int i, DB, UB, DBT0,UBT0,DBT1,UBT1,DownSpeed,UpSpeed;
    QPointer<MonitorWidget> m_centralWidget;
    QSettings m_settings;
    QString format(long b);

    QTimer *m_refershTimer;
};

#endif // MONITORPLUGIN_H
