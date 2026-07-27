// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include "dfmplugin_history_global.h"

#include <QObject>
#include <QUrl>
#include <QList>
#include <QDateTime>

namespace dfmplugin_history {

struct HistoryEntry
{
    QUrl url;
    QString displayName;
    QDateTime timestamp;
    bool isFile = false;
};

class HistoryManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(HistoryManager)

public:
    static HistoryManager *instance();

    void load();
    void save();

    void addEntry(const QUrl &url, bool isFile = false);
    void removeEntry(const QUrl &url);
    void clearAll();

    QList<HistoryEntry> entries() const;
    int count() const;

signals:
    void historyChanged();

private:
    explicit HistoryManager(QObject *parent = nullptr);
    ~HistoryManager() override;

    QList<HistoryEntry> historyEntries;
    static constexpr int kMaxEntries = 50;
    static constexpr char kConfigGroup[] { "History" };
    static constexpr char kConfigKey[] { "HistoryEntries" };
};

}   // namespace dfmplugin_history

#endif   // HISTORYMANAGER_H
