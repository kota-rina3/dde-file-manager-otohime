// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HISTORYSIDEBARHELPER_H
#define HISTORYSIDEBARHELPER_H

#include "dfmplugin_history_global.h"

#include <QObject>
#include <QUrl>

namespace dfmplugin_history {

class HistorySidebarHelper : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(HistorySidebarHelper)

public:
    static HistorySidebarHelper *instance();

    void registerToSidebar();
    void refreshSidebarItems();
    void clearSidebarHistory();
    void removeSidebarItem(const QUrl &url);

    static void itemClicked(quint64 windowId, const QUrl &url);
    static void contextMenu(quint64 windowId, const QUrl &url, const QPoint &globalPos);

private:
    explicit HistorySidebarHelper(QObject *parent = nullptr);

    void removeAllHistoryItems();
};

}   // namespace dfmplugin_history

#endif   // HISTORYSIDEBARHELPER_H
