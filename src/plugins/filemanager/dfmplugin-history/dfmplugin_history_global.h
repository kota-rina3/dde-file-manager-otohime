// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DFMPLUGIN_HISTORY_GLOBAL_H
#define DFMPLUGIN_HISTORY_GLOBAL_H

#include <dfm-base/dfm_log_defines.h>

#define DPHISTORY_NAMESPACE dfmplugin_history

#define DPHISTORY_BEGIN_NAMESPACE namespace DPHISTORY_NAMESPACE {
#define DPHISTORY_END_NAMESPACE }
#define DPHISTORY_USE_NAMESPACE using namespace DPHISTORY_NAMESPACE;

DPHISTORY_BEGIN_NAMESPACE
DFM_LOG_USE_CATEGORY(DPHISTORY_NAMESPACE)

namespace HistoryActionID {
inline constexpr char kOpen[] { "open" };
inline constexpr char kRemove[] { "remove" };
inline constexpr char kClear[] { "clear" };
}   // namespace HistoryActionID

DPHISTORY_END_NAMESPACE

#endif   // DFMPLUGIN_HISTORY_GLOBAL_H
