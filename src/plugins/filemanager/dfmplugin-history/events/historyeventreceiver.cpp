// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "historyeventreceiver.h"
#include "utils/historymanager.h"

#include <dfm-base/dfm_event_defines.h>
#include <dfm-base/utils/systempathutil.h>

#include <QFileInfo>
#include <QDir>

using namespace dfmplugin_history;
DFMBASE_USE_NAMESPACE

HistoryEventReceiver *HistoryEventReceiver::instance()
{
    static HistoryEventReceiver ins;
    return &ins;
}

HistoryEventReceiver::HistoryEventReceiver(QObject *parent)
    : QObject(parent)
{
}

void HistoryEventReceiver::handleWindowUrlChanged(quint64 winId, const QUrl &url)
{
    Q_UNUSED(winId)

    // Only record local file directories
    if (!url.isValid() || !url.isLocalFile())
        return;

    QFileInfo info(url.toLocalFile());
    if (!info.exists() || !info.isDir())
        return;

    HistoryManager::instance()->addEntry(url, false);
}

void HistoryEventReceiver::handleOpenFiles(quint64 winId, const QList<QUrl> &urls)
{
    Q_UNUSED(winId)

    for (const QUrl &url : urls) {
        if (!url.isValid() || !url.isLocalFile())
            continue;

        QFileInfo info(url.toLocalFile());
        if (!info.exists())
            continue;

        if (info.isFile()) {
            HistoryManager::instance()->addEntry(url, true);
        }
    }
}
