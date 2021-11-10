/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_federated_learning/brave_federated_data_service.h"

#include <map>
#include <string>
#include <utility>

#include "brave/components/brave_federated_learning/data_stores/data_store.h"
#include "brave/components/brave_federated_learning/data_stores/notification_timing_data_store.h"
#include "sql/recovery.h"
#include "sql/statement.h"
#include "sql/transaction.h"

namespace brave {

BraveFederatedDataService::BraveFederatedDataService(
    const base::FilePath& database_path)
    : base_database_path_(database_path) {}

BraveFederatedDataService::~BraveFederatedDataService() {}

bool BraveFederatedDataService::Init() {
  federated_learning::NotificationTimingDataStore* notification_timing_ds =
      new federated_learning::NotificationTimingDataStore(base_database_path_);
  const std::string task_name = "notification_timing_federated_task";
  const std::string task_id = "0.0.1";

  const bool success = notification_timing_ds->Init(task_id, task_name);
  if (!success)
    return false;

  data_stores_.insert(std::make_pair(task_name, notification_timing_ds));
  return true;
}

absl::optional<federated_learning::DataStore*>
BraveFederatedDataService::GetDataStore(const std::string& task_name) {
  auto it = data_stores_.find(task_name);
  if (it != data_stores_.end()) {
    return absl::optional<federated_learning::DataStore*>(it->second);
  }

  return absl::nullopt;
}

bool BraveFederatedDataService::PurgeTaskDataStore(
    const std::string& task_name) {
  absl::optional<federated_learning::DataStore*> data_store =
      GetDataStore(task_name);
  if (data_store) {
    return (*data_store)->DeleteAllLogs();
  } else {
    return false;
  }
}

bool BraveFederatedDataService::PurgeDataService() {
  return sql::Database::Delete(base_database_path_);
}

}  // namespace brave
