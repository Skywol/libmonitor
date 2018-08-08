#include "monitorwidget.h"
#include <QPainter>
#include <QApplication>

#define PLUGIN_STATE_KEY    "enable"

MonitorWidget::MonitorWidget(QWidget *parent) : QWidget(parent),m_settings("deepin","dde-dock-monitor")
{
    text = "▲0.00KB/s\n▼0.00KB/s";
}

bool MonitorWidget::enabled()
{
    return m_settings.value(PLUGIN_STATE_KEY,true).toBool();
}

void MonitorWidget::setEnabled(const bool b)
{
    m_settings.setValue(PLUGIN_STATE_KEY,b);
}

QSize MonitorWidget::sizeHint() const
{
    QFontMetrics fm(qApp->font());
    return fm.boundingRect("▲ 000.00KB/s").size()+QSize(10,fm.boundingRect("▼ 000.00KB/s").height());
}

void MonitorWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void MonitorWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::white);
    painter.drawText(rect(),Qt::AlignLeft | Qt::AlignVCenter,text);
}
