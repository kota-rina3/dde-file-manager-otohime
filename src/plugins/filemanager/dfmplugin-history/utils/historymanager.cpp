// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "historymanager.h"

#include <dfm-base/base/application/application.h>
#include <dfm-base/base/application/settings.h>

#include <QFileInfo>

using namespace dfmplugin_history;
DFMBASE_USE_NAMESPACE

HistoryManager *HistoryManager::instance()
{
    static HistoryManager ins;
    return &ins;
}

HistoryManager::HistoryManager(QObject *parent)
    : QObject(parent)
{
    load();
}

HistoryManager::~HistoryManager()
{
    save();
}

void HistoryManager::load()
{
    const QVariantList list = Application::appObtuselySetting()->value(kConfigGroup, kConfigKey).toList();
    historyEntries.clear();

    for (const QVariant &var : list) {
        const QVariantMap map = var.toMap();
        HistoryEntry entry;
        entry.url = QUrl(map.value("url").toString());
        entry.displayName = map.value("displayName").toString();
        entry.timestamp = QDateTime::fromString(map.value("timestamp").toString(), Qt::ISODate);
        entry.isFile = map.value("isFile", false).toBool();

        if (entry.url.isValid() && entry.url.isLocalFile()) {
            // Only keep entries that still exist
            QFileInfo fi(entry.url.toLocalFile());
            if (fi.exists()) {
                if (entry.displayName.isEmpty())
                    entry.displayName = fi.fileName();
                historyEntries.append(entry);
            }
        }
    }
}

void HistoryManager::save()
{
    QVariantList list;
    for (const auto &entry : historyEntries) {
        QVariantMap map;
        map["url"] = entry.url.toString();
        map["displayName"] = entry.displayName;
        map["timestamp"] = entry.timestamp.toString(Qt::ISODate);
        map["isFile"] = entry.isFile;
        list.append(map);
    }
    Application::appObtuselySetting()->setValue(kConfigGroup, kConfigKey, list);
}

void HistoryManager::addEntry(const QUrl &url, bool isFile)
{
    if (!url.isValid() || !url.isLocalFile())
        return;

    // Don't add special system paths
    QString localPath = url.toLocalFile();
    if (localPath.isEmpty())
        return;

    // Remove existing entry for this URL (to move it to top)
    for (int i = 0; i < historyEntries.size(); ++i) {
        if (historyEntries.at(i).url == url) {
            historyEntries.removeAt(i);
            break;
        }
    }

    // Create new entry and prepend
    HistoryEntry entry;
    entry.url = url;
    entry.timestamp = QDateTime::currentDateTime();
    entry.isFile = isFile;

    QFileInfo fi(localPath);
    entry.displayName = fi.fileName();

    historyEntries.prepend(entry);

    // Trim to max size
    while (historyEntries.size() > kMaxEntries)
        historyEntries.removeLast();

    save();
    Q_EMIT historyChanged();
}

void HistoryManager::removeEntry(const QUrl &url)
{
    for (int i = 0; i < historyEntries.size(); ++i) {
        if (historyEntries.at(i).url == url) {
            historyEntries.removeAt(i);
            save();
            Q_EMIT historyChanged();
            return;
        }
    }
}

void HistoryManager::clearAll()
{
    if (historyEntries.isEmpty())
        return;

    historyEntries.clear();
    save();
    Q_EMIT historyChanged();
}

QList<HistoryEntry> HistoryManager::entries() const
{
    return historyEntries;
}

int HistoryManager::count() const
{
    return historyEntries.size();
}
