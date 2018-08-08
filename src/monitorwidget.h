#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include <QSettings>

class MonitorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MonitorWidget(QWidget *parent = nullptr);

    bool enabled();
    void setEnabled(const bool b);

    QString text;

public slots:

private:
    QSettings m_settings;
    QSize sizeHint() const;
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);


};

#endif // MONITORWIDGET_H
