/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_federated_learning/data_stores/notification_timing_data_store.h"

#include <string>
#include <utility>

#include "base/bind.h"
#include "base/guid.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/macros.h"
#include "base/numerics/safe_conversions.h"
#include "base/strings/stringprintf.h"
#include "base/time/time.h"
#include "brave/components/brave_federated_learning/data_stores/data_store.h"
#include "sql/meta_table.h"
#include "sql/recovery.h"
#include "sql/statement.h"
#include "sql/transaction.h"
#include "ui/base/page_transition_types.h"

namespace {

void BindSampleLogToStatement(
    const brave::federated_learning::NotificationTimingTaskLog&
        notification_timing_log,
    sql::Statement* s) {
  // DCHECK(base::IsValidGUID(sample_log.id));
  s->BindString(0, notification_timing_log.id);
  s->BindString(1, notification_timing_log.time);
  s->BindString(2, notification_timing_log.locale);
  s->BindInt(3, notification_timing_log.number_of_tabs);
  s->BindString(4, notification_timing_log.creation_date);
}

}  // namespace

namespace brave {

namespace federated_learning {

// NotificationTimingTaskLog ---------------------------------------

NotificationTimingTaskLog::NotificationTimingTaskLog(
    const std::string& id,
    const std::string& time,
    const std::string& locale,
    const int number_of_tabs,
    const std::string& creation_date)
    : id(id),
      time(time),
      locale(locale),
      number_of_tabs(number_of_tabs),
      creation_date(creation_date) {}

NotificationTimingTaskLog::NotificationTimingTaskLog(
    const NotificationTimingTaskLog& other) = default;

NotificationTimingTaskLog::NotificationTimingTaskLog() {}
NotificationTimingTaskLog::~NotificationTimingTaskLog() {}

// NotificationTimingDataStore
// -----------------------------------------------------

NotificationTimingDataStore::NotificationTimingDataStore(
    const base::FilePath& database_path)
    : DataStore(database_path) {}

bool NotificationTimingDataStore::AddLog(const NotificationTimingTaskLog& log) {
  sql::Statement s(db_.GetUniqueStatement(
      base::StringPrintf(
          "INSERT INTO %s (id, time, locale, number_of_tabs, creation_date) "
          "VALUES (?,?,?,?)",
          task_name_.c_str())
          .c_str()));
  BindSampleLogToStatement(log, &s);
  return s.Run();
}

void NotificationTimingDataStore::LoadLogs(
    GuidToNotificationTimingTaskLogMap* notification_timing_logs) {
  DCHECK(notification_timing_logs);
  sql::Statement s(db_.GetUniqueStatement(
      base::StringPrintf(
          "SELECT id, time, locale, number_of_tabs, creation_date FROM %s",
          task_name_.c_str())
          .c_str()));

  notification_timing_logs->clear();
  while (s.Step()) {
    NotificationTimingTaskLog ntl(s.ColumnString(0), s.ColumnString(1),
                                  s.ColumnString(2), s.ColumnInt(3),
                                  s.ColumnString(4));
    notification_timing_logs->insert(std::make_pair(s.ColumnString(0), ntl));
  }
}

NotificationTimingDataStore::~NotificationTimingDataStore() {}

bool NotificationTimingDataStore::EnsureTable() {
  if (db_.DoesTableExist(task_name_))
    return true;

  sql::Transaction transaction(&db_);
  return transaction.Begin() &&
         db_.Execute(base::StringPrintf(
                         "CREATE TABLE %s (id TEXT PRIMARY KEY, "
                         "time TEXT, locale TEXT, number_of_tabs INTEGER, "
                         "creation_date TEXT)",
                         task_name_.c_str())
                         .c_str()) &&
         transaction.Commit();
}

}  // namespace federated_learning
}  // namespace brave
