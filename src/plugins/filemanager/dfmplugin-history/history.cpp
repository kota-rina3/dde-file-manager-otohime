// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "history.h"
#include "events/historyeventreceiver.h"
#include "utils/historymanager.h"
#include "utils/historysidebarhelper.h"

#include <dfm-base/dfm_event_defines.h>
#include <dfm-base/widgets/filemanagerwindowsmanager.h>
#include <dfm-base/widgets/filemanagerwindow.h>

#include <dfm-framework/dpf.h>

namespace dfmplugin_history {
DFM_LOG_REGISTER_CATEGORY(DPHISTORY_NAMESPACE)

DFMBASE_USE_NAMESPACE

void History::initialize()
{
    bindEvents();
    bindWindows();
}

bool History::start()
{
    return true;
}

void History::bindEvents()
{
    dpfSignalDispatcher->subscribe(GlobalEventType::kChangeCurrentUrl,
                                   HistoryEventReceiver::instance(),
                                   &HistoryEventReceiver::handleWindowUrlChanged);
    dpfSignalDispatcher->subscribe(GlobalEventType::kOpenFiles,
                                   HistoryEventReceiver::instance(),
                                   &HistoryEventReceiver::handleOpenFiles);
}

void History::bindWindows()
{
    const auto &winIdList { FMWindowsIns.windowIdList() };
    std::for_each(winIdList.begin(), winIdList.end(), [this](quint64 id) {
        onWindowOpened(id);
    });
    connect(&FMWindowsIns, &FileManagerWindowsManager::windowOpened,
            this, &History::onWindowOpened, Qt::DirectConnection);
}

void History::onWindowOpened(quint64 winId)
{
    auto window = FMWindowsIns.findWindowById(winId);
    Q_ASSERT_X(window, "History", "Cannot find window by id");

    if (window->sideBar())
        addToSidebar();
    else
        connect(window, &FileManagerWindow::sideBarInstallFinished,
                this, &History::addToSidebar, Qt::DirectConnection);
}

void History::addToSidebar()
{
    static std::once_flag flag;
    std::call_once(flag, [this]() {
        HistorySidebarHelper::instance()->registerToSidebar();
    });
}

}   // namespace dfmplugin_history
