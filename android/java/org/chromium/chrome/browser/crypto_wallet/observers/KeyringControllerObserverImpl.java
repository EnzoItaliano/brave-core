/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.observers;

import org.chromium.brave_wallet.mojom.KeyringControllerObserver;
import org.chromium.chrome.browser.crypto_wallet.activities.BraveWalletActivity;
import org.chromium.mojo.system.MojoException;

public class KeyringControllerObserverImpl implements KeyringControllerObserver {
    private BraveWalletActivity mParentActivity;

    public KeyringControllerObserverImpl(BraveWalletActivity parentActivity) {
        mParentActivity = parentActivity;
    }

    @Override
    public void keyringCreated() {}

    @Override
    public void keyringRestored() {}

    @Override
    public void locked() {
        mParentActivity.lockWallet();
    }

    @Override
    public void unlocked() {}

    @Override
    public void backedUp() {}

    @Override
    public void accountsChanged() {}

    @Override
    public void autoLockMinutesChanged() {}

    @Override
    public void selectedAccountChanged() {}

    @Override
    public void close() {}

    @Override
    public void onConnectionError(MojoException e) {}
}
