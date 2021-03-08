#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include "appconfig.h"

Q_GLOBAL_STATIC(AppConfig, appConfig)

AppConfig *AppConfig::instance()
{
	return appConfig;
}


AppConfig::AppConfig(QObject *parent) : QObject(parent),
										m_fileConfig(QLatin1String(""))
{
}

AppConfig::~AppConfig()
{
}

bool AppConfig::read(const QString &fileConfig)
{
	QFileInfo fInfo(fileConfig);
	if (!fInfo.exists())
	{
		qDebug() << QString(tr("%1 not exist")).arg(fileConfig);
		return false;
	}
	m_fileConfig = fileConfig;

	QFile config;
	QByteArray data;
	config.setFileName(fInfo.absoluteFilePath());
	if (config.open(QIODevice::ReadOnly))
	{
		data = config.readAll();
		config.close();
	}
	else
	{
		qDebug() << QString(tr("Can not open configuration file %1")).arg(fInfo.absoluteFilePath());
		return false;
	}

	QJsonObject json = QJsonDocument::fromJson(data).object();

    (void)json;

	return true;
}

void AppConfig::save()
{
	QJsonObject json;

    (void)json;

	if (m_fileConfig.isEmpty())
	{
		m_fileConfig = QLatin1String("BrowserDemo.json");
	}

	QJsonDocument doc(json);
	QFile config(m_fileConfig);
	if (config.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		config.write(doc.toJson());
		config.close();
	}
}