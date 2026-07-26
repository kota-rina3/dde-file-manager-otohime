// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HISTORYPLUGIN_H
#define HISTORYPLUGIN_H

#include "dfmplugin_history_global.h"

#include <dfm-framework/dpf.h>

namespace dfmplugin_history {
class History : public dpf::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.deepin.plugin.filemanager" FILE "history.json")

    DPF_EVENT_NAMESPACE(DPHISTORY_NAMESPACE)

public:
    virtual void initialize() override;
    virtual bool start() override;

private:
    void bindEvents();
    void bindWindows();
    void onWindowOpened(quint64 winId);
    void addToSidebar();
};

}   // namespace dfmplugin_history

#endif   // HISTORYPLUGIN_H
