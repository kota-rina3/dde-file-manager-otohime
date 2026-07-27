// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "historysidebarhelper.h"
#include "historymanager.h"

#include <dfm-base/widgets/filemanagerwindowsmanager.h>
#include <dfm-base/dfm_event_defines.h>

#include <dfm-framework/dpf.h>

#include <QMenu>
#include <QFileInfo>
#include <QDesktopServices>
#include <QIcon>
#include <QPoint>

#include <functional>

using namespace dfmplugin_history;
DFMBASE_USE_NAMESPACE

// Redefine types locally (same signature as sidebar plugin uses)
using ItemClickedActionCallback = std::function<void(quint64 windowId, const QUrl &url)>;
using ContextMenuCallback = std::function<void(quint64 windowId, const QUrl &url, const QPoint &globalPos)>;
Q_DECLARE_METATYPE(ItemClickedActionCallback);
Q_DECLARE_METATYPE(ContextMenuCallback);

HistorySidebarHelper *HistorySidebarHelper::instance()
{
    static HistorySidebarHelper ins;
    return &ins;
}

HistorySidebarHelper::HistorySidebarHelper(QObject *parent)
    : QObject(parent)
{
}

void HistorySidebarHelper::registerToSidebar()
{
    refreshSidebarItems();

    connect(HistoryManager::instance(), &HistoryManager::historyChanged,
            this, &HistorySidebarHelper::refreshSidebarItems, Qt::QueuedConnection);
}

void HistorySidebarHelper::refreshSidebarItems()
{
    removeAllHistoryItems();

    const QList<HistoryEntry> entries = HistoryManager::instance()->entries();
    for (const auto &entry : entries) {
        QIcon icon;
        if (entry.isFile)
            icon = QIcon::fromTheme("text-x-generic");
        else
            icon = QIcon::fromTheme("folder");

        ItemClickedActionCallback clickCb { HistorySidebarHelper::itemClicked };
        ContextMenuCallback contextMenuCb { HistorySidebarHelper::contextMenu };

        Qt::ItemFlags flags { Qt::ItemIsEnabled | Qt::ItemIsSelectable };

        QVariantMap map {
            { "Property_Key_Group", QString("Group_History") },
            { "Property_Key_DisplayName", entry.displayName },
            { "Property_Key_Icon", icon },
            { "Property_Key_QtItemFlags", QVariant::fromValue(flags) },
            { "Property_Key_CallbackItemClicked", QVariant::fromValue(clickCb) },
            { "Property_Key_CallbackContextMenu", QVariant::fromValue(contextMenuCb) },
            { "Property_Key_VisiableControl", QString("history_%1").arg(entry.url.toString()) },
        };

        dpfSlotChannel->push("dfmplugin_sidebar", "slot_Item_Add", entry.url, map);
    }
}

void HistorySidebarHelper::clearSidebarHistory()
{
    HistoryManager::instance()->clearAll();
}

void HistorySidebarHelper::removeSidebarItem(const QUrl &url)
{
    HistoryManager::instance()->removeEntry(url);
}

void HistorySidebarHelper::itemClicked(quint64 windowId, const QUrl &url)
{
    QFileInfo info(url.toLocalFile());
    if (!info.exists())
        return;

    if (info.isDir()) {
        dpfSignalDispatcher->publish(GlobalEventType::kChangeCurrentUrl, windowId, url);
    } else if (info.isFile()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(url.toLocalFile()));
    }
}

void HistorySidebarHelper::contextMenu(quint64 windowId, const QUrl &url, const QPoint &globalPos)
{
    Q_UNUSED(windowId)

    QMenu *menu = new QMenu;
    menu->addAction(QObject::tr("Open"), [url, windowId]() {
        HistorySidebarHelper::itemClicked(windowId, url);
    });

    menu->addSeparator();

    menu->addAction(QObject::tr("Remove from history"), [url]() {
        HistorySidebarHelper::instance()->removeSidebarItem(url);
    });

    menu->addAction(QObject::tr("Clear all history"), []() {
        HistorySidebarHelper::instance()->clearSidebarHistory();
    });

    menu->exec(globalPos);
    delete menu;
}

void HistorySidebarHelper::removeAllHistoryItems()
{
    const QList<HistoryEntry> entries = HistoryManager::instance()->entries();
    for (const auto &entry : entries) {
        dpfSlotChannel->push("dfmplugin_sidebar", "slot_Item_Remove", entry.url);
    }
}
