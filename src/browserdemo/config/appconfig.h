#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <QtCore/QObject>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class AppConfig : public QObject
{
    Q_OBJECT

public:
    AppConfig(QObject *parent = 0);
    ~AppConfig();

    static AppConfig *instance();

    bool read(const QString &fileConfig);
    void save();

public:

private:
    QString m_fileConfig;

    Q_DISABLE_COPY(AppConfig)
};

#endif // APP_CONFIG_H
