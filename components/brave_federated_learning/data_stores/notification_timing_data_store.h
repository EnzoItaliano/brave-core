/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_FEDERATED_LEARNING_DATA_STORES_NOTIFICATION_TIMING_DATA_STORE_H_
#define BRAVE_COMPONENTS_BRAVE_FEDERATED_LEARNING_DATA_STORES_NOTIFICATION_TIMING_DATA_STORE_H_

#include <map>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "brave/components/brave_federated_learning/data_stores/data_store.h"
#include "sql/database.h"
#include "sql/meta_table.h"
#include "ui/base/page_transition_types.h"
#include "url/gurl.h"

namespace brave {

namespace federated_learning {

// Log Definition --------------------------------------------------------
struct NotificationTimingTaskLog {
  NotificationTimingTaskLog(const std::string& id,
                            const std::string& time,
                            const std::string& locale,
                            const int number_of_tabs,
                            const std::string& creation_date);
  NotificationTimingTaskLog(const NotificationTimingTaskLog& other);
  NotificationTimingTaskLog();
  ~NotificationTimingTaskLog();

  std::string id;
  std::string time;
  std::string locale;
  int number_of_tabs;
  std::string creation_date;
};

class NotificationTimingDataStore : public DataStore {
 public:
  explicit NotificationTimingDataStore(const base::FilePath& database_path);
  ~NotificationTimingDataStore();

  NotificationTimingDataStore(const NotificationTimingDataStore&) = delete;
  NotificationTimingDataStore& operator=(const NotificationTimingDataStore&) =
      delete;

  typedef std::map<std::string, NotificationTimingTaskLog>
      GuidToNotificationTimingTaskLogMap;

  using DataStore::DeleteAllLogs;
  using DataStore::Init;

  bool AddLog(const NotificationTimingTaskLog& log);
  void LoadLogs(GuidToNotificationTimingTaskLogMap* notification_timing_logs);
  bool EnsureTable() override;
};

}  // namespace federated_learning
}  // namespace brave

#endif  // BRAVE_COMPONENTS_BRAVE_FEDERATED_LEARNING_DATA_STORES_NOTIFICATION_TIMING_DATA_STORE_H_
