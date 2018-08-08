#include "monitorplugin.h"
#include <QApplication>

MonitorPlugin::MonitorPlugin(QObject *parent) : QObject (parent),
    m_settings("deepin","dde-dock-monitor"),
    m_refershTimer(new QTimer(this))
{
    i=DB=UB=DBT0=DBT1=UBT0=UBT1=DownSpeed=UpSpeed=0;
    m_refershTimer->setInterval(1000);
    m_refershTimer->start();

    m_centralWidget = new MonitorWidget;

    connect(m_refershTimer,&QTimer::timeout,this,&MonitorPlugin::updateMonitor);

}

const QString MonitorPlugin::pluginName() const
{
    return "monitor";
}

const QString MonitorPlugin::pluginDisplayName() const
{
    return "网速";
}

void MonitorPlugin::init(PluginProxyInterface *proxyInter){
    m_proxyInter =proxyInter;

    if(m_centralWidget->enabled())
    {
        m_proxyInter->itemAdded(this,QString());
    }
}

void MonitorPlugin::pluginStateSwitched()
{
    m_centralWidget->setEnabled(!m_centralWidget->enabled());
    if(m_centralWidget->enabled())
    {
        m_proxyInter->itemAdded(this,QString());
    }
    else
    {
        m_proxyInter->itemRemoved(this,QString());
    }
}

bool MonitorPlugin::pluginIsDisable()
{
    return !m_centralWidget->enabled();
}

int MonitorPlugin::itemSortKey(const QString &itemkey)
{
    Q_UNUSED(itemkey);

    const QString key = QString("pos_%1").arg(displayMode());
    return m_settings.value(key, 0).toInt();
}

void MonitorPlugin::setSortKey(const QString &itemkey,const int order)
{
    Q_UNUSED(itemkey);

    const QString key = QString("pos_%1").arg(displayMode());
    m_settings.setValue(key, order);
}

QWidget *MonitorPlugin::itemWidget(const QString &itemkey)
{
    Q_UNUSED(itemkey);
    return m_centralWidget;
}

QString MonitorPlugin::format(long b)
{
    QString string = "";
    if(b >99999999){
        string = QString::number(b/(1024*1024*1024.0),'f',2) + "GB/s";
    }else if (b>999999) {
        string = QString::number(b/(1024*1024.0),'f',2) + "MB/s";
    }else if (b>999){
        string = QString::number(b/1024.0,'f',2) + "KB/s";
    }else{
        string = QString::number(0,'f',2) + "KB/s";
    }
    return string;
}

void MonitorPlugin::updateMonitor()
{
    //单位时间字节数即网速
    QFile file("/proc/net/dev");
    file.open(QIODevice::ReadOnly);
    QString netInfo = file.readLine();
    DBT1=UBT1=0;
    while (!file.atEnd()) {
        netInfo = file.readLine();
        QStringList list = netInfo.split(QRegExp("\\s{1,}"));
        DB = list.at(1).toLong();
        UB = list.at(9).toLong();
        DBT1 += DB;
        UBT1 += UB;
    }
    file.close();
    if(i>0){
        DownSpeed = DBT1-DBT0;
        UpSpeed = UBT1-UBT0;
        UBT0 = UBT1;
        DBT0 = DBT1;
    }
    QString netspeed = "▲ "+format(UpSpeed)+"\n▼ "+format(DownSpeed);
    i++;
    if(i>2)i=2;

    //绘制
    m_centralWidget->text=netspeed;
    m_centralWidget->update();
}
